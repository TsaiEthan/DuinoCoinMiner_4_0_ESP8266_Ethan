# 1 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
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
# 19 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
/* If optimizations cause problems, change them to -O0 (the default) */
#pragma GCC optimize("-Ofast")

/* If during compilation the line below causes a

  "fatal error: arduinoJson.h: No such file or directory"

  message to occur; it means that you do NOT have the

  ArduinoJSON library installed. To install it,

  go to the below link and follow the instructions:

  https://github.com/revoxhere/duino-coin/issues/832 */
# 28 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
# 29 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2


# 32 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 33 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 34 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 35 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2







# 43 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 44 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 45 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 46 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2

# 48 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 49 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2

# 51 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 52 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2





  // Auto adjust physical core count
  // (ESP32-S2/C3 have 1 core, ESP32 has 2 cores, ESP8266 has 1 core)


typedef ESP8266WebServer WebServer;
# 73 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
# 74 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2
# 75 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2

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
# 108 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino"
    delay(50); // Make sure the CREATE and LASTWRITE times are different
    file.close();
}

//char SSIDforRun[31];
//char PASSWORDforRun[31];
char account[31];
char minerKey[31];
char rigName[31];

# 119 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2

WiFiManager wm;
unsigned int setConnectTime = 30; // how long to try to connect for before continuing
unsigned int setCloseConfigportalTime = 300; // auto close configportal after n seconds
unsigned int startTime = millis(); // 紀錄設置入口的啟動時間
unsigned int timeout = 120; // 設置入口的逾時秒數
bool portalRunning = false; // 設置入口是否執行中，預設「否」

// The extra parameters to be configured (can be either global or just in the setup)
// After connecting, parameter.getValue() will get you the configured value
// id/name placeholder/prompt default length
WiFiManagerParameter custom_account("account", "DuinoCoin帳號：", "919ethan", 30);
WiFiManagerParameter custom_minigkey("minigkey", "挖礦密碼(非登入密碼)：", "Qq681417", 30);
WiFiManagerParameter custom_rigname("rigname", "挖礦機名稱：", "ESP8266_", 30);

//callback notifying us of the need to save config
void saveConfigCallback() {
    if (!SPIFFS.begin()) { //to start littlefs
        Serial.println("SPIFFS mount failed");
        return;
    }
    else {
        //Serial.println("SPIFFS Mounted Successfully");
    }

    //Serial.println("Should save config");

    //writeFile("/SSIDforRun.txt", wm.getWiFiSSID().c_str());
    //writeFile("/PASSWORDforRun.txt", wm.getWiFiPass().c_str());
    writeFile("/account.txt", custom_account.getValue());
    writeFile("/minerKey.txt", custom_minigkey.getValue());
    writeFile("/rigName.txt", custom_rigname.getValue());
    SPIFFS.end();
}

void doWiFiManager() { // 執行WiFi管理員的工作
  if (portalRunning) { // 若「設置入口」執行中…
    wm.process(); // 處理「設置入口」的用戶端連結請求
    // 如果「設置入口」已啟用超過預設時間（120秒）…
    if ((millis() - startTime) > (timeout * 1000)) {
      Serial.println("「Wi-Fi設置入口」操作逾時…");
      portalRunning = false; // 設成「非執行中」
      wm.stopConfigPortal(); // 停止「設置入口」
     }
  }

  // 若啟用「設置入口」的接腳被按一下
  // 而且portalRunning變數值不為true…
  if (digitalRead(0) == 0x0 && (!portalRunning)) {
    delay(50);
    if (digitalRead(0) == 0x0) {
        delay(1500);
        if (digitalRead(0) == 0x0) {
            Serial.println("按鈕被按下1.5秒了，啟動設置入口。");
            wm.setConfigPortalBlocking(false); // 設成「非擱置」模式
            wm.startConfigPortal("LoopDuinoCoin-AP"); // 啟用Wi-Fi AP

            portalRunning = true; // 設成「設置入口」執行中
            startTime = millis(); // 紀錄目前的時間
        }
    }
  }
}

void ReadFSData() {
    if (!SPIFFS.begin()) { //to start littlefs
        Serial.println("SPIFFS mount failed");
        return;
    }
    else {
        //Serial.println("SPIFFS Mounted Successfully");
    }
    //Serial.println("Should read FS data");

    //strcpy(SSIDforRun, readFile("/SSIDforRun.txt").c_str());
    //strcpy(PASSWORDforRun, readFile("/PASSWORDforRun.txt").c_str());
    strcpy(account, readFile("/account.txt").c_str());
    strcpy(minerKey, readFile("/minerKey.txt").c_str());
    strcpy(rigName, readFile("/rigName.txt").c_str());

    SPIFFS.end();
}

void ShowConfigData() {
    Serial.println("The values in the file are: ");
    if(wm.getWiFiIsSaved()) {
        Serial.println("SSIDforRun : " + wm.getWiFiSSID());
        Serial.println("PASSWORDforRun : " + wm.getWiFiPass());
    }
    Serial.println("DUCO_USER : " + String(account));
    Serial.println("MINER_KEY : " + String(minerKey));
    Serial.println("RIG_IDENTIFIER : " + String(rigName));
}

void setup() {
    WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP  
    Serial.begin(921600);
    //Serial.begin(115200);
    delay(500);

    // put your setup code here, to run once:
    pinMode(0, 0x02);

    wm.setDebugOutput(true); // 輸出除錯訊息
    //wm.resetSettings();
    wm.setConnectTimeout(setConnectTime); // how long to try to connect for before continuing
    //wm.setConfigPortalTimeout(setCloseConfigportalTime); // auto close configportal after n seconds
    wm.setScanDispPerc(true); //是否百分比顯示
    wm.setMinimumSignalQuality(10); //設定最低訊號強度百分比(數字)，預設為8
    //set config save notify callback
    wm.setSaveConfigCallback(saveConfigCallback);

    //add all your parameters here
    wm.addParameter(&custom_account);
    wm.addParameter(&custom_minigkey);
    wm.addParameter(&custom_rigname);

    if (!wm.autoConnect("DuinoCoin-AP")) {
        Serial.println("Failed to connect");
        // ESP.restart();
    }

    ReadFSData();

    Serial.print("connected to ");
    Serial.print(wm.getWiFiSSID());
    Serial.println(" success!!");

    ShowConfigData();
}

void loop() {
    doWiFiManager();
    ReadFSData();

    Serial.print("connected to ");
    Serial.print(wm.getWiFiSSID());
    Serial.println(" success!!");

    ShowConfigData();
}
