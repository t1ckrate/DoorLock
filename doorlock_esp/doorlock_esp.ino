#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <ArduinoOTA.h>
#include <Arduino_JSON.h>

#include <Keypad.h>
#include <LiquidCrystal_I2C.h>


static const uint8_t D0   = 16;
static const uint8_t D1   = 5;
static const uint8_t D2   = 4;
static const uint8_t D3   = 0;
static const uint8_t D4   = 2;
static const uint8_t D5   = 14;
static const uint8_t D6   = 12;
static const uint8_t D7   = 13;
static const uint8_t D8   = 15;

const byte ROWS = 3; //four rows
const byte COLS = 3; //three columns

char keys[ROWS][COLS] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'}
};

byte rowPins[ROWS] = {D4, D0, D3}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {D5, D6, D7}; //connect to the column pinouts of the keypad

//Create an object of keypad
Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

//Password Length + 1
#define Password_Length 7

//Password Properties
char Data[Password_Length];
byte data_count = 0, master_count = 0;
bool Pass_is_good;
char customKey;

//Just a simple bool to check if wifi is working
bool wifiIsntWorking = false;

//LCD I2C declarations
LiquidCrystal_I2C lcd(0x27,20,4);

//Wifi Server declarations
ESP8266WiFiMulti WiFiMulti;
ESP8266WebServer server(80);

//Set credentials as your convenience
const char* www_username = "";
const char* www_password = "";

void setup(){
    //Serial initialization
    Serial.begin(9600);

    //LCD initialization
    lcd.init();

    // Turn on the LCD backlight & clearing it
    lcd.backlight();
    lcd.clear();
    clearData();

    // Initialize a WIFI Connection and set WiFi credentials
    WiFi.mode(WIFI_STA);
    WiFiMulti.addAP("SSID", "PASSWORD");   //WiFi connection

    while (WiFiMulti.run() != WL_CONNECTED) {  //Wait for the WiFI connection completion
      delay(500);
      Serial.println("Waiting for connection");
    }

    //OTA (OverTheAir) Initialization
    ArduinoOTA.begin();

    // Start Web Server & Handling WebServer events
    server.on("/displayAccessGranted", []() {
      if (!server.authenticate(www_username, www_password)) {
        return server.requestAuthentication();
      }
      server.send(200, "text/plain", "Login OK");
      accessGranted("");
      delay(5000);
      lcd.clear();
      clearData();
    });
    server.begin();

}

//Wifi and Web server checks / Keypad handler
void loop(){
  if(WiFiMulti.run()== WL_CONNECTED){   //Check WiFi connection status
    if(wifiIsntWorking){
      lcd.clear();
      waitingAuth();
    }

    ArduinoOTA.handle();
    server.handleClient();

    wifiIsntWorking = false;
    customKey = keypad.getKey();
    if (customKey){
      Data[data_count] = customKey;
      lcd.setCursor(data_count+7,3);
      lcd.print("*");
      data_count++;
      }

    if(data_count == Password_Length-1){
      lcd.clear();
      processAuth();

      String password(Data);

      HTTPClient http;
      // SET IP, PORT AND API_TOKEN as the same of the Java APP
      http.begin("http://192.168.1.1:48274/api/esp/requestPassword?key=[API_TOKEN]&password="+password);

      int httpResponseCode = http.POST("");
      Serial.println(httpResponseCode);

      if(httpResponseCode == 200){
        String payload = http.getString();

        JSONVar myObject = JSON.parse(payload);

        // JSON.typeof(jsonVar) can be used to get the type of the var
        if (JSON.typeof(myObject) == "undefined") {
          Serial.println("Parsing input failed!");
          return;
        }
        
        accessGranted((const char*) myObject["name"]);
        
        http.end();
        delay(5000);
      }

      else{
        accessDenied();
        http.end();
        delay(3000);
        }

      lcd.clear();
      clearData();
     }
    }

    else{
      wifiIsntWorking = true;
      errorWifi();
      delay(3000);
    }

}

// Clear LCD display content
void clearData(){
  while(data_count !=0){
    Data[data_count--] = 0;
  }

  waitingAuth();
  return;
}

// Help to display content on the LCD
void handleDisplay(String line1, String line2, String line3, String line4){
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print(line1);
  lcd.setCursor(0,1);
  lcd.print(line2);
  lcd.setCursor(0,2);
  lcd.print(line3);
  lcd.setCursor(0,3);
  lcd.print(line4);
}


//LCD Displays messages
void accessGranted(String customName){
  handleDisplay("- Bureau de Thomas -", "Bienvenue " + customName, "Poussez la porte.", "--------------------");
}

void emergencySystem(String customName){
  handleDisplay("- Bureau de Thomas -", "Emergency toggled !!! ", "Poussez la porte.", "--------------------");
}

void accessDenied(){
  handleDisplay("- Bureau de Thomas -", "Code d'acces invalid", "Merci de reessayer.", "--------------------");
}

void processAuth(){
  handleDisplay("- Bureau de Thomas -", "En cours d'auth.", "Merci de patientez.", "--------------------");
}

void waitingAuth(){
  handleDisplay("- Bureau de Thomas -", "Auth. en attente", "Entrez un code.", "-------______-------");
}

void errorWifi(){
    handleDisplay("- Bureau de Thomas -", "Erreur WIFI.", "Badge uniquement.", "--------------------");
}
