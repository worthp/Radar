#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

ESP8266WebServer server(80);

void handleRoot();

void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    // 
    Serial.begin(115200);
    //Serial1.begin(115200);
    
    //WiFiManager, Local intialization. Once its business is done, there is no need to keep it around
    WiFiManager wm;

    // reset settings - wipe stored credentials for testing
    // these are stored by the esp library
    //wm.resetSettings();

    // Automatically connect using saved credentials,

    bool res;
    res = wm.autoConnect("AutoConnectAP"); // anonymous ap

    if(!res) {
        Serial.println("Failed to connect");
    } 
    else {
        //if you get here you have connected to the WiFi    
        Serial.println("connected...yeey :)");
        
        if ((MDNS.begin("myesp"))) {
            Serial.println("mDNS started for myesp");
        } else {
            Serial.println("mDNS failed");
        }

        server.begin(); // start http server
        server.on("/", handleRoot);

        Serial.swap();
    }
}

void handleRoot() {
    String m ;
    m.concat("you got here! [");
    m.concat(millis());
    m.concat("]");

    server.send(200, "text/plain", m);
}

bool go = true;
void loop() {
//    delay(20);
    Serial.print("can you see me now????");
    MDNS.update();
    if (go) {
        server.handleClient();
        //go = false;
    }
}