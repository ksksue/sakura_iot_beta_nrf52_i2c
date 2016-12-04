#include "drv_sakura_iot.h"
#include "drv_sakura_iot_i2c.h"
#include <string.h>
#include "debug.h"

#define DRV_SAKURA_IOT_MAX_TX_DATA_SIZE 21
#define DRV_SAKURA_IOT_MAX_RX_DATA_SIZE 21

static uint8_t m_txdata[DRV_SAKURA_IOT_MAX_TX_DATA_SIZE];
static uint8_t m_rxdata[DRV_SAKURA_IOT_MAX_RX_DATA_SIZE];

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

void drv_sakura_iot_init(void)
{
    drv_sakura_iot_i2c_init(drv_sakura_iot_event_handler);
}

uint8_t drv_sakura_iot_get_connection_status(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_CONNECTION_STATUS, 0, (void *)0, m_txdata);

    DPRINTF_TXDATA(m_txdata, 3);

    drv_sakura_iot_i2c_read(3, m_txdata, 4, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 4);

    return m_rxdata[2];
}

uint8_t drv_sakura_iot_get_signal_quality(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_SIGNAL_QUALITY, 0, (void *)0, m_txdata);

    DPRINTF_TXDATA(m_txdata, 3);

    drv_sakura_iot_i2c_read(3, m_txdata, 4, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 4);

    return m_rxdata[2];
}

void drv_sakura_iot_get_date_time(uint8_t *date_time)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_GET_DATE_TIME, 0, (void *)0, m_txdata);

    DPRINTF_TXDATA(m_txdata, 3);

    drv_sakura_iot_i2c_read(3, m_txdata, 11, m_rxdata);
    memcpy(date_time, &(m_rxdata[2]), 8);

    DPRINTF_RXDATA(m_rxdata, 11);
}

uint8_t drv_sakura_iot_echo_back_test(uint8_t data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_ECHO_BACK_TEST, 1, &data, m_txdata);

    DPRINTF_TXDATA(m_txdata, 4);

    drv_sakura_iot_i2c_read(4, m_txdata, 11, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 4);

    return m_rxdata[2];
}

void drv_sakura_iot_tx_enqueue(txrxdata_t *data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_ENQUEUE, 10, (uint8_t *)data, m_txdata);
    DPRINTF_TXDATA(m_txdata, 13);

    drv_sakura_iot_i2c_read(13, m_txdata, 3, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 3);
}

void drv_sakura_iot_tx_queue_send(void)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_QUEUE_SEND, 0, (void *)0, m_txdata);

    DPRINTF_TXDATA(m_txdata, 3);

    drv_sakura_iot_i2c_read(3, m_txdata, 3, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 3);
}

void drv_sakura_iot_tx_immediately(txrxdata_t *data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_TX_IMMEDIATELY, 10, (uint8_t *)data, m_txdata);
    DPRINTF_TXDATA(m_txdata, 13);

    drv_sakura_iot_i2c_read(13, m_txdata, 3, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 3);

}

bool drv_sakura_iot_rx_dequeue(txrxdata_t *data)
{
    drv_sakura_iot_make_packet(COMM_SAKURA_IOT_RX_DEQUEUE, 0, (void *)0, m_txdata);
    DPRINTF_TXDATA(m_txdata, 3);

    drv_sakura_iot_i2c_read(3, m_txdata, 21, m_rxdata);

    DPRINTF_RXDATA(m_rxdata, 21);

    if(m_rxdata[0] == RES_STATUS_SAKURA_IOT_ERROR_DONE && m_rxdata[1] == 0x00) {
        return false;
    }

    memcpy((uint8_t *)data, &(m_rxdata[2]), 18);
    return true;
}
