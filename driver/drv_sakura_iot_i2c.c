#include "drv_sakura_iot_i2c.h"
#include "nrf_delay.h"
#include "debug.h"

static nrf_drv_twi_t m_i2c = NRF_DRV_TWI_INSTANCE(0);
static bool m_i2c_init_done = false;
static bool m_i2c_event_done = false;

static void twi_event_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    m_i2c_event_done = true;
}

static bool wait_event_done(void)
{
    for(int i=0; i<100; i++) {
        nrf_delay_ms(1);
        if(m_i2c_event_done) {
            return true;
        }
    }

    return false;
}

void drv_sakura_iot_i2c_init(drv_sakura_iot_i2c_evt_t event_handler)
{
    if(m_i2c_init_done) {
        return;
    }

    uint32_t err_code;
    nrf_drv_twi_config_t i2c_conf = NRF_DRV_TWI_DEFAULT_CONFIG(0);
    i2c_conf.scl = TWI0_CONFIG_SCL;
    i2c_conf.sda = TWI0_CONFIG_SDA;

    err_code = nrf_drv_twi_init(&m_i2c, &i2c_conf, twi_event_handler, NULL);
    APP_ERROR_CHECK(err_code);
    nrf_drv_twi_enable(&m_i2c);
    m_i2c_init_done = true;
    m_i2c_event_done = false;
}

ret_code_t drv_sakura_iot_i2c_write(uint32_t txlength, uint8_t* txdata)
{
    if(!m_i2c_init_done) {
        return 1;
    }

    nrf_drv_twi_xfer_desc_t xfer_desc = DRV_SAKURA_IOT_I2C_WRITE_DESC(txdata, txlength);
    return nrf_drv_twi_xfer(&m_i2c, &xfer_desc, 0);
}

void drv_sakura_iot_i2c_txrx(uint32_t txlength, uint8_t* txdata, uint32_t rxlength, uint8_t* rxdata)
{
    ret_code_t err_code;

    if(!m_i2c_init_done) {
        return;
    }

    nrf_drv_twi_xfer_desc_t desc = DRV_SAKURA_IOT_I2C_READ_DESC(txdata, txlength, rxdata, rxlength);

    err_code = nrf_drv_twi_xfer(&m_i2c, &desc, NRF_DRV_TWI_FLAG_TX_NO_STOP);
    APP_ERROR_CHECK(err_code);

}

void drv_sakura_iot_i2c_read(uint32_t txlength, uint8_t* txdata, uint32_t rxlength, uint8_t* rxdata)
{
    ret_code_t err_code;

    if(!m_i2c_init_done) {
        return;
    }

    m_i2c_event_done = false;
    err_code = nrf_drv_twi_tx(&m_i2c, DRV_SAKURA_IOT_I2C_SLAVE_ADDR, txdata, txlength, false);
    APP_ERROR_CHECK(err_code);

    bool done = wait_event_done();
    if(done==false) {
        return;
        DPRINTF("done false\n");
    }

    nrf_delay_ms(10);

    m_i2c_event_done = false;
    err_code = nrf_drv_twi_rx(&m_i2c, DRV_SAKURA_IOT_I2C_SLAVE_ADDR, rxdata, rxlength);
    APP_ERROR_CHECK(err_code);
    wait_event_done();
}
