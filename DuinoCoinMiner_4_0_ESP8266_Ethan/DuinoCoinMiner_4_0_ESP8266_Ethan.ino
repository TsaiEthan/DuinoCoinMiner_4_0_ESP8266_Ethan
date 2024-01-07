/*
   ____  __  __  ____  _  _  _____       ___  _____  ____  _  _
  (  _ \(  )(  )(_  _)( \( )(  _  )___  / __)(  _  )(_  _)( \( )
   )(_) ))(__)(  _)(_  )  (  )(_)((___)( (__  )(_)(  _)(_  )  (
  (____/(______)(____)(_)\_)(_____)     \___)(_____)(____)(_)\_)
  Official code for all ESP8266/32 boards            version 4.0
  Main .ino file

  The Duino-Coin Team & Community 2019-2024 © MIT Licensed
  https://duinocoin.com
  https://github.com/revoxhere/duino-coin

  If you don't know where to start, visit official website and navigate to
  the Getting Started page. Have fun mining!

  To edit the variables (username, WiFi settings, etc.) use the Settings.h tab!
*/

/* If optimizations cause problems, change them to -O0 (the default) */
#pragma GCC optimize("-Ofast")

/* If during compilation the line below causes a
  "fatal error: arduinoJson.h: No such file or directory"
  message to occur; it means that you do NOT have the
  ArduinoJSON library installed. To install it,
  go to the below link and follow the instructions:
  https://github.com/revoxhere/duino-coin/issues/832 */
#include <ArduinoJson.h>

#if defined(ESP8266)
#include <ESP8266WiFi.h>
#include <ESP8266mDNS.h>
#include <ESP8266HTTPClient.h>
#include <ESP8266WebServer.h>
#else
#include <ESPmDNS.h>
#include <WiFi.h>
#include <HTTPClient.h>
#include <WebServer.h>
#endif

#include <WiFiUdp.h>
#include <ArduinoOTA.h>
#include <WiFiClient.h>
#include <Ticker.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "MiningJob.h"
#include "Settings.h"

#if defined(WEB_DASHBOARD)
#include "Dashboard.h"
#endif

  // Auto adjust physical core count
  // (ESP32-S2/C3 have 1 core, ESP32 has 2 cores, ESP8266 has 1 core)
#if defined(ESP8266)
#define CORE 1
typedef ESP8266WebServer WebServer;
#elif defined(CONFIG_FREERTOS_UNICORE)
#define CORE 1
#else
#define CORE 2
// Install TridentTD_EasyFreeRTOS32 if you get an error
#include <TridentTD_EasyFreeRTOS32.h>
#endif

#if defined(WEB_DASHBOARD)
WebServer server(80);
#endif


#include <Arduino.h>
#include <FS.h>

//char account[30];
//char minerKey[30];
//char rigName[30];

#include <WiFiManager.h>
#define TRIGGER_PIN 0

bool showDebugMessage = false;
bool wiFiManagerDebugOutput = true;
char setupAPName[30] = "DuinoCoin-AP";
char loopAPName[30] = "DuinoCoin-AP";
unsigned int setConnectTime = 20;
bool shouldSaveConfig = false;

namespace {
    
    MiningConfig* configuration = new MiningConfig(
        DUCO_USER,
        RIG_IDENTIFIER,
        MINER_KEY
    );
    
#if defined(ESP32) && CORE == 2
    EasyMutex mutexClientData, mutexConnectToServer;
#endif

    void UpdateHostPort(String input) {
        // Thanks @ricaun for the code
        DynamicJsonDocument doc(256);
        deserializeJson(doc, input);
        const char* name = doc["name"];

        configuration->host = doc["ip"].as<String>().c_str();
        configuration->port = doc["port"].as<int>();
        node_id = String(name);

        Serial.println("Poolpicker selected the best mining node: " + node_id);
    }

