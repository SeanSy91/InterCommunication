// INNERCOMMUNICATION.H
#ifndef INNERCOMMUNICATION_H
#define INNERCOMMUNICATION_H


#include <Arduino.h>
// #include "Cmd.h"

#define UNITSTX         0x5A6B7C8D
#define UNITETX         0x1728394A

#define UNITID          0x01

typedef struct _Packet_Header_{
    uint8_t l_unitid;
    uint8_t l_cmd;
    uint16_t l_reserved;
}PacketHeader_t;

typedef struct _Frame_Header_{
    uint32_t l_stx;
    uint32_t l_length;
    PacketHeader_t l_head;
}FrameHeader_t;

typedef struct _Frame_End_{
    uint16_t l_crc16;
    uint32_t l_etx;
}FrameEnd_t;

typedef struct _Packet_PID_{
    float l_PGain;
    float l_IGain;
    float l_DGain;
}PIDPacket_t;

typedef struct _Packet_RPM_{
    float l_Target_RPM1;
    float l_Target_RPM2;
    float l_Target_RPM3;
    float l_Target_RPM4;
    float l_RPM1;
    float l_RPM2;
    float l_RPM3;
    float l_RPM4;
}RPMPacket_t;

typedef struct _Frame_PID_{
    FrameHeader_t m_stHeader;
    PIDPacket_t m_stPIDValue;
    FrameEnd_t m_stEnd;
}PIDFrame_t;

typedef struct _Frame_RPM_{
    FrameHeader_t m_stHeader;
    RPMPacket_t m_stRPMValue;
    FrameEnd_t m_stEnd;
}RPM_Frame_t;

typedef union{
    char l_buff[4];
    int l_int;
    float l_float;
    uint32_t l_uint32;
}Unit_t;

class ICHandler {
private:
    bool isValidRPMFrame(const char* data);

    HardwareSerial &serialPort;
    RPM_Frame_t g_stGetRPMData;
    char g_cRPMSuccessFlag;
    uint32_t cmdFailCount, cmdSuccessCount;
public:
    ICHandler(HardwareSerial &serial);
    
//     void begin(long baudRate);
    void begin(long baudRate, int rxPin = -1, int txPin = -1);
    
    void processSerialData();
    void logRPMData();
    
};

#endif
