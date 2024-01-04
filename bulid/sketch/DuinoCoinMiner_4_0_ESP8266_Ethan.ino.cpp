#line 1 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
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

#line 76 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
String readFile(const char* path);
#line 92 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
void writeFile(const char* path, const char* message);
#line 132 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
void saveConfigCallback();
#line 139 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
void setup();
#line 208 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
void loop();
#line 76 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
String readFile(const char* path) {
    Serial.printf("Reading file: %s\r\n", path);
    File file = SPIFFS.open(path, "r");
    if (!file || file.isDirectory()) {
        Serial.println("- empty file or failed to open file");
        return String();
    }
    Serial.println("- read from file:");
    String fileContent;
    while (file.available()) {
        fileContent += String((char)file.read());
    }
    file.close();
    Serial.println(fileContent);
    return fileContent;
}
void writeFile(const char* path, const char* message) {
    Serial.printf("Writing file: %s\r\n", path);
    File file = SPIFFS.open(path, "w");
    if (!file) {
        Serial.println("- failed to open file for writing");
        return;
    }
    if (file.print(message)) {
        Serial.println("- file written");
    }
    else {
        Serial.println("- write failed");
    }
    delay(100);  // Make sure the CREATE and LASTWRITE times are different
    file.close();
}

char SSIDforRun[] = "";
char PASSWORDforRun[] = "";
char account[] = "";
char minerKey[] = "";
char rigName[] = "";

#include <WiFiManager.h>
#define TRIGGER_PIN 0
WiFiManager wm;
int timeout = 120; // seconds to run for

// The extra parameters to be configured (can be either global or just in the setup)
// After connecting, parameter.getValue() will get you the configured value
// id/name placeholder/prompt default length
WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", "919ethan", 30);
WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", "Qq681417", 30);
WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", "ESP8266_", 30);


//flag for saving data
bool shouldSaveConfig = false;

//callback notifying us of the need to save config
void saveConfigCallback() {
    Serial.println("Should save config");
    shouldSaveConfig = true;
}



void setup() {
    Serial.begin(921600);
    //Serial.begin(115200);
    if (!SPIFFS.begin()) { //to start littlefs
        Serial.println("SPIFFS mount failed");
        return;
    }
    else {
        Serial.println("SPIFFS Mounted Successfully");
    }

    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    // put your setup code here, to run once:
    pinMode(TRIGGER_PIN, INPUT_PULLUP);
    //wm.resetSettings();
    wm.setConnectTimeout(20); // how long to try to connect for before continuing
    wm.setConfigPortalTimeout(30); // auto close configportal after n seconds

    //set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    //add all your parameters here
    wm.addParameter(&custom_account);
    wm.addParameter(&custom_minigkey);
    wm.addParameter(&custom_rigname);

    bool res;
    res = wm.autoConnect("DuinoCoin-AP");
    if (!res) {
        Serial.println("Failed to connect");
        // ESP.restart();
    }

    strcpy(SSIDforRun, wm.getWiFiSSID().c_str());
    writeFile("/SSIDforRun.txt", SSIDforRun);

    Serial.print("connected to ");
    Serial.print(SSIDforRun);
    Serial.println(" success!!");


    strcpy(SSIDforRun, readFile("/SSIDforRun.txt").c_str());

    strcpy(PASSWORDforRun, wm.getWiFiPass().c_str());
    writeFile("/PASSWORDforRun.txt", PASSWORDforRun);

    Serial.println("\tPASSWORDforRun : " + readFile("/PASSWORDforRun.txt"));


    //read updated parameters
    strcpy(account, custom_account.getValue());
    writeFile("/account.txt", account);

    strcpy(minerKey, custom_minigkey.getValue());
    writeFile("/minerKey.txt", minerKey);

    strcpy(rigName, custom_rigname.getValue());
    writeFile("/rigName.txt", rigName);


    Serial.println("The values in the file are: ");
    Serial.println("DUCO_USER : " + readFile("/account.txt"));
    Serial.println("MINER_KEY : " + readFile("/minerKey.txt"));
    Serial.println("RIG_IDENTIFIER : " + readFile("/rigName.txt"));


    SPIFFS.end();
}

void loop() {
    if (digitalRead(TRIGGER_PIN) == LOW) {
        delay(100);
        if (digitalRead(TRIGGER_PIN) == LOW) {
            delay(2000);
            if (digitalRead(TRIGGER_PIN) == LOW) {
                wm.resetSettings();
                wm.setConnectTimeout(20); // how long to try to connect for before continuing
                wm.setConfigPortalTimeout(30); // auto close configportal after n seconds
                if (!wm.startConfigPortal("DuinoCoin-AP")) {
                    Serial.println("failed to connect and hit timeout");
                    delay(3000);
                    ESP.restart();
                    delay(5000);
                }
                Serial.print("connected to ");
                Serial.print(wm.getWiFiSSID());
                Serial.println(" success!!");
            }
        }
    }
}

