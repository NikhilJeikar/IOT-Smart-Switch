//Constants
// Request Headers
#define Acknowledge "ACK"
#define SetName "SID"
#define GetName "GID"
#define Get "GET"
#define Set "SET"
#define Reset "RST"
#define Count "CNT"

#define RequestFooter "+|-|+"

#define SizeEOD 5

#define NotUsed 0
#define Used 1

#define Port 8888

#define MaxConnection 8

#define RequestHeaderSize 3
#define RequestFooterSize 5

// Global variables
extern IPAddress LocalIP;
extern IPAddress Subnet;
extern IPAddress Gateway;
extern IPAddress PrimaryDNS;
extern IPAddress SecondaryDNS;

extern const char *SSID;
extern const char *Password;

extern int DelayInterval;
extern int ConnectedCount;
extern int ConnectionStat[MaxConnection];

extern WiFiClient Clients[MaxConnection];

// Global Functions for Basic routine
void OnLoop();
