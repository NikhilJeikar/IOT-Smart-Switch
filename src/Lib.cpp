#include <ESP8266WiFi.h>
#include <string>
#include <LittleFS.h>
#include "Lib.h"
#include "NetworkingConstants.h"

bool Switches[NumberGPIO] = {false};
char Data[NumberGPIO];
String Names[NumberGPIO];
WiFiServer TCPServer(Port);

// Local Function Declaration
void SendState();
void GetButtonState();
void PreserveState();
void LoadState();
void ResetSystem();

// Local Function Defenition
void PreserveState()
{
    if(LittleFS.begin())
    {
        File Document = LittleFS.open("/Preserved.txt","w");
        String Out = "";
        if(Document)
        {
            for (bool i : Switches)
                Out = Out + i; 
            Out = Out + "\n";

            for(String Name:Names)
                Out = Out + Name + "\n";
        }
        if(Document.println(Out) == 0)
        {
            PreserveState();
        }
        Document.close();
    }
}

void LoadState()
{
    if(LittleFS.begin() && LittleFS.exists("/Preserved.txt"))
    {
        File Document = LittleFS.open("/Preserved.txt","r");
        int index = 0;
        String temp = Document.readStringUntil('\n');
        Serial.println(temp);
        for(char val : temp)
        {
            if (val == '0')
                Switches[index] = false;
            if (val == '1')
                Switches[index] = true;
            index++;
        }
        for (int i = 0; i < NumberGPIO; i++)
        {
            String tem = Document.readStringUntil('\n');
            Serial.println(tem);
            Names[i] = tem;
        }
        GetButtonState();
        Document.close();
    }
    else
    {
        for(int i = 1;i <= NumberGPIO;i++)
        {
            Names[i - 1] = "Button - " + String(i);
        }
    }
}

void GetButtonState()
{
    int index = 0;
    for (bool i : Switches)
    {
        if(i == 0)
            Data[index] = '0';
        if(i == 1)
            Data[index] = '1';
        index++;
    }
}

void ResetSystem()
{
    if(LittleFS.begin())
    {
        LittleFS.remove("/Preserved.txt");
        for(int i = 1;i <= NumberGPIO;i++)
        {
            Names[i - 1] = "Button - " + String(i);
            Switches[i - 1] = false;
        }
        SendState();
    }
}

void SendState()
{
    PreserveState();
    GetButtonState();
    for (WiFiClient i : Clients)
    {
        i.write(Data);
    }
}

// Global Function Defenition
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
    LoadState();
}

void Operate(String RData, WiFiClient Client)
{
    String Start = RData.substring(0, RequestHeaderSize);
    String Command = RData.substring(RequestHeaderSize, RData.length() - RequestFooterSize);
    String Footer = RData.substring(RData.length() - RequestFooterSize);
    if (Start == Get)
    {
        GetButtonState();
        Serial.println(Data);
        Client.write(Data);
    }
    else if (Start == Set)
    {
        int index = 0;
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
        String Out = Names[Command.toInt()];
        char ChrArry[Out.length() + 1];
        strcpy(ChrArry,Out.c_str());
        Client.write(ChrArry);
    }
    else if(Start == Reset)
    {
        ResetSystem();
    }
    else if(Start == Count)
    {
        Client.write(NumberGPIO);
    }
}