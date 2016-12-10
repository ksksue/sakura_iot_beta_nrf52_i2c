#ifndef DRV_SAKURA_IOT_I2C_H_
#define DRV_SAKURA_IOT_I2C_H_

#include <stdint.h>
#include "nrf_drv_twi.h"
#include "app_error.h"
#include "app_util_platform.h"

/**
 * @brief Slave address of Sakura IoT Platform β
 */
#define DRV_SAKURA_IOT_I2C_SLAVE_ADDR   0x4F

#define DRV_SAKURA_IOT_I2C_WRITE_DESC(p_tx, tx_len) \
    {                                               \
        .type = NRF_DRV_TWI_XFER_TX,                \
        .address = DRV_SAKURA_IOT_I2C_SLAVE_ADDR,   \
        .primary_length   = tx_len,                 \
        .p_primary_buf    = p_tx,                   \
    };

#define DRV_SAKURA_IOT_I2C_READ_DESC(p_tx, tx_len, p_rx, rx_len)    \
    {                                               \
        .type = NRF_DRV_TWI_XFER_TXRX,              \
        .address = DRV_SAKURA_IOT_I2C_SLAVE_ADDR,   \
        .primary_length   = tx_len,                 \
        .secondary_length = rx_len,                 \
        .p_primary_buf    = p_tx,                   \
        .p_secondary_buf  = p_rx,                   \
    };

typedef void (*drv_sakura_iot_i2c_evt_t)(nrf_drv_twi_evt_t const * p_event, void * p_context);
void drv_sakura_iot_i2c_init(drv_sakura_iot_i2c_evt_t event_handler);
void drv_sakura_iot_i2c_read_block(uint32_t txlength, uint8_t* txdata, uint32_t rxlength, uint8_t* rxdata);

#endif  // ifndef DRV_SAKURA_IOT_I2C_H_