    String httpGetString(String URL) {
        String payload = "";
        WiFiClientSecure client;
        client.setInsecure();
        HTTPClient http;

        if (http.begin(client, URL)) {
            int httpCode = http.GET();

            if (httpCode == HTTP_CODE_OK)
                payload = http.getString();
            else
                Serial.printf("Error fetching node from poolpicker: %s\n", http.errorToString(httpCode).c_str());

            http.end();
        }
        return payload;
    }

    void SelectNode() {
        String input = "";
        int waitTime = 1;
        int poolIndex = 0;

        while (input == "") {
            Serial.println("Fetching mining node from the poolpicker in " + String(waitTime) + "s");
            input = httpGetString("https://server.duinocoin.com/getPool");

            delay(waitTime * 1000);
            // Increase wait time till a maximum of 8 seconds
            // (addresses: Limit connection requests on failure in ESP boards #1041)
            waitTime *= 2;
            if (waitTime > 8)
                waitTime = 8;
        }

        UpdateHostPort(input);
    }

    void SetupOTA() {
        // Prepare OTA handler
        ArduinoOTA.onStart([]()
            { Serial.println("Start"); });
        ArduinoOTA.onEnd([]()
            { Serial.println("\nEnd"); });
        ArduinoOTA.onProgress([](unsigned int progress, unsigned int total)
            { Serial.printf("Progress: %u%%\r", (progress / (total / 100))); });
        ArduinoOTA.onError([](ota_error_t error)
            {
                Serial.printf("Error[%u]: ", error);
                if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
                else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
                else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
                else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
                else if (error == OTA_END_ERROR) Serial.println("End Failed"); });

        ArduinoOTA.setHostname(RIG_IDENTIFIER); // Give port a name
        ArduinoOTA.begin();
    }

    void VerifyWifi() {
        while (WiFi.status() != WL_CONNECTED || WiFi.localIP() == IPAddress(0, 0, 0, 0))
            WiFi.reconnect();
    }

    void handleSystemEvents(void) {
        VerifyWifi();
        ArduinoOTA.handle();
        yield();
    }

#if defined(WEB_DASHBOARD)
    void dashboard() {
        Serial.println("Handling HTTP client");
        String s = WEBSITE;
        s.replace("@@IP_ADDR@@", WiFi.localIP().toString());

        s.replace("@@HASHRATE@@", String(hashrate / 1000));
        s.replace("@@DIFF@@", String(difficulty / 100));
        s.replace("@@SHARES@@", String(share_count));
        s.replace("@@NODE@@", String(node_id));

#if defined(ESP8266)
        s.replace("@@DEVICE@@", "ESP8266");
#elif defined(CONFIG_FREERTOS_UNICORE)
        s.replace("@@DEVICE@@", "ESP32-S2/C3");
#else
        s.replace("@@DEVICE@@", "ESP32");
#endif

        s.replace("@@ID@@", String(RIG_IDENTIFIER));
        s.replace("@@MEMORY@@", String(ESP.getFreeHeap()));
        s.replace("@@VERSION@@", String(SOFTWARE_VERSION));
        server.send(200, "text/html", s);
    }
#endif


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

