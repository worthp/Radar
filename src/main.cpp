#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>

#include <kld7.h>

ESP8266WebServer server(80);

void handleRoot();

KLD7 radar;

void setup() {
    // WiFi.mode(WIFI_STA); // explicitly set mode, esp defaults to STA+AP
    // it is a good practice to make sure your code sets wifi mode how you want it.

    // put your setup code here, to run once:
    // 
    Serial.begin(115200, SERIAL_8E1);
    Serial1.begin(115200);
    
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
    }
    
    Serial.println("wifi/http setup() complete");
    Serial.flush(); // just cuz i don't trust anything - even what flush does :(

    // use alternate rx/tx for KLD7
    Serial.swap();
    
    // tell radar which serial port to use
    radar.setSerialConnection(&Serial);
    if (radar.init() != KLD7::OK) {
        Serial.swap();
        Serial.println("radar is ok");
    }
}

void handleRoot() {
    radar.getNextFrameData();

    server.send(200, "text/json", radar.getStatus());
}

void loop() {
    delay(2);
    //radar.getNextFrameData();

    MDNS.update();
    server.handleClient();
}