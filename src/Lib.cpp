#include <ESP8266WiFi.h>
#include <string>
#include "Lib.h"
#include "NetworkingConstants.h"

bool Switches[NumberGPIO] = {false};
char Data[NumberGPIO + 1];
String Names[NumberGPIO];
WiFiServer TCPServer(Port);

void GetState()
{
    String Ret = "";
    for (bool i : Switches)
        Ret = Ret + i;
    strcpy(Data, Ret.c_str());
    Data[NumberGPIO] = '\n';
}

void Init()
{
    Serial.begin(BaudRate);

    Serial.println();
    WiFi.begin(SSID, Password);

    Serial.println("Connecting to Wifi");
    while (WiFi.status() != WL_CONNECTED)
    {
        Serial.print(".");
        delay(1000);
    }

    Serial.println("");
    Serial.print("Connected to ");
    Serial.println(SSID);

    Serial.print("IP address: ");
    Serial.println(WiFi.localIP());
    TCPServer.begin();
    Serial.print("Port: ");
    Serial.println(Port);
    GetState();
    for(int i = 1;i <= NumberGPIO;i++)
    {
        Names[i] = "Button - " + String(i);
    }
}

void SendState()
{
    GetState();
    for (WiFiClient i : Clients)
    {
        i.write(Data);
    }
}

void Operate(String RData, WiFiClient Client)
{
    String Start = RData.substring(0, RequestHeaderSize);
    String Command = RData.substring(RequestHeaderSize, RData.length() - RequestFooterSize);
    String Footer = RData.substring(RData.length() - RequestFooterSize);
    if (Start == Get)
    {
        GetState();
        Client.write(Data);
    }
    else if (Start == Set)
    {
        int index = 0;
        Serial.println(Start);
        Serial.println(Command);
        for (char i : Command)
        {
            if (i == '0')
                Switches[index] = false;
            if (i == '1')
                Switches[index] = true;
            index++;
        }
        SendState();
    }
    else if(Start == SetName)
    {
        Names[Command.toInt()] = Client.readStringUntil('\n');
    }
    else if(Start == GetName)
    {
        String Out = "";
        for(String i : Names)
        {
            Out = Out + i + Seperator;
        }
        Out = Out.substring(0,Out.length() - 1);
        Out = Out.substring(1,Out.length());
        char ChrArry[Out.length() + 1];
        strcpy(ChrArry,Out.c_str());
        Client.write(ChrArry);
    }
}