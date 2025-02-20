
#include "InterCommunication.h"

ICHandler::ICHandler(HardwareSerial &serial) : serialPort(serial), g_cRPMSuccessFlag(0), cmdFailCount(0), cmdSuccessCount(0) {}

// void ICHandler::begin(long baudRate){
//     serialPort.begin(baudRate);
//     Serial.println("IC Serial Initialized");
// }

void ICHandler::begin(long baudRate, int rxPin, int txPin){
    if (rxPin != -1 && txPin != -1) {
        serialPort.begin(baudRate, SERIAL_8N1, rxPin, txPin);
    } else {
        serialPort.begin(baudRate);
    }    
    Serial.println("IC Serial Initialized");
}

void ICHandler::processSerialData() {
    if (serialPort.available()) {
        char rxdatas[80] = {0, };
        serialPort.readBytes(rxdatas, sizeof(RPM_Frame_t));
        
        if (isValidRPMFrame(rxdatas)) {
            memcpy(&g_stGetRPMData, rxdatas, sizeof(RPM_Frame_t));
            g_cRPMSuccessFlag = 1;
            cmdSuccessCount++;
        } else {
            cmdFailCount++;
        }
    }
}

void ICHandler::logRPMData() {
    if (g_cRPMSuccessFlag) {
        g_cRPMSuccessFlag = 0;
        char printbuff[100] = {0, };
        uint32_t timeclk = millis();
        
        sprintf(printbuff, "%d,1R %4.1f 2R %4.1f 3R %4.1f 4R %4.1f\r\n", timeclk,
                g_stGetRPMData.m_stRPMValue.l_RPM1, g_stGetRPMData.m_stRPMValue.l_RPM2,
                g_stGetRPMData.m_stRPMValue.l_RPM3, g_stGetRPMData.m_stRPMValue.l_RPM4);
        
        serialPort.write(printbuff, strlen(printbuff));
    }
}

bool ICHandler::isValidRPMFrame(const char* data) {
    return data[0] == 0x8d && data[1] == 0x7c && data[2] == 0x6b && data[3] == 0x5a &&
           data[48] == 0x4a && data[49] == 0x39 && data[50] == 0x28 && data[51] == 0x17;
}
