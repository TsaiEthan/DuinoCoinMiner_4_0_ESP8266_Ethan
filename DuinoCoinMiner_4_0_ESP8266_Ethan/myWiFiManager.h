#ifndef MYWIFIMANAGER_H
#define MYWIFIMANAGER_H

/*
 

DuinoCoin�s���x��{���קﻡ��
1.�N myWiFiManager.h �BmyWiFiManager.cpp ����ɮץ[�� �x��.ino�ɪ���Ƨ���
2.���}.ino�ɽs��
3.�b�̤W���[�J #include "myWiFiManager.h"
4.�bsetup()�禡���A�� SetupWifi() �A�󴫦� setupWiFiManager()
5.�bloop()�禡�����Ĥ@��A�[�J doWiFiManager();
6.�s���ɮ�:Settings.h�A���U�C10��
                        �즡                                                           �ܧ�
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
7.�����H�W�B�J�A���WESP8266�}�o���A�sĶ�W�ǵ{��
8.�W�ǧ�����AWiFi�s�� DuinoCoin-AP �A�K�K�X
9.�Y���۰ʶi�J�]�w�e���A�i���s������J 192.168.4.1 �A�i�J�]�w�e��
10.�]�w�Ҧ�����A�x�s���}�A�Y�}�l�۰ʫ��q

������ �`�N�GWiFiManger����ƤΥ[��Logo�Ϥ��A�O�z�L�ק� C:\Users\yamah\Documents\Arduino\libraries\WiFiManager\ ��Ƨ��� WiFiManager.cpp �� wm_strings_en.h �F�� ������

���� .ino�ɤ��� SelectNode() �禡�A�Ʀr32�i��p�A�Y�u���s�s�u�ɶ� ����

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
