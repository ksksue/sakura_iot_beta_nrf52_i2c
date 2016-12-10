#include "drv_sakura_iot.h"
#include "drv_sakura_iot_i2c.h"
#include <string.h>
#include "debug.h"

#define DRV_SAKURA_IOT_MAX_TX_DATA_SIZE 21
#define DRV_SAKURA_IOT_MAX_RX_DATA_SIZE 21

#define PAYLOAD_SIZE(x) ((x)+(3))
#define DRV_SAKURA_IOT_RX_STATUS_CHECK()    \
    if(drv_sakura_iot_check_rx_status(m_rxdata) == false) { return DRV_SAKURA_IOT_RETURN_ERROR; }

static uint8_t m_txdata[DRV_SAKURA_IOT_MAX_TX_DATA_SIZE];
static uint8_t m_rxdata[DRV_SAKURA_IOT_MAX_RX_DATA_SIZE];

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

static void drv_sakura_iot_event_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    // do nothing
}

static uint8_t drv_sakura_iot_get_parity(uint16_t length, uint8_t* data)
{
    uint16_t i;
    uint8_t parity = 0;
    for(i=0; i<length; i++) {
        parity ^= data[i];
    }
    return parity;
}

static uint16_t drv_sakura_iot_make_packet(uint8_t command, uint8_t data_len, uint8_t* data, uint8_t *out)
{
    uint8_t index;
    uint8_t i;

    out[0] = command;
    out[1] = data_len;
    index = 2;
    for(i=0; i<data_len; i++) {
        out[2+i] = data[i];
        index++;
    }
    out[index] = drv_sakura_iot_get_parity(data_len+2, out);
    return index+1;
}

static bool drv_sakura_iot_check_rx_status(uint8_t *rxdata)
{
    if(rxdata[0] != RES_STATUS_SAKURA_IOT_SUCCESS) {
        return false;
    }
    return true;
}

static void drv_sakura_iot_txdata_alignment(txrxdata_t *txdata, uint8_t *data)
{
    memcpy(data, (uint8_t *)txdata, 2);
    memcpy(data+2, (uint8_t *)&(txdata->data), 8);
    memcpy(data+10, txdata->time, 8);
}

static void drv_sakura_iot_rxdata_alignment(uint8_t *data, txrxdata_t *rxdata)
{
    memcpy((uint8_t *)rxdata, data, 2);
    memcpy((uint8_t *)&(rxdata->data), data+2, 8);
    memcpy(rxdata->time, data+10, 8);
}


void drv_sakura_iot_init(void)
{
    drv_sakura_iot_i2c_init(drv_sakura_iot_event_handler);
}


//**********************************************************
// general commands
//**********************************************************

uint8_t drv_sakura_iot_get_connection_status(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_CONNECTION_STATUS, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(1), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_get_signal_quality(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_SIGNAL_QUALITY, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(1), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_get_date_time(sakura_iot_date_time_t *date_time)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_DATE_TIME, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(8), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    memcpy(date_time->ar, &(m_rxdata[2]), 8);

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_echo_back_test(uint8_t length, uint8_t *indata, uint8_t *outdata)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_ECHO_BACK_TEST, length, indata, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(length), m_txdata, PAYLOAD_SIZE(length), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    memcpy(outdata, &(m_rxdata[2]), length);

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}


//**********************************************************
// IO commands
//**********************************************************
uint8_t drv_sakura_iot_read_adc(uint8_t ch, uint16_t *volt)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_READ_ADC, 1, &ch, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(1), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    *volt = (((uint16_t)m_rxdata[1]) << 8) | m_rxdata[0];

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}


//**********************************************************
// data tx commands
//**********************************************************
uint8_t drv_sakura_iot_tx_enqueue(txrxdata_t *data)
{
    uint8_t aldata[10];
    drv_sakura_iot_txdata_alignment(data, aldata);
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_ENQUEUE, 10, aldata, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(10), m_txdata, PAYLOAD_SIZE(0), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_tx_immediately(txrxdata_t *data)
{
    uint8_t aldata[10];
    drv_sakura_iot_txdata_alignment(data, aldata);
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_IMMEDIATELY, 10, (uint8_t *)aldata, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(10), m_txdata, PAYLOAD_SIZE(0), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_tx_available_queue_length(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_TX_QUEUE_LENGTH, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_tx_queued_length(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_TX_QUEUE_LENGTH, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[3];
}

uint8_t drv_sakura_iot_tx_queue_flush(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_QUEUE_FLUSH, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(0), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_tx_queue_send(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_QUEUE_SEND, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(0), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_get_tx_queue_status(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_TX_STATUS, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_get_tx_immediately_status(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_TX_STATUS, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[3];
}

//**********************************************************
// data rx commands
//**********************************************************
uint8_t drv_sakura_iot_rx_dequeue(txrxdata_t *data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_RX_DEQUEUE, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(18), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();
    drv_sakura_iot_rxdata_alignment(m_rxdata+2, data);

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_rx_queue_peek(txrxdata_t *data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_RX_QUEUE_PEEK, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(18), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();
    drv_sakura_iot_rxdata_alignment(m_rxdata+2, data);

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_get_rx_available_queue_length(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_RX_QUEUE_LENGTH, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_get_rx_queued_length(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_RX_QUEUE_LENGTH, 0, (void *)0, m_txdata);
    drv_sakura_iot_i2c_read_block(PAYLOAD_SIZE(0), m_txdata, PAYLOAD_SIZE(2), m_rxdata);
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return m_rxdata[3];
}


//**********************************************************
// etc commands
//**********************************************************
uint8_t drv_sakura_iot_get_product_id(uint8_t *pid)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_get_unique_id(uint8_t *uid)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_get_firmware_version(uint8_t *version)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_unlock(void)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_firmware_update(void)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_get_firmware_update_status(void)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}

uint8_t drv_sakura_iot_software_reset(void)
{
    DRV_SAKURA_IOT_RX_STATUS_CHECK();

    return DRV_SAKURA_IOT_RETURN_SUCCESS;
}
