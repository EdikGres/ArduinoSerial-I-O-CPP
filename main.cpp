#include <iostream>
#include "Serial.h"
int main() {
    std::string port = "COM3";

    Serial* SP = new Serial("\\\\.\\COM3");    // adjust as needed

    if (SP->IsConnected())
        printf("connected\n");

    char incomingData[256] = "";
    int dataLength = 255;
    int readResult = 0;
    char send[] = "I have a big heart";

    SP->WriteData(send, sizeof(send)/sizeof(char));

    while(SP->IsConnected())
    {
        readResult = SP->ReadData(incomingData,dataLength);
        //printf("Bytes read: (0 means no data available) %i\n",readResult);
        incomingData[readResult] = 0;

        printf("%s",incomingData);

        Sleep(500);


    }
    return 0;

    return 0;
}
