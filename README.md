# Micro-ROS2 多协议传输控制器



## 高速转发

```
python3 test_recv_rate.py
---
[INFO] [1717647133.659309251] [can_test]: CAN RECV: 3204.49 Hz
[INFO] [1717647134.659325374] [can_test]: CAN RECV: 3886.82 Hz
[INFO] [1717647135.659439243] [can_test]: CAN RECV: 3851.61 Hz
[INFO] [1717647136.659842215] [can_test]: CAN RECV: 3856.67 Hz
[INFO] [1717647137.659981359] [can_test]: CAN RECV: 3870.19 Hz
[INFO] [1717647138.660002111] [can_test]: CAN RECV: 3817.91 Hz
[INFO] [1717647139.660118618] [can_test]: CAN RECV: 3833.66 Hz
[INFO] [1717647140.660182147] [can_test]: CAN RECV: 3813.75 Hz
[INFO] [1717647141.660413550] [can_test]: CAN RECV: 3878.01 Hz
[INFO] [1717647142.660632209] [can_test]: CAN RECV: 3842.23 Hz
[INFO] [1717647143.660780058] [can_test]: CAN RECV: 3906.34 Hz
[INFO] [1717647144.660855664] [can_test]: CAN RECV: 3890.71 Hz
```

协议：


## CAN Frame RECV/SEND
```
typedef struct
{
    uint8_t FRAME_HEAD=0x5A;           
    uint8_t FRAME_ID=0x01;             
    union {
        struct {
            uint8_t extd: 1;           /**< Extended Frame Format (29bit ID) */
            uint8_t rtr: 1;            /**< Message is a Remote Frame */
            uint8_t ss: 1;             /**< Transmit as a Single Shot Transmission. Unused for received. */
            uint8_t self: 1;           /**< Transmit as a Self Reception Request. Unused for received. */
            uint8_t dlc_non_comp: 1;   /**< Message's Data length code is larger than 8. This will break compliance with ISO 11898-1 */
            uint8_t reserved: 3;       /**< Reserved bits */
        };
    };
    uint32_t identifier;                /**< 11 or 29 bit identifier */
    uint8_t data_length_code;           /**< Data length code */
    uint8_t data[8];                    /**< Data bytes (not relevant in RTR frame) */
    uint8_t checksum;                   /**< Checksum, default value: 0x00 */
    uint8_t FRAME_END=0x5A;                  /**< Frame end, default value: 0x5A */
} __attribute__((packed)) CANFrame;
```

## Seting CAN Rate

```
typedef struct
{
    uint8_t FRAME_HEAD=0x5A;                
    uint8_t FRAME_ID=0x02;                 
    uint16 can_rate; 
    uint8_t FRAME_END=0x5A;         
} __attribute__((packed)) CANRateSetFrame;
```


```
struct FrameFlag
{
    uint8_t extd0 : 1;
    uint8_t rtr1 : 1;
    uint8_t ss2 : 1;
    uint8_t self3 : 1;
    uint8_t green4 : 1;
    uint8_t dlc_non_comp : 1;
    uint8_t nc6 : 1;
    uint8_t nc7 : 1;
} __attribute__((packed));
```