
#include "myWiFiManager.h"
#include "MiningJob.h"
#include <Arduino.h>

bool showDebugMessage = false;
bool wiFiManagerDebugOutput = true;
char setupAPName[30] = "DuinoCoin-AP";
char loopAPName[30] = "DuinoCoin-AP";
unsigned int setConnectTime = 20;
bool shouldSaveConfig = false;

const char DUCO_USER[] = "my_cool_username";
const char MINER_KEY[] = "mySecretPass";
const char SSID[] = "SSID";
const char PASSWORD[] = "PASSW0RD";
const char RIG_IDENTIFIER[] = "None";
unsigned int hashrate = 0;
unsigned int difficulty = 0;
unsigned long share_count = 0;
String node_id = "";
String WALLET_ID = "";

namespace {


    String readFile(const char* path) {
        if (showDebugMessage)
            Serial.printf("Reading file: %s\r\n", path);
        File file = SPIFFS.open(path, "r");
        if (!file || file.isDirectory()) {
            if (showDebugMessage)
                Serial.println("- empty file or failed to open file");
            return String();
        }
        if (showDebugMessage)
            Serial.print("- read from file:");
        String fileContent;
        while (file.available()) {
            fileContent += String((char)file.read());
        }
        file.close();
        if (showDebugMessage)
            Serial.println(fileContent);
        return fileContent;
    }
    void writeFile(const char* path, const char* message) {
        if (showDebugMessage)
            Serial.printf("Writing file: %s\r\n", path);
        File file = SPIFFS.open(path, "w");
        if (!file) {
            if (showDebugMessage)
                Serial.println("- failed to open file for writing");
            return;
        }
        bool res = file.print(message);
        if (showDebugMessage) {
            if (res) {
                Serial.println("- file written");
            }
            else {
                Serial.println("- write failed");
            }
        }
        delay(50);  // Make sure the CREATE and LASTWRITE times are different
        file.close();
    }

    //callback notifying us of the need to save config
    void saveConfigCallback() {
        if (showDebugMessage)
            Serial.println("Set shouldSaveConfig = true");
        shouldSaveConfig = true;
    }

    void ReadFSData() {
        if (!SPIFFS.begin()) { //to start littlefs
            if (showDebugMessage)
                Serial.println("SPIFFS mount failed");
            return;
        }
        else {
            if (showDebugMessage)
                Serial.println("SPIFFS Mounted Successfully");
        }
        if (showDebugMessage)
            Serial.println("Should read FS data");

        if (readFile("/account.txt") == "") {
            configuration->DUCO_USER = "919ethan";
            //strcpy(account, "919ethan");
        }
        else {
            configuration->DUCO_USER = readFile("/account.txt");
            //strcpy(account, readFile("/account.txt").c_str());
        }

        if (readFile("/minerKey.txt") == "") {
            configuration->MINER_KEY = "Qq681417";
            //strcpy(minerKey, "Qq681417");
        }
        else {
            configuration->MINER_KEY = readFile("/minerKey.txt");
            //strcpy(minerKey, readFile("/minerKey.txt").c_str());
        }

        if (readFile("/rigName.txt") == "") {
            configuration->RIG_IDENTIFIER = "ESP8266_";
            //strcpy(rigName, "ESP8266_");
        }
        else {
            configuration->RIG_IDENTIFIER = readFile("/rigName.txt");
            //strcpy(rigName, readFile("/rigName.txt").c_str());
        }

        SPIFFS.end();
    }

    void WriteDataToFS(String whoCallTheFunction, String writeAccountData, String writeMinerKeyData, String writeRigNameData) {


        if (!SPIFFS.begin()) { //to start littlefs
            if (showDebugMessage)
                Serial.println("SPIFFS mount failed");
            return;
        }
        else {
            if (showDebugMessage)
                Serial.println("SPIFFS Mounted Successfully");
        }
        if (showDebugMessage)
            Serial.println(whoCallTheFunction + ": Should save config");
        writeFile("/account.txt", writeAccountData.c_str());
        writeFile("/minerKey.txt", writeMinerKeyData.c_str());
        writeFile("/rigName.txt", writeRigNameData.c_str());
        SPIFFS.end();
        ReadFSData();
    }

    void ShowConfigData(bool WiFiIsSaved, String WiFiSSID, String WiFiPWD) {
        Serial.println("The values in the file are: ");
        if (WiFiIsSaved) {
            Serial.println("SSIDforRun : " + WiFiSSID);
            Serial.println("PASSWORDforRun : " + WiFiPWD);
        }
        Serial.println("DUCO_USER : " + configuration->DUCO_USER);
        Serial.println("MINER_KEY : " + configuration->MINER_KEY);
        Serial.println("RIG_IDENTIFIER : " + configuration->RIG_IDENTIFIER);
    }

    void SecondToRestart() {
        Serial.println("Failed to connect");
        for (int i = 3; i > 0; i--)
        {
            Serial.println("Will restart in " + String(i) + "second");
            delay(1000);
        }
        ESP.restart();
    }

    void setupWiFiManager() {
        shouldSaveConfig = false;
        ReadFSData();

        // The extra parameters to be configured (can be either global or just in the setup)
        // After connecting, parameter.getValue() will get you the configured value
        // id/name placeholder/prompt default length
        WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", configuration->DUCO_USER.c_str(), 30);
        WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", configuration->MINER_KEY.c_str(), 30);
        WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", configuration->RIG_IDENTIFIER.c_str(), 30);

        WiFiManager wm;
        wm.setSaveConfigCallback(saveConfigCallback); //set config save notify callback
        wm.setDarkMode(true); // 設定是否為黑底模式
        wm.setDebugOutput(wiFiManagerDebugOutput);  // 輸出除錯訊息
        wm.setBreakAfterConfig(true);
        //wm.resetSettings();
        //wm.setTimeout(12);
        //wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
        //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
        wm.setScanDispPerc(true); // 是否百分比顯示
        wm.setMinimumSignalQuality(40); // 設定最低訊號強度百分比(數字)，預設為8


        //add all your parameters here
        wm.addParameter(&custom_account);
        wm.addParameter(&custom_minigkey);
        wm.addParameter(&custom_rigname);

        while (!wm.autoConnect(setupAPName)) {
            wm.autoConnect(setupAPName);
        }

        if (showDebugMessage)
            Serial.println("shouldSaveConfig is: " + String(shouldSaveConfig));

        //save the custom parameters to FS
        if (shouldSaveConfig) {
            WriteDataToFS(setupAPName, custom_account.getValue(), custom_minigkey.getValue(), custom_rigname.getValue());
        }

        Serial.println("\n\nconnected to " + wm.getWiFiSSID() + " success!!");
        Serial.println("Local IP address: " + WiFi.localIP().toString());
        if (showDebugMessage) {
            Serial.println("configuration->DUCO_USER: " + configuration->DUCO_USER);
            Serial.println("configuration->MINER_KEY: " + configuration->MINER_KEY);
            Serial.println("configuration->RIG_IDENTIFIER: " + configuration->RIG_IDENTIFIER);
        }
        Serial.println();

        if (showDebugMessage && WiFi.waitForConnectResult() == WL_CONNECTED) {
            ShowConfigData(wm.getWiFiIsSaved(), wm.getWiFiSSID(), wm.getWiFiPass());
        }

        SelectNode();
    }

    void doWiFiManager() {   // 執行WiFi管理員的工作
        // // 若按鈕<Flash>被按下1.5秒
        if (digitalRead(TRIGGER_PIN) == LOW) {
            delay(50);
            if (digitalRead(TRIGGER_PIN) == LOW) {
                delay(1500);
                if (digitalRead(TRIGGER_PIN) == LOW) {
                    shouldSaveConfig = false;
                    Serial.println("按鈕<Flash>被按下1.5秒了，啟動設置入口");
                    ReadFSData();

                    if (showDebugMessage) {
                        strcpy(loopAPName, "LoopDuinoCoin-AP");
                    }

                    WiFiManager wm;
                    // The extra parameters to be configured (can be either global or just in the setup)
                    // After connecting, parameter.getValue() will get you the configured value
                    // id/name placeholder/prompt default length
                    WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", configuration->DUCO_USER.c_str(), 30);
                    WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", configuration->MINER_KEY.c_str(), 30);
                    WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", configuration->RIG_IDENTIFIER.c_str(), 30);

                    wm.setDarkMode(true); // 設定是否為黑底模式
                    wm.setDebugOutput(wiFiManagerDebugOutput);  // 輸出除錯訊息
                    wm.setBreakAfterConfig(true);
                    //wm.resetSettings();
                    //wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
                    //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
                    wm.setScanDispPerc(true); // 是否百分比顯示
                    wm.setMinimumSignalQuality(40); // 設定最低訊號強度百分比(數字)，預設為8
                    //set config save notify callback
                    wm.setSaveConfigCallback(saveConfigCallback);

                    //add all your parameters here
                    wm.addParameter(&custom_account);
                    wm.addParameter(&custom_minigkey);
                    wm.addParameter(&custom_rigname);
                    wm.setCleanConnect(true);
                    wm.setConfigPortalBlocking(true); // 設定啟用「設置入口」時，是否擱置其他連線請求，傳入false代表不擱置
                    wm.startConfigPortal(loopAPName); // 啟用Wi-Fi AP

                    if (showDebugMessage)
                        Serial.println("shouldSaveConfig is: " + String(shouldSaveConfig));

                    //save the custom parameters to FS
                    if (shouldSaveConfig) {

                        WriteDataToFS(setupAPName, custom_account.getValue(), custom_minigkey.getValue(), custom_rigname.getValue());

                        WiFi.mode(WIFI_STA); // Setup ESP in client mode
                        delay(1000);
#if defined(ESP8266)
                        WiFi.setSleepMode(WIFI_NONE_SLEEP);
#else
                        WiFi.setSleep(false);
#endif
                        WiFi.begin(wm.getWiFiSSID(), wm.getWiFiPass());

                        int wait_passes = 0;
                        while (WiFi.waitForConnectResult() != WL_CONNECTED) {
                            if (wait_passes == 0) {
                                Serial.print("connecting ");
                            }
                            else {
                                Serial.print(".");
                            }
                            wait_passes++;
                            delay(500);

                            if (wait_passes / 2 > setConnectTime) {
                                wm.resetSettings();
                                SecondToRestart();
                            }
                        }
                    }
                    if (showDebugMessage && WiFi.waitForConnectResult() == WL_CONNECTED) {
                        Serial.println("connected to " + wm.getWiFiSSID() + " success!!");

                        ShowConfigData(wm.getWiFiIsSaved(), wm.getWiFiSSID(), wm.getWiFiPass());
                    }
                }
            }
        }
    }
}
