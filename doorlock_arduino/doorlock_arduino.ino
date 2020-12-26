// Ethernet - Version: Latest
#include <SPI.h>
#include <Ethernet.h>

// LiquidCrystal - Version: Latest
#include <LiquidCrystal.h>

#define PIN_A4   (18)
#define PIN_A5   (19)

int relayRFID = 6; //INPUT
int doorLock = 7; //OUTPUT
int lockdownKey = 8; //INPUT
int buzzer = 9; //OUTPUT

//Emergency System Part
int emergencySwitch = PIN_A4;
int emergencyButton = PIN_A5;

boolean  = false;
boolean emergencyStatus = false;

String myStr = "";
String keyStr = "";

// -- Networking -- // (Set IPs as your convenience)
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };
IPAddress ip(192, 168, 1, 1);

// -- Ethernet Server & Client //
EthernetServer server(80);
EthernetClient remote;

// -- RestAPI Client -- // (Set IPs as your convenience)
IPAddress restapi(192, 168, 1, 1);
String restapiIP = "192.168.1.1";

// -- on Arduino booting -- //
void setup() {
  //INPUT pins declaration
  pinMode(relayRFID, INPUT_PULLUP);
  pinMode(lockdownKey, INPUT_PULLUP);
  pinMode(emergencySwitch, INPUT_PULLUP);
  pinMode(emergencyButton, INPUT_PULLUP);

  //OUTPUT pins declaration
  pinMode(doorLock, OUTPUT);
  pinMode(buzzer, OUTPUT);

  //LED Builtin declaration
  pinMode(LED_BUILTIN, OUTPUT);

  //Ethernet & WebServer initialization
  Ethernet.begin(mac, ip);
  server.begin();

  //Serial initialization
  Serial.begin(9600);
  Serial.println("Initialization completed.");

  //Buzzer says everything ready to go !
  tone(buzzer, 2093, 150);
  delay(300);
  tone(buzzer, 2093, 150);
  delay(150);
}

//Wifi and Web server checks / RFID & Lockdown (Switch with a key to prevent unlocking) & Emergency switches handler
void loop() {
  checkForClient();

  int rfidState = digitalRead(relayRFID);

  int lockdownState = digitalRead(lockdownKey);

  int emergencySwitchState = digitalRead(emergencySwitch);
  int emergencyButtonState = digitalRead(emergencyButton);

  if (emergencyButtonState == HIGH && !emergencyStatus) {
    delay(750);
    if(emergencyButtonState == LOW)
      return;

    emergencyStatus = true;
  }

  if (emergencyStatus) {
    digitalWrite(doorLock, HIGH);
    delay(50);
    digitalWrite(doorLock, LOW);

    tone(buzzer, 400, 200);
    delay(500);
    tone(buzzer, 400, 200);
    delay(500);

    delay(300);

    tone(buzzer, 400, 500);
    delay(250);
    tone(buzzer, 400, 500);
    delay(250);

    tone(buzzer, 400, 200);
    delay(500);
    tone(buzzer, 400, 200);
    delay(500);

    if(emergencySwitchState == LOW && emergencyButtonState == LOW){
      emergencyStatus = false;
    }
  }

  else {
    emergencyStatus = false;

    if (lockdownState == LOW) {

      if (rfidState == LOW) {
        tone(buzzer, 370, 50);
        delay(100);
        tone(buzzer, 370, 300);
        delay(1000);
      }
    }

    else {
      if (rfidState == LOW)
        handleAccessGranted(true);
    }
  }
}

// Handle an granted access and sends a request to web server to display a welcome message on the LCD (ESP8266)
void handleAccessGranted(boolean local) {
  digitalWrite(doorLock, HIGH);
  delay(50);
  digitalWrite(doorLock, LOW);

  digitalWrite(LED_BUILTIN, HIGH);

  if (local)
    // SET API_TOKEN as the same of the Java APP
    HTTPRequest("POST /api/arduino/handleAccessGranted?key=[API_TOKEN] HTTP/1.1", "Host: " + restapiIP, "Connection: close");

  tone(buzzer, 523, 50);
  delay(50);
  tone(buzzer, 783, 50);
  delay(50);
  tone(buzzer, 1046, 50);
  delay(50);
  tone(buzzer, 1568, 50);
  delay(50);
  tone(buzzer, 2093, 70);
  delay(250);

  delay(2000);
  digitalWrite(LED_BUILTIN, LOW);
}

