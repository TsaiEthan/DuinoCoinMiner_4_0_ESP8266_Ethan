#ifndef MYWIFIMANAGER_H
#define MYWIFIMANAGER_H

/*
 

DuinoCoin新版官方程式修改說明
1.將 myWiFiManager.h 、myWiFiManager.cpp 兩個檔案加到 官方.ino檔的資料夾內
2.打開.ino檔編輯
3.在最上面加入 #include "myWiFiManager.h"
4.在setup()函式中，把 SetupWifi() ，更換成 setupWiFiManager()
5.在loop()函式中的第一行，加入 doWiFiManager();
6.編輯檔案:Settings.h，更改下列10行
                        原式                                                           變更為
    extern const char DUCO_USER[] = "my_cool_username";         |           extern const char DUCO_USER[];    
    extern const char MINER_KEY[] = "mySecretPass";             |           extern const char MINER_KEY[];
    extern const char SSID[] = "SSID";                          |           extern const char SSID[];
    extern const char PASSWORD[] = "PASSW0RD";                  |           extern const char PASSWORD[];
    extern const char RIG_IDENTIFIER[] = "None";                |           extern const char RIG_IDENTIFIER[];

    extern unsigned int hashrate = 0;                           |           extern unsigned int hashrate;
    extern unsigned int difficulty = 0;                         |           extern unsigned int difficulty;
    extern unsigned long share_count = 0;                       |           extern unsigned long share_count;
    extern String node_id = "";                                 |           extern String node_id;
    extern String WALLET_ID = "";                               |           extern String WALLET_ID;
7.完成以上步驟，接上ESP8266開發版，編譯上傳程式
8.上傳完成後，WiFi連接 DuinoCoin-AP ，免密碼
9.若未自動進入設定畫面，可用瀏覽器輸入 192.168.4.1 ，進入設定畫面
10.設定所有欄位後，儲存離開，即開始自動挖礦

★★★ 注意：WiFiManger中文化及加掛Logo圖片，是透過修改 C:\Users\yamah\Documents\Arduino\libraries\WiFiManager\ 資料夾內 WiFiManager.cpp 及 wm_strings_en.h 達成 ★★★

★★ .ino檔內的 SelectNode() 函式，數字32可改小，縮短重新連線時間 ★★

*/


#include"MiningJob.h"
#include <Arduino.h>
#include <FS.h>

#include <WiFiManager.h>
#define TRIGGER_PIN 0

//#pragma once
  
//My Parameter Add Here
extern bool showDebugMessage;
extern bool wiFiManagerDebugOutput;
extern char setupAPName[30];
extern char loopAPName[30];
extern unsigned int setConnectTime;
extern bool shouldSaveConfig;

#ifdef __cplusplus
extern "C" {
#endif

    namespace {
        //DuinoCoin Offical Parameter Add Here
        extern MiningConfig* configuration;

        //DuinoCoin Offical Functions Add Here
        void UpdateHostPort(String input);
        String httpGetString(String URL);
        void SelectNode();
        void SetupOTA();
        void VerifyWifi();
        void handleSystemEvents(void);


        //My Functions Add Here
        String readFile(const char* path);
        void writeFile(const char* path, const char* message);
        void saveConfigCallback();
        void ReadFSData();
        void WriteDataToFS(String whoCallTheFunction, String writeAccountData, String writeMinerKeyData, String writeRigNameData);
        void ShowConfigData(bool WiFiIsSaved, String WiFiSSID, String WiFiPWD);
        void SecondToRestart();
        void setupWiFiManager();
        void doWiFiManager();
    }


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
extern MiningJob* job[CORE];
void task1_func(void*);
void task2_func(void*);
void setup();
void loopOneCore();
void loop();

#ifdef __cplusplus
}
#endif

#endif  // End of MYWIFIMANAGER_H
