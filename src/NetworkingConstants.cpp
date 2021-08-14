#include <ESP8266WiFi.h>
#include "NetworkingConstants.h"
#include "Lib.h"

const char *SSID = "Your router name(SSID)";
const char *Password = "Your router password";

int DelayInterval = 1000;
int ConnectedCount = 0;
int ConnectionStat[MaxConnection] = {NotUsed};

WiFiClient Clients[MaxConnection];

void OnLoop()
{
    WiFiClient client = TCPServer.available();
    if(client)
    {
        for (int i = 0; i < MaxConnection; i++)
        {
            if(ConnectionStat[i] == NotUsed)
            {
                ConnectedCount++;
                Clients[i] = client;
                ConnectionStat[i] = Used;
                break;
            }
        }
        
    }
    for(int i = 0;i < MaxConnection;i++)
    {
        if(ConnectionStat[i] == Used)
        {
            if(Clients[i].connected())
            {
                Clients[i].setTimeout((long) (DelayInterval/ConnectedCount));
                String Received = Clients[i].readStringUntil('\n');
                Operate(Received,Clients[i]);
            }
            else
            {
                ConnectedCount--;
                ConnectionStat[i] = NotUsed;
            }
        }
    }
}