    void WriteDataToFS(String whoCallTheFunction, String writeAccountData, String writeMinerKeyData, String writeSSIDData, String writePWDData, String writeRigNameData) {


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
        wm.setDarkMode(true); //設定是否為黑底模式
        wm.setDebugOutput(wiFiManagerDebugOutput);  // 輸出除錯訊息
        wm.setBreakAfterConfig(true);
        //wm.resetSettings();
        //wm.setTimeout(12);
        //wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
        //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
        wm.setScanDispPerc(true); //是否百分比顯示
        wm.setMinimumSignalQuality(40); //設定最低訊號強度百分比(數字)，預設為8


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
            WriteDataToFS(setupAPName, custom_account.getValue(), custom_minigkey.getValue(), wm.getWiFiSSID(), wm.getWiFiPass(), custom_rigname.getValue());
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
        // 若啟用「設置入口」的接腳被按下1.5秒
        if (digitalRead(TRIGGER_PIN) == LOW) {
            delay(50);
            if (digitalRead(TRIGGER_PIN) == LOW) {
                delay(1500);
                if (digitalRead(TRIGGER_PIN) == LOW) {
                    shouldSaveConfig = false;
                    Serial.println("按鈕被按下1.5秒了，啟動設置入口。");
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

                    wm.setDarkMode(true); //設定是否為黑底模式
                    wm.setDebugOutput(wiFiManagerDebugOutput);  // 輸出除錯訊息
                    wm.setBreakAfterConfig(true);
                    //wm.resetSettings();
                    //wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
                    //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
                    wm.setScanDispPerc(true); //是否百分比顯示
                    wm.setMinimumSignalQuality(40); //設定最低訊號強度百分比(數字)，預設為8
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

                        WriteDataToFS(setupAPName, custom_account.getValue(), custom_minigkey.getValue(), wm.getWiFiSSID(), wm.getWiFiPass(), custom_rigname.getValue());

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

} // End of namespace

MiningJob* job[CORE];

#if CORE == 2
EasyFreeRTOS32 task1, task2;
#endif

void task1_func(void*) {
#if defined(ESP32) && CORE == 2
    VOID SETUP() { }

    VOID LOOP() {
        job[0]->mine();
    }
#endif
}

void task2_func(void*) {
#if defined(ESP32) && CORE == 2
    VOID SETUP() {
        job[1] = new MiningJob(1, configuration);
    }

    VOID LOOP() {
        job[1]->mine();
    }
#endif
}




// put your setup code here, to run once:
void setup() {
    
    delay(500);

    Serial.begin(921600);
    Serial.println("\n\nDuino-Coin " + String(configuration->MINER_VER));
    pinMode(LED_BUILTIN, OUTPUT);

    assert(CORE == 1 || CORE == 2);
    WALLET_ID = String(random(0, 2811)); // Needed for miner grouping in the wallet
    job[0] = new MiningJob(0, configuration);

#if defined(USE_DHT)
    Serial.println("Initializing DHT sensor (Duino IoT)");
    dht.begin();
    Serial.println("Test reading: " + String(dht.readHumidity()) + "% humidity");
    Serial.println("Test reading: temperature " + String(dht.readTemperature()) + "°C");
#endif

#if defined(USE_DS18B20)
    Serial.println("Initializing DS18B20 sensor (Duino IoT)");
    sensors.begin();
    sensors.requestTemperatures();
    Serial.println("Test reading: " + String(sensors.getTempCByIndex(0)) + "°C");
#endif

#if defined(USE_INTERNAL_SENSOR)
    Serial.println("Initializing internal ESP32 temperature sensor (Duino IoT)");
    temp_sensor_config_t temp_sensor = TSENS_CONFIG_DEFAULT();
    temp_sensor.dac_offset = TSENS_DAC_L2;
    temp_sensor_set_config(temp_sensor);
    temp_sensor_start();
    float result = 0;
    temp_sensor_read_celsius(&result);
    Serial.println("Test reading: " + String(result) + "°C");
#endif

    setupWiFiManager();

    SetupOTA();

#if defined(WEB_DASHBOARD)
    if (!MDNS.begin(RIG_IDENTIFIER)) {
        Serial.println("mDNS unavailable");
    }
    MDNS.addService("http", "tcp", 80);
    Serial.println("Configured mDNS for dashboard on http://" + String(RIG_IDENTIFIER)
        + ".local (or http://" + WiFi.localIP().toString() + ")");
    server.on("/", dashboard);
    server.begin();
#endif

    job[0]->blink(BLINK_SETUP_COMPLETE);

#if CORE == 2 && defined(ESP32)
    task1.start(task1_func);
    task2.start(task2_func);
#endif
}

void loopOneCore() {
    job[0]->mine();

#if defined(ESP8266)
    // Fastest clock mode for 8266s
    system_update_cpu_freq(160);
#else
    // Fastest clock mode for 32s
    setCpuFrequencyMhz(240);
#endif

    VerifyWifi();
    ArduinoOTA.handle();
#if defined(WEB_DASHBOARD) 
    server.handleClient();
#endif
}

void loop() {
    doWiFiManager();

#if CORE == 1
    loopOneCore();
#endif

}
