#include <WiFiManager.h> // https://github.com/tzapu/WiFiManager
#include <ESP8266mDNS.h>
#include <ESP8266WebServer.h>
#include <Esp.h>
#include <uri/UriBraces.h>

#include <kld7.h>
#include <web.h>

ESP8266WebServer server(80);

void handleRoot();
void handleRadarSettings();
void handleUpdateRadarSettings();
void handleStats();
void handleLogs();
void handleReboot();

KLD7 radar;
Web web;

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
        server.on("/radarsettings/", handleRadarSettings);
        server.on(UriBraces("/updateradarsettings/{}/{}"), handleUpdateRadarSettings);
        server.on("/stats/", handleStats);
        server.on("/logs/", handleLogs);
        server.on("/reboot/", handleReboot);
    }
    
    Serial.println("wifi/http setup() complete");

    // use alternate rx/tx for KLD7
    Serial.flush();
    Serial.swap();
    
    // tell radar which serial port to use
    radar.setSerialConnection(&Serial);
    if (radar.init() != KLD7::OK) {
        Serial.flush();
        Serial.swap();
        Serial.println("KLD7 failed to init()");
    }
}

void handleRoot() {
    server.send(200, "text/html", web.homePage(radar));
}
void handleReboot() {
    ESP.restart();
    server.send(200, "text/html", web.logPage(radar));
}

void handleRadarSettings() {
    char msg[] {0};
    server.send(200, "text/html", web.radarSettingsPage(radar, msg));
}

void handleUpdateRadarSettings() {
    char b[128];
    // this needs to be a lot smarter!!!!
    //radar.setRadarParameters();
    String var = server.pathArg(0);
    String val = server.pathArg(1);
    KLD7::RESPONSE r;

    if ((r = radar.updateRadarParameter(var, val)) != KLD7::OK) {
        sprintf(b, "Failed to update [%s] to val[%s] [%s]\n", var.c_str(), val.c_str(), radar.responseText[r].c_str());
    }

    server.send(200, "text/html", web.radarSettingsPage(radar, b));
}

void handleStats() {
    server.send(200, "text/html", web.statsPage(radar));
}

void handleLogs() {
    server.send(200, "text/html", web.logPage(radar));
}

void loop() {
    delay(500);
    if (radar.inited) {
        radar.getNextFrameData();
    } else {
        Serial.println("KLD7 is not initialized.");
    }

    MDNS.update();
    server.handleClient();
}