#ifndef __WEB_H
#define __WEB_H

#include <Arduino.h>
#include <kld7.h>

class Web {
public:
    Web(){};
    ~Web() {};
    
    String homePage(KLD7 radar);
    String statsPage(KLD7 radar);
    String htmlHead = String("<!DOCTYPE html>\n<head><meta http-equiv='refresh' content='1'>\n<style>html { font-size: large;} ; th, td { border: 1px solid rgb(160 160 160); padding: 8px 10px; } th[scope='col'] { background-color: #505050; color: white; } th[scope='row'] { background-color: #d6ecd4; } td { text-align: center; } tr:nth-of-type(even) { background-color: #eeeeee; } table { border-collapse: collapse; border: 2px solid rgb(140 140 140); font-family: sans-serif; font-size: 0.8rem; letter-spacing: 1px; } caption { caption-side: bottom; padding: 10px; } </style></head>");
private:
    String menu();
    String _statPage = "";

};

#endif