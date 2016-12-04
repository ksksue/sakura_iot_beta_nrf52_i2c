#ifndef DRV_SAKURA_IOT_H_
#define DRV_SAKURA_IOT_H_
#include <stdint.h>
#include <stdbool.h>

enum {
    // general commands
    COMM_SAKURA_IOT_GET_CONNECTION_STATUS       = 0x01,
    COMM_SAKURA_IOT_GET_SIGNAL_QUALITY          = 0x02,
    COMM_SAKURA_IOT_GET_DATE_TIME               = 0x03,
    COMM_SAKURA_IOT_ECHO_BACK_TEST              = 0x0F,

    // ADC commands
    COMM_SAKURA_IOT_READ_ADC                    = 0x10,

    // TX commands
    COMM_SAKURA_IOT_TX_ENQUEUE                  = 0x20,
    COMM_SAKURA_IOT_TX_IMMEDIATELY              = 0x21,
    COMM_SAKURA_IOT_GET_TX_QUEUE_LENGTH         = 0x22,
    COMM_SAKURA_IOT_TX_QUEUE_FLUSH              = 0x23,
    COMM_SAKURA_IOT_TX_QUEUE_SEND               = 0x24,
    COMM_SAKURA_IOT_GET_TX_STATUS               = 0x25,

    // RX commands
    COMM_SAKURA_IOT_RX_DEQUEUE                  = 0x30,
    COMM_SAKURA_IOT_RX_QUEUE_PEEK               = 0x31,
    COMM_SAKURA_IOT_GET_RX_QUEUE_LENGTH         = 0x32,
    COMM_SAKURA_IOT_RX_QUEUE_FLUSH              = 0x33,

    // file download commands
    COMM_SAKURA_IOT_START_FILE_DOWNLOAD         = 0x40,
    COMM_SAKURA_IOT_GET_FILE_METADATA           = 0x41,
    COMM_SAKURA_IOT_GET_FILE_DOWNLOAD_STATUS    = 0x42,
    COMM_SAKURA_IOT_CANCEL_FILE_DOWNLOAD        = 0x43,
    COMM_SAKURA_IOT_GET_FILE_DATA               = 0x44,

    // management commands
    COMM_SAKURA_IOT_GET_PRODUCT_ID              = 0xA0,
    COMM_SAKURA_IOT_GET_UNIQUE_ID               = 0xA1,
    COMM_SAKURA_IOT_GET_FIRMWARE_VERSION        = 0xA2,
    COMM_SAKURA_IOT_UNLOCK                      = 0xA8,
    COMM_SAKURA_IOT_FIRMWARE_UPDATE             = 0xA9,
    COMM_SAKURA_IOT_GET_FIRMWARE_UPDATE_STATUS  = 0xAA,
    COMM_SAKURA_IOT_SOFTWARE_RESET              = 0xAF,
};

enum {
    RES_STATUS_SAKURA_IOT_SUCCESS           = 0x01,
    RES_STATUS_SAKURA_IOT_ERROR_PARITY      = 0x02,
    RES_STATUS_SAKURA_IOT_ERROR_UNDEFINED   = 0x03,
    RES_STATUS_SAKURA_IOT_ERROR_RANGE       = 0x04,
    RES_STATUS_SAKURA_IOT_ERROR_DONE        = 0x05,
    RES_STATUS_SAKURA_IOT_ERROR_LOCKED      = 0x06,
    RES_STATUS_SAKURA_IOT_ERROR_OVERLAP     = 0x07,
};
/*
typedef union _txrxdata_t {
    uint8_t byte[18];
    struct {
        uint8_t ch;
        uint8_t type;
        uint8_t data[8];
        uint8_t time[8];
    } data;
} txrxdata_t;
*/

typedef struct _txrxdata_t {
        uint8_t ch;
        uint8_t type;
        uint8_t data[8];
        uint8_t time[8];
} txrxdata_t;

void drv_sakura_iot_init(void);
uint8_t drv_sakura_iot_get_connection_status(void);
uint8_t drv_sakura_iot_get_signal_quality(void);
void drv_sakura_iot_get_date_time(uint8_t *date_time);
uint8_t drv_sakura_iot_echo_back_test(uint8_t data);

void drv_sakura_iot_tx_enqueue(txrxdata_t *data);
void drv_sakura_iot_tx_queue_send(void);
void drv_sakura_iot_tx_immediately(txrxdata_t *data);

bool drv_sakura_iot_rx_dequeue(txrxdata_t *data);

#endif  // ifndef DRV_SAKURA_IOT_H_
