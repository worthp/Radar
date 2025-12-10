#ifndef __WEB_H
#define __WEB_H

#include <Arduino.h>
#include <kld7.h>

class Web {
public:
    Web(){};
    ~Web() {};
    
    String homePage(KLD7 radar);
    String radarSettingsPage(KLD7 radar, char msg[]);
    void updateRadarParameters(KLD7 radar);
    String statsPage(KLD7 radar);
    String logPage(KLD7 radar);

    int lastRefreshInterval = -1;
    String formattedHtmlHead;
    String htmlHeadFormat = String("<!DOCTYPE html>\n<html><head><meta http-equiv='refresh' content='%d'>\n<style> html, body { margin: 0; padding: 0; min-height:200vh; box-sizing: border-box; } th, td { border: 1px solid rgb(160 160 160); padding: 8px 10px; } th[scope='col'] { background-color: #505050; color: white; } th[scope='row'] { background-color: #d6ecd4; } td { text-align: center; } tr:nth-of-type(even) { background-color: #eeeeee; } table { width: 100%; border-collapse: collapse; border: 2px solid rgb(140 140 140); font-family: sans-serif; } caption { caption-side: bottom; padding: 10px; } </style></head>");
    String htmlHead(int refreshInterval);
private:
    String menu();
    String _statPage = "";

};

#endif