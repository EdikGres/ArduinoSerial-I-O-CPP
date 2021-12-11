//
// Created by atlon on 11.12.2021.
//

#include "Serial.h"

Serial::Serial(std::string port) {
    const char *portName = port.c_str();
    this->connected = false;
    this->hSerial = CreateFile(portName,
                               GENERIC_READ | GENERIC_WRITE,
                               0,
                               NULL,
                               OPEN_EXISTING,
                               FILE_ATTRIBUTE_NORMAL,
                               NULL);
    if (this->hSerial == INVALID_HANDLE_VALUE) {
        if (GetLastError() == ERROR_FILE_NOT_FOUND) {
            printf("ERROR: Handle was not attached. Reason: %s not available.\n", portName);
        } else {
            printf("ERROR!!!");
        }
    }
        //try to set params
    else {
        DCB dcbSerialParams = {0};
        if (!GetCommState(this->hSerial, &dcbSerialParams)) {
            //If impossible, show an error
            printf("failed to get current serial parameters!");
        } else {
            //Define serial connection parameters for the arduino board
            dcbSerialParams.BaudRate = CBR_9600;
            dcbSerialParams.ByteSize = 8;
            dcbSerialParams.StopBits = ONESTOPBIT;
            dcbSerialParams.Parity = NOPARITY;
            dcbSerialParams.fDtrControl = DTR_CONTROL_ENABLE;

            //Set the parameters and check for their proper application
            if (!SetCommState(hSerial, &dcbSerialParams)) {
                printf("ALERT: Could not set Serial Port parameters");
            } else {
                this->connected = true;
                //Flush any remaining characters in the buffers
                PurgeComm(this->hSerial, PURGE_RXCLEAR | PURGE_TXCLEAR);
                //We wait 2s as the arduino board will be reseting
                Sleep(ARDUINO_DELAY);
            }
        }
    }
}

Serial::~Serial() {
    if (this->connected) {
        this->connected = false;
        CloseHandle(this->hSerial);
    }
}

int Serial::ReadData(char *buffer, unsigned int nbChar) {
    //Number of bytes we'll have read
    DWORD bytesRead;
    //Number of bytes we'll really ask to read
    unsigned int toRead;
    //Use the ClearCommError function to get status info on the Serial port
    ClearCommError(this->hSerial, &this->errors, &this->status);
    //Check if there is something to read
    if (this->status.cbInQue > 0) {
        if (this->status.cbInQue > nbChar) {
            toRead = nbChar;
        } else {
            toRead = this->status.cbInQue;
        }
        if (ReadFile(this->hSerial, buffer, toRead, &bytesRead, NULL)) {
            return bytesRead;
        }
    }

    return 0;
}

bool Serial::WriteData(const char *buffer, unsigned int nbChar) {
    DWORD bytesSend;
    //Try to write the buffer on the Serial port
    if (!WriteFile(this->hSerial, (void *) buffer, nbChar, &bytesSend, 0)) {
        //In case it don't work get comm error and return false
        ClearCommError(this->hSerial, &this->errors, &this->status);
        std::cerr << "Error write" << std::endl;
        return false;
    } else
        return true;
}

bool Serial::IsConnected() {
    return this->connected;
}
