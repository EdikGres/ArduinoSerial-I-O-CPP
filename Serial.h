//
// Created by atlon on 11.12.2021.
//

#ifndef ARDUINOCPPCONNECT_SERIAL_H
#define ARDUINOCPPCONNECT_SERIAL_H
#include <iostream>
#include <windows.h>
#include <cstdio>
#include <cstdlib>

#define ARDUINO_DELAY 3000

class Serial {
private:
    HANDLE hSerial;
    bool connected;
    COMSTAT status;
    DWORD errors;
public:
    explicit Serial(std::string port);
    ~Serial();
    int ReadData(char *buffer, unsigned int nbChar);
    bool WriteData(const char *buffer, unsigned int nbChar);
    bool IsConnected();
};


#endif //ARDUINOCPPCONNECT_SERIAL_H
