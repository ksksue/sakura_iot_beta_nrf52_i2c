#ifndef DRV_SAKURA_IOT_H_
#define DRV_SAKURA_IOT_H_
#include <stdint.h>
#include <stdbool.h>

#define DRV_SAKURA_IOT_RETURN_SUCCESS   (0)
#define DRV_SAKURA_IOT_RETURN_ERROR     (0xFF)

typedef enum {
    DRV_SAKURA_IOT_CONNECTION_STATUS_CONNECTED                  = 0x80,
    DRV_SAKURA_IOT_CONNECTION_STATUS_NO_ERROR                   = 0x00,
    DRV_SAKURA_IOT_CONNECTION_STATUS_OUT_OF_SIGNAL_AREA         = 0x01,
    DRV_SAKURA_IOT_CONNECTION_STATUS_CONNECTION_ERROR           = 0x02,
    DRV_SAKURA_IOT_CONNECTION_STATUS_UNINTENDED_DISCONNECTION   = 0x03,
} drv_sakura_iot_connection_status;

typedef enum {
    DRV_SAKURA_IOT_SIGNAL_QUALITY_NO_SERVICE    = 0x00,
    DRV_SAKURA_IOT_SIGNAL_QUALITY_VERY_WEAK     = 0x01,
    DRV_SAKURA_IOT_SIGNAL_QUALITY_WEAK          = 0x02,
    DRV_SAKURA_IOT_SIGNAL_QUALITY_MIDDLE        = 0x03,
    DRV_SAKURA_IOT_SIGNAL_QUALITY_STRONG        = 0x04,
    DRV_SAKURA_IOT_SIGNAL_QUALITY_VERY_STRONG   = 0x05,
} drv_sakura_iot_signal_quality;

typedef enum {
    DRV_SAKURA_IOT_TX_STATUS_NON            = 0x00,
    DRV_SAKURA_IOT_TX_STATUS_SENDING        = 0x01,
    DRV_SAKURA_IOT_TX_STATUS_SENDING_FAILED = 0x02,
} drv_sakura_iot_tx_status;

typedef enum {
    DRV_SAKURA_IOT_FIRMWARE_UPDATE_STATUS_NO_ERROR          = 0x00,
    DRV_SAKURA_IOT_FIRMWARE_UPDATE_STATUS_ALREADY_UPDATED   = 0x01,
    DRV_SAKURA_IOT_FIRMWARE_UPDATE_STATUS_UPDATE_FAILED     = 0x02,
    DRV_SAKURA_IOT_FIRMWARE_UPDATE_STATUS_DOWNLOAD_FAILED   = 0x03,
    DRV_SAKURA_IOT_FIRMWARE_UPDATE_STATUS_VERIFY_FAILED     = 0x04,
    
} drv_sakura_iot_firmware_update_status;


typedef struct _txrxdata_t {
    uint8_t ch;
    uint8_t type;
    union {
        int32_t     i;
        uint32_t    ui;
        int64_t     l;
        uint64_t    ul;
        float       f;
        double      d;
        uint8_t     ar[8];
    } data;
    uint8_t time[8];
} txrxdata_t;

typedef union _date_time_t {
    uint64_t    time;
    uint8_t     ar[8];
} sakura_iot_date_time_t;


void drv_sakura_iot_init(void);

//**********************************************************
// general commands
//**********************************************************
uint8_t drv_sakura_iot_get_connection_status(void);
uint8_t drv_sakura_iot_get_signal_quality(void);
uint8_t drv_sakura_iot_get_date_time(sakura_iot_date_time_t *date_time);
uint8_t drv_sakura_iot_echo_back_test(uint8_t length, uint8_t *indata, uint8_t *outdata);

//**********************************************************
// IO commands
//**********************************************************
uint8_t drv_sakura_iot_read_adc(uint8_t ch, uint16_t *volt);

//**********************************************************
// data tx commands
//**********************************************************
uint8_t drv_sakura_iot_tx_enqueue(txrxdata_t *data);
uint8_t drv_sakura_iot_tx_immediately(txrxdata_t *data);
uint8_t drv_sakura_iot_tx_available_queue_length(void);
uint8_t drv_sakura_iot_tx_queued_length(void);
uint8_t drv_sakura_iot_tx_queue_flush(void);
uint8_t drv_sakura_iot_tx_queue_send(void);
uint8_t drv_sakura_iot_get_tx_queue_status(void);
uint8_t drv_sakura_iot_get_tx_immediately_status(void);

//**********************************************************
// data rx commands
//**********************************************************
uint8_t drv_sakura_iot_rx_dequeue(txrxdata_t *data);
uint8_t drv_sakura_iot_rx_queue_peek(txrxdata_t *data);
uint8_t drv_sakura_iot_get_rx_available_queue_length(void);
uint8_t drv_sakura_iot_get_rx_queued_length(void);
uint8_t drv_sakura_iot_rx_queue_flush(void);

//**********************************************************
// file download commands
//**********************************************************
/* not supported on the beta version
typedef struct _metadata_t {
    uint8_t status;
    uint8_t size[4];
    uint8_t time[8];
    uint8_t check[4];
} metadata_t;

typedef struct _dl_status_t {
    uint8_t status;
    uint8_t size[4];
} dl_status_t;

typedef struct _dl_data_t {
    uint32_t size;
    uint8_t *data;
} dl_data_t;

uint8_t drv_sakura_iot_start_file_download(void);
uint8_t drv_sakura_iot_get_file_metadata(metadata_t *data);
uint8_t drv_sakura_iot_get_file_download_status(dl_status_t *status);
uint8_t drv_sakura_iot_cancel_file_download(void);
uint8_t drv_sakura_iot_get_file_data(uint8_t length, uint8_t *data);
*/

//**********************************************************
// etc command
//**********************************************************
uint8_t drv_sakura_iot_get_product_id(uint8_t *pid);
uint8_t drv_sakura_iot_get_unique_id(uint8_t *uid);
uint8_t drv_sakura_iot_get_firmware_version(uint8_t *version);
uint8_t drv_sakura_iot_unlock(void);
uint8_t drv_sakura_iot_firmware_update(void);
uint8_t drv_sakura_iot_get_firmware_update_status(void);
uint8_t drv_sakura_iot_software_reset(void);


#endif  // ifndef DRV_SAKURA_IOT_H_
