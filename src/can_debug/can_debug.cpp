#include "can_debug.h"

#define ENABLE_LOG 0

void write_can(void* param)
{


}

static DRAM_ATTR char frame[1024];
static DRAM_ATTR char decodeBuff[1024];
static DRAM_ATTR uint16_t frame_index;
static DRAM_ATTR int16_t end_index = -1;
static DRAM_ATTR int16_t start_index = -1;


#define MAX_FRAME_SIZE 1024

static uint16_t canrate = 0;
static FrameSetCANRate frameSetCanRate;
static CanFrame rxFrame,txFrame;
static FrameCAN frameRx,frameTx;
FrameCANInfoReport frameCanReport;

void checkDealFrame(char *buff, uint16_t len)
{
    int dlen = inverse_escape_frame((uint8_t *)buff, (uint8_t *)decodeBuff, len);
    uint8_t csum = checkSum8(0x00, (uint8_t *)(decodeBuff + 1), dlen - 3);
    uint8_t dsum = decodeBuff[dlen - 2];
    uint8_t fid = decodeBuff[1];
    if (csum == dsum)
    {
        if(fid==0x04)
        {
            memcpy(&frameSetCanRate,decodeBuff,sizeof(frameSetCanRate));
            if(canrate!=frameSetCanRate.can_rate)
            {
                SystemConfig.config("can_rate",String(frameSetCanRate.can_rate));
                delay(10);
                ESP32Can.end();
                ESP32Can.setSpeed(ESP32Can.convertSpeed(frameSetCanRate.can_rate));
                ESP32Can.setPins(4, 5);
                ESP32Can.setRxQueueSize(5);
                ESP32Can.setTxQueueSize(5);
                ESP32Can.begin();
            }
        }
        else if(fid==0x02)
        {
            memcpy(&frameTx,decodeBuff,sizeof(frameTx));
            txFrame.identifier = frameTx.identifier;
            txFrame.extd = frameTx.extd;
            txFrame.data_length_code = frameTx.data_length_code;
            memcpy(txFrame.data,frameTx.data,txFrame.data_length_code);
            botlog_debug("write id=%d ",txFrame.identifier);
            ESP32Can.writeFrame(txFrame);  // timeout defaults to 1 ms
        }
    }
    else
    {
        // botlog_warn("crc check fail %d!=%d!", csum, dsum);
    }
}

void  dealFrame(uint16_t len)
{
    if (frame_index + len > 1024)
    {
        frame_index = 0;
    }
    Serial.readBytes(frame + frame_index,len);
    frame_index += len;
    start_index = findStartIndex5A((uint8_t *)frame, 0, frame_index);
    end_index = findEndIndex5A((uint8_t *)frame, start_index + 1, frame_index);
    while (end_index != -1)
    {
        if (start_index != -1)
        {
            uint16_t frame_len = end_index - start_index;
            checkDealFrame(frame + start_index, frame_len);
        }
        // 填补
        for (int i = 0; i < frame_index - end_index; i++)
        {
            frame[i] = frame[end_index + i];
        }
        frame_index = frame_index - end_index;
        // 更新值
        start_index = findStartIndex5A((uint8_t *)frame, 0, frame_index);
        end_index = findEndIndex5A((uint8_t *)frame, start_index + 2, frame_index);
    }
}




void wirteFrame(uint8_t *sendframe, uint16_t len)
{
    static uint8_t encodeBuff[1024];
    uint16_t encodeLen = escape_frame(sendframe, encodeBuff, len);
    Serial.write((const char *)encodeBuff, encodeLen);
}

void can_debug_task(void* param)
{
    botlog_info("frame size=%d\n",sizeof(frameCanReport));
    while (true)
    {
        while(ESP32Can.readFrame(&rxFrame, 1))
        {
            frameRx.extd = rxFrame.extd;
            frameRx.rtr = rxFrame.rtr;
            frameRx.ss = rxFrame.ss;
            frameRx.self = rxFrame.self;
            frameRx.dlc_non_comp = rxFrame.dlc_non_comp;
            
            frameRx.identifier = rxFrame.identifier;
            frameRx.data_length_code = rxFrame.data_length_code;

            memcpy(frameRx.data, rxFrame.data, frameRx.data_length_code);
            frameRx.checksum = checkSum8(0x00, (uint8_t *)&frameRx + 1, sizeof(frameRx) - 3);
            wirteFrame((uint8_t*)&frameRx,sizeof(frameRx));
        }     

        if(Serial.available()){
            dealFrame(Serial.available());
        }

        RUN_EVERY(1000,{
            if(ESP32Can.readStatus(&frameCanReport.status)){
                if(canrate==0){
                    canrate = SystemConfig.get_config_can_rate();
                }
                frameCanReport.can_rate = canrate;
                frameCanReport.checksum = checkSum8(0x00, (uint8_t *)&frameCanReport + 1, sizeof(frameCanReport) - 3);
                wirteFrame((uint8_t*)&frameCanReport,sizeof(frameCanReport));
            }
        });

        delay(1);
    }


}