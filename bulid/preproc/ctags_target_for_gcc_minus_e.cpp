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
    delay(100); // Make sure the CREATE and LASTWRITE times are different
    file.close();
}

char SSIDforRun[] = "";
char PASSWORDforRun[] = "";
char account[] = "";
char minerKey[] = "";
char rigName[] = "";

# 116 "C:\\Users\\yamah\\Documents\\Arduino\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan\\DuinoCoinMiner_4_0_ESP8266_Ethan.ino" 2

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
    pinMode(0, 0x02);
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
    if (digitalRead(0) == 0x0) {
        delay(100);
        if (digitalRead(0) == 0x0) {
            delay(2000);
            if (digitalRead(0) == 0x0) {
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
