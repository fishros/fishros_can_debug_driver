#ifndef __PROTOCDEF_H__
#define __PROTOCDEF_H__
#include <stdint.h>
#include <driver/twai.h>


typedef struct
{
    uint8_t FRAME_HEAD = 0x5A;
    uint8_t FRAME_ID = 0x01;
    struct {
        uint8_t extd: 1;           /**< Extended Frame Format (29bit ID) */
        uint8_t rtr: 1;            /**< Message is a Remote Frame */
        uint8_t ss: 1;             /**< Transmit as a Single Shot Transmission. Unused for received. */
        uint8_t self: 1;           /**< Transmit as a Self Reception Request. Unused for received. */
        uint8_t dlc_non_comp: 1;   /**< Message's Data length code is larger than 8. This will break compliance with ISO 11898-1 */
    };
    uint32_t identifier;      /**< 11 or 29 bit identifier */
    uint8_t data_length_code; /**< Data length code */
    uint8_t data[8];          /**< Data bytes (not relevant in RTR frame) */
    uint8_t checksum;         /**< Checksum, default value: 0x00 */
    uint8_t FRAME_END = 0x5A; /**< Frame end, default value: 0x5A */
} __attribute__((packed)) FrameCAN;


typedef struct
{
    uint8_t FRAME_HEAD = 0x5A;
    uint8_t FRAME_ID = 0x03;
    uint16_t can_rate;
    twai_status_info_t status;
    uint8_t checksum;        
    uint8_t FRAME_END = 0x5A;
} __attribute__((packed)) FrameCANInfoReport;

typedef struct
{
    uint8_t FRAME_HEAD = 0x5A;
    uint8_t FRAME_ID = 0x04;
    uint16_t can_rate;
    uint8_t FRAME_END = 0x5A;
} __attribute__((packed)) FrameSetCANRate;


#endif