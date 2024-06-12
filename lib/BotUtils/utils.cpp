#include "utils.h"

#define ENABLE_LOG 1

String getBoardName()
{
  char board_name[20];
  uint8_t macAddr[6];
  WiFi.macAddress(macAddr);
//   sprintf(board_name, "%02x%02x%02x%02x%02x%02x", macAddr[0],macAddr[1],macAddr[2],macAddr[3],macAddr[4], macAddr[5]);
//     sprintf(board_name, "%02x%02x%02x",macAddr[3]+ macAddr[0], macAddr[4]+macAddr[1], macAddr[5]+macAddr[2]);
  sprintf(board_name, "%02x%02x%02x",macAddr[3], macAddr[4], macAddr[5]);
  return String(board_name);
}

uint16_t calculateCRC16(uint8_t *data, size_t length)
{
    uint16_t crc = 0xFFFF;

    for (size_t i = 0; i < length; i++)
    {
        crc ^= data[i];
        for (size_t j = 0; j < 8; j++)
        {
            if (crc & 0x0001)
            {
                crc = (crc >> 1) ^ 0xA001;
            }
            else
            {
                crc >>= 1;
            }
        }
    }

    return crc;
}

// 返回整数的平方根（只保留整数部分）
int integerSquareRoot(int n) {
    if (n <= 1) {
        return n;
    }

    int left = 1, right = n, result = 0;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        
        if (mid <= n / mid) {
            left = mid + 1;
            result = mid;
        } else {
            right = mid - 1;
        }
    }

    return result;
}


uint8_t checkSum8(uint8_t crc, const uint8_t *data, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        crc += data[i];
    }
    return crc;
}

uint16_t checkSum16(uint16_t crc, const uint8_t *data, size_t length)
{
    for (size_t i = 0; i < length; i++)
    {
        crc += data[i];
    }
    return crc;
}

void printFrame2Hex(const char *title, uint8_t *buffer, uint16_t size)
{
    int i;
    botlog_debug("\n*************************start %s len(%d)**************************", title,size);
    for (i = 1; i <= size; i++)
    {
        botlog_debug_raw("0x%02X ", buffer[i - 1]);
        if (i % 16 == 0)
        {
            botlog_debug_raw("\n");
        }
    }
    botlog_debug_raw("\n*************************end %s*******************************\n", title);
}

uint8_t *allocDMABuffer(const size_t n)
{
    if (n % 4 != 0)
    {
        printf("[WARN] DMA buffer size must be multiples of 4 bytes\n");
    }
    return (uint8_t *)heap_caps_malloc(n, MALLOC_CAP_DMA);
}



#define START_BYTE1 0x55
#define START_BYTE2 0xDD
#define END_BYTE1 0x55
#define END_BYTE2 0xEE

// 在 frame 中查找 0x55 0xDD 的位置
int findStartIndex(const uint8_t *frame, int startIndex, int endIndex)
{
  for (int i = startIndex; i < endIndex; ++i)
  {
    if (frame[i] == START_BYTE1 && frame[i + 1] == START_BYTE2)
    {
      return i;
    }
  }
  return -1;
}

// 在 frame 中查找 0x55 0xEE 的位置
int findEndIndex(const uint8_t *frame, int startIndex, int endIndex)
{
  for (int i = startIndex; i < endIndex; ++i)
  {
    if (frame[i] == END_BYTE1 && frame[i + 1] == END_BYTE2)
    {
      return i + 2;
    }
  }
  return -1;
}

// 在 frame 中查找 0x55 0xDD 的位置
int findStartIndex5A(const uint8_t *frame, int startIndex, int endIndex)
{
  for (int i = startIndex; i < endIndex; ++i)
  {
    if (frame[i] == 0x5A)
    {
      return i;
    }
  }
  return -1;
}

int findEndIndex5A(const uint8_t *frame, int startIndex, int endIndex)
{
  for (int i = startIndex; i < endIndex; ++i)
  {
    if (frame[i] == 0x5A)
    {
      return i+1;
    }
  }
  return -1;
}

#define FIRST_CODE 0x5A
#define END_CODE 0x5A

int inverse_escape_frame(uint8_t *frame, uint8_t *result, uint16_t len)
{
    uint16_t i = 0, j = 0;
    result[j++] = FIRST_CODE;
    for (i = 1; i < len - 1; i++)
    {
        if (frame[i] == 0x50 && frame[i + 1] == 0x0A)
        {
            result[j++] = 0x5A;
            i++;
            continue;
        }
        if (frame[i] == 0x50 && frame[i + 1] == 0x05)
        {
            result[j++] = 0x50;
            i++;
            continue;
        }
        result[j++] = frame[i];
    }
    result[j++] = END_CODE;
    return j;
}

int escape_frame(uint8_t *frame, uint8_t *result, uint16_t len)
{
    uint16_t i = 0, j = 0;
    result[j++] = FIRST_CODE;
    for (i = 1; i < len - 1; i++)
    {
        if (frame[i] == 0x50)
        {
            result[j++] = 0x50;
            result[j++] = 0x05;
            continue;
        }
        if (frame[i] == 0x5A)
        {
            result[j++] = 0x50;
            result[j++] = 0x0A;
            continue;
        }
        result[j++] = frame[i];
    }
    result[j++] = END_CODE;
    return j;
}
