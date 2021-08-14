// I/O Constants
#define BaudRate 9600
#define NumberGPIO 17

// Global variables for Preserving the state
extern bool Switches[];
extern WiFiServer TCPServer;

extern char Data[NumberGPIO];
extern String Names[NumberGPIO];

// Global Functions for Basic routine
void Init();
void Operate(String RData, WiFiClient Client);