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

String readFile(const char* path) {
    //Serial.printf("Reading file: %s\r\n", path);
    File file = SPIFFS.open(path, "r");
    if (!file || file.isDirectory()) {
        Serial.println("- empty file or failed to open file");
        return String();
    }
    //Serial.print("- read from file:");
    String fileContent;
    while (file.available()) {
        fileContent += String((char)file.read());
    }
    file.close();
    //Serial.println(fileContent);
    return fileContent;
}
void writeFile(const char* path, const char* message) {
    //Serial.printf("Writing file: %s\r\n", path);
    File file = SPIFFS.open(path, "w");
    if (!file) {
        Serial.println("- failed to open file for writing");
        return;
    }
    file.print(message);
    /*
    if (file.print(message)) {
        Serial.println("- file written");
    }
    else {
        Serial.println("- write failed");
    }
    */
    delay(50);  // Make sure the CREATE and LASTWRITE times are different
    file.close();
}

char account[30];
char minerKey[30];
char rigName[30];

#include <WiFiManager.h>
#define TRIGGER_PIN 0
WiFiManager wm;

//callback notifying us of the need to save config
//void saveConfigCallback() {

//}

void ReadFSData() {
    if (!SPIFFS.begin()) { //to start littlefs
        Serial.println("SPIFFS mount failed");
        return;
    }
    else {
        //Serial.println("SPIFFS Mounted Successfully");
    }
    //Serial.println("Should read FS data");

    if (readFile("/account.txt") == "") {
        strcpy(account, "919ethan");
    }
    else {
        strcpy(account, readFile("/account.txt").c_str());
    }

    if (readFile("/minerKey.txt") == "") {
        strcpy(minerKey, "Qq681417");
    }
    else {
        strcpy(minerKey, readFile("/minerKey.txt").c_str());
    }

    if (readFile("/rigName.txt") == "") {
        strcpy(rigName, "ESP8266_");
    }
    else {
        strcpy(rigName, readFile("/rigName.txt").c_str());
    }    

    SPIFFS.end();
}

void ShowConfigData() {
    Serial.println("The values in the file are: ");
    if (wm.getWiFiIsSaved()) {
        Serial.println("SSIDforRun : " + wm.getWiFiSSID());
        Serial.println("PASSWORDforRun : " + wm.getWiFiPass());
    }
    Serial.println("DUCO_USER : " + String(account));
    Serial.println("MINER_KEY : " + String(minerKey));
    Serial.println("RIG_IDENTIFIER : " + String(rigName));
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
    ReadFSData();

    // The extra parameters to be configured (can be either global or just in the setup)
    // After connecting, parameter.getValue() will get you the configured value
    // id/name placeholder/prompt default length
    WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", account, 30);
    WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", minerKey, 30);
    WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", rigName, 30);

    wm.setDarkMode(true); //設定是否為黑底模式
    wm.setDebugOutput(true);  // 輸出除錯訊息
    wm.setBreakAfterConfig(true);
    //wm.resetSettings();
    //wm.setTimeout(12);
    //wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
    //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
    wm.setScanDispPerc(true); //是否百分比顯示
    wm.setMinimumSignalQuality(10); //設定最低訊號強度百分比(數字)，預設為8
    //set config save notify callback
    //wm.setSaveConfigCallback(saveConfigCallback);

    //add all your parameters here
    wm.addParameter(&custom_account);
    wm.addParameter(&custom_minigkey);
    wm.addParameter(&custom_rigname);

    if (!wm.autoConnect("DuinoCoin-AP")) {
        SecondToRestart();
    }

    if (!SPIFFS.begin()) { //to start littlefs
        Serial.println("SPIFFS mount failed");
        return;
    }
    else {
        //Serial.println("SPIFFS Mounted Successfully");
    }
    //Serial.println("Should save config");
    writeFile("/account.txt", custom_account.getValue());
    writeFile("/minerKey.txt", custom_minigkey.getValue());
    writeFile("/rigName.txt", custom_rigname.getValue());
    SPIFFS.end();

    ReadFSData();

    Serial.print("connected to ");
    Serial.print(wm.getWiFiSSID());
    Serial.println(" success!!");

    ShowConfigData();
}

void doWiFiManager() {   // 執行WiFi管理員的工作
  // 若啟用「設置入口」的接腳被按下1.5秒
  if (digitalRead(TRIGGER_PIN) == LOW) {
    delay(50);
    if (digitalRead(TRIGGER_PIN) == LOW) {
        delay(1500);
        if (digitalRead(TRIGGER_PIN) == LOW) {
            Serial.println("按鈕被按下1.5秒了，啟動設置入口。");
            ReadFSData();
            
            // The extra parameters to be configured (can be either global or just in the setup)
            // After connecting, parameter.getValue() will get you the configured value
            // id/name placeholder/prompt default length
            WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", account, 30);
            WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", minerKey, 30);
            WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", rigName, 30);

            wm.setConfigPortalBlocking(true); // 設定啟用「設置入口」時，是否擱置其他連線請求，傳入false代表不擱置
            // 啟用Wi-Fi AP
            wm.startConfigPortal("LoopDuinoCoin-AP");

            if (!SPIFFS.begin()) { //to start littlefs
                Serial.println("SPIFFS mount failed");
                return;
            }
            else {
                //Serial.println("SPIFFS Mounted Successfully");
            }
            //Serial.println("Should save config");
            writeFile("/account.txt", custom_account.getValue());
            writeFile("/minerKey.txt", custom_minigkey.getValue());
            writeFile("/rigName.txt", custom_rigname.getValue());
            SPIFFS.end();

            ReadFSData();

            Serial.print("connected to ");
            Serial.print(wm.getWiFiSSID());
            Serial.println(" success!!");

            ShowConfigData();
        }
    }
  }
  if(!WiFi.isConnected()) {
      SecondToRestart();
  }
}

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    Serial.begin(921600);
    //Serial.begin(115200);
    delay(500);
    
    // put your setup code here, to run once:
    pinMode(TRIGGER_PIN, INPUT_PULLUP);

    setupWiFiManager();
}

void loop() {
    doWiFiManager();
}
