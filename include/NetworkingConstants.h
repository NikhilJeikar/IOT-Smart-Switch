//Constants
// Request Headers
#define Acknowledge "ACK"
#define SetName "SID"
#define GetName "GID"
#define Get "GET"
#define Set "SET"

#define Seperator '/'
#define RequestFooter "+|-|+"

#define SizeSeperator 7
#define SizeEOD 5
#define Buffer 1024 * 4

#define NotUsed 0
#define Used 1

#define Port 8888

#define MaxConnection 8

#define RequestHeaderSize 3
#define RequestFooterSize 5

// Global variables
extern const char *SSID;
extern const char *Password;

extern const int Channel;
extern int DelayInterval;
extern int ConnectedCount;
extern int ConnectionStat[MaxConnection];

extern WiFiClient Clients[MaxConnection];

// Global Functions for Basic routine
void OnLoop();