// Handle incoming http requests and returns some beautiful HTML content
void checkForClient() {

  EthernetClient client = server.available();

  if (client) {
    // an http request ends with a blank line
    boolean currentLineIsBlank = true;
    boolean sentHeader = false;
    boolean reading;
    myStr = "";
    keyStr = "";
    while (client.connected()) {
      if (client.available()) {

        char c = client.read();

        if (reading && c == ' ') reading = false;
        if (c == '?') reading = true; //found the ?, begin reading the info

        if (reading) {
          Serial.print(c);
          if (c != '?') {
            myStr += c;
          }
        }

        if (c == '\n' && currentLineIsBlank)  break;

        if (c == '\n') {
          currentLineIsBlank = true;
        } else if (c != '\r') {
          currentLineIsBlank = false;
        }
      }
    }
    parseThangs(myStr);
    client.println("HTTP/1.1 200 OK");
    client.println("Content-Type: text/html");
    client.println("Connection: close");  // the connection will be closed after completion of the response
    client.println();
    client.println("<!DOCTYPE HTML>");
    client.println("<html>");
    // SET UNLOCK_TOKEN as the same of the Java APP
    if (keyStr == "[UNLOCK_TOKEN]") {
      if (emergencyStatus) {
        client.println("<head>");
        client.println("<title>Arduino RFID Controller</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("Password OK");
        client.println("<h1>Emergency Access is activated</h1>");
        client.println("<h2>This password is not authorised for disabling the emergency system.</h2>");
        client.println("</body>");
        client.println("</html>");
        delay(100); // give the web browser time to receive the data
        client.stop(); // close the connection:
      } else {
        client.println("Password OK, access granted !");
        client.println("<head>");
        client.println("<title>Arduino RFID Controller</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("</body>");
        client.println("</html>");
        delay(100); // give the web browser time to receive the data
        client.stop(); // close the connection:
        handleAccessGranted(false);

      }
      // SET EMERGENCY_TOKEN as the same of the Java APP
    } else if (keyStr == "[EMERGENCY_TOKEN]") {
      if (!emergencyStatus) {
        client.println("<head>");
        client.println("<title>Arduino RFID Controller</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("Password OK");
        client.println("<h1>Emergency Access is now activated</h1>");
        client.println("</body>");
        client.println("</html>");
        delay(100); // give the web browser time to receive the data
        client.stop(); // close the connection:
        emergencyStatus = true;
      } else {
        client.println("<head>");
        client.println("<title>Arduino RFID Controller</title>");
        client.println("</head>");
        client.println("<body>");
        client.println("Password OK");
        client.println("<h1>Emergency Access is desactivated</h1>");
        client.println("</body>");
        client.println("</html>");
        delay(100); // give the web browser time to receive the data
        client.stop(); // close the connection:
        emergencyStatus = false;

      }
    }

    else {
      client.println("Bad Password, make sure you entered the right.");
      client.println("</html>");
      delay(100); // give the web browser time to receive the data
      client.stop(); // close the connection:
    }
    keyStr = "";
  }
}

// Used to parse the http request
void parseThangs(String str) {
  int startIndex = str.indexOf("key");
  int endIndex = str.indexOf("#");
  keyStr = str.substring(startIndex + 4, endIndex);
  startIndex = str.indexOf("scenar");
  endIndex = str.indexOf("$");
  String sc = str.substring(startIndex + 7, endIndex);
  Serial.println(sc);

}

// Help to send an HTTP Request
void HTTPRequest(String line1, String line2, String line3) {
  if (remote.connect(restapi, 48274)) {
    Serial.println("connected");
    // Make a HTTP request:
    remote.println(line1);
    //remote.println("Authorization: ");
    remote.println(line2);
    remote.println(line3);
    remote.println();
  } else {
    // if you didn't get a connection to the server:
    Serial.println("connection failed");
  }
  remote.stop();
}
