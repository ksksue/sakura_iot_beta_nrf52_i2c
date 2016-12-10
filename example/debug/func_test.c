#include "drv_sakura_iot.h"
#include "nrf_delay.h"
#include "debug.h"

#define ERR_CHECK_SAKURA_IOT(x) \
    if(x==DRV_SAKURA_IOT_RETURN_ERROR) { DPRINT_ERR(); return;}

static sakura_iot_date_time_t m_date;
static txrxdata_t m_txdata;
static txrxdata_t m_rxdata;

//**********************************************************
// general commands
//**********************************************************
static void test_drv_sakura_iot_get_connection_status(void)
{
}

static void test_drv_sakura_iot_get_signal_quality(void)
{
}

static void test_drv_sakura_iot_get_date_time(void)
{
}

static void test_drv_sakura_iot_echo_back_test(void)
{
}


//**********************************************************
// IO commands
//**********************************************************
static void test_drv_sakura_iot_read_adc(void)
{
}


//**********************************************************
// data tx commands
//**********************************************************
static void test_drv_sakura_iot_tx_enqueue(void)
{
    m_txdata.ch = 0;
    m_txdata.type = 'l';
    m_txdata.data.ul = 0x123456789ABCDEF0;
    m_txdata.data.ul = 1311768467463790300;
}

static void test_drv_sakura_iot_tx_immediately(void)
{
}

static void test_drv_sakura_iot_tx_available_queue_length(void)
{
}

static void test_drv_sakura_iot_tx_queued_length(void)
{
}

static void test_drv_sakura_iot_tx_queue_flush(void)
{
}

static void test_drv_sakura_iot_tx_queue_send(void)
{
}

static void test_drv_sakura_iot_get_tx_queue_status(void)
{
}

static void test_drv_sakura_iot_get_tx_immediately_status(void)
{
}


//**********************************************************
// data rx commands
//**********************************************************
static void test_drv_sakura_iot_rx_dequeue(void)
{
}

static void test_drv_sakura_iot_rx_queue_peek(void)
{
}

static void test_drv_sakura_iot_get_rx_available_queue_length(void)
{
}

static void test_drv_sakura_iot_get_rx_queued_length(void)
{
}

static void test_drv_sakura_iot_rx_queue_flush(void)
{
}


//**********************************************************
// etc command
//**********************************************************
static void test_drv_sakura_iot_get_product_id(void)
{
}

static void test_drv_sakura_iot_get_unique_id(void)
{
}

static void test_drv_sakura_iot_get_firmware_version(void)
{
}

static void test_drv_sakura_iot_unlock(void)
{
}

static void test_drv_sakura_iot_firmware_update(void)
{
}

static void test_drv_sakura_iot_get_firmware_update_status(void)
{
}

static void test_drv_sakura_iot_software_reset(void)
{
}



static void all_check(void)
{
//**********************************************************
// general commands
//**********************************************************
    test_drv_sakura_iot_get_connection_status();
    test_drv_sakura_iot_get_signal_quality();
    test_drv_sakura_iot_get_date_time();
    test_drv_sakura_iot_echo_back_test();

//**********************************************************
// IO commands
//**********************************************************
    test_drv_sakura_iot_read_adc();

//**********************************************************
// data tx commands
//**********************************************************
    test_drv_sakura_iot_tx_enqueue();
    test_drv_sakura_iot_tx_immediately();
    test_drv_sakura_iot_tx_available_queue_length();
    test_drv_sakura_iot_tx_queued_length();
    test_drv_sakura_iot_tx_queue_flush();
    test_drv_sakura_iot_tx_queue_send();
    test_drv_sakura_iot_get_tx_queue_status();
    test_drv_sakura_iot_get_tx_immediately_status();

//**********************************************************
// data rx commands
//**********************************************************
    test_drv_sakura_iot_rx_dequeue();
    test_drv_sakura_iot_rx_queue_peek();
    test_drv_sakura_iot_get_rx_available_queue_length();
    test_drv_sakura_iot_get_rx_queued_length();
    test_drv_sakura_iot_rx_queue_flush();

//**********************************************************
// etc command
//**********************************************************
    test_drv_sakura_iot_get_product_id();
    test_drv_sakura_iot_get_unique_id();
    test_drv_sakura_iot_get_firmware_version();
    test_drv_sakura_iot_unlock();
    test_drv_sakura_iot_firmware_update();
    test_drv_sakura_iot_get_firmware_update_status();
    test_drv_sakura_iot_software_reset();

    while(1) {
        nrf_delay_ms(10);
    }
}

static void rx_test(void)
{
    uint8_t con_status;

    uint8_t rx_queue_num;
    while(1) {
        con_status = drv_sakura_iot_get_connection_status();
        if((con_status & 0x80) != 0x80) {
            DPRINT_ERR("no connection\n");
            nrf_delay_ms(1000);
            continue;
        }

        rx_queue_num = drv_sakura_iot_get_rx_queued_length();
        if(rx_queue_num == 0) {
            DPRINT_ERR("no rx data\n");
        } else {
            drv_sakura_iot_rx_dequeue(&m_rxdata);
            D_RF_PRINTF("rxdata ch:%d type:%02x data:%d\n", m_rxdata.ch, m_rxdata.type, m_rxdata.data.ui);
        }
        nrf_delay_ms(10);
    }
}

void func_test(void)
{
    drv_sakura_iot_init();

    rx_test();

/*
    while(1) {
        con_status = drv_sakura_iot_get_connection_status();
        DPRINTF("connection status : %02x\n",con_status);
        if((con_status & 0x80) != 0x80) {
            nrf_delay_ms(3000);
            continue;
        }

        DPRINTF("signal quality : %02x\n", drv_sakura_iot_get_signal_quality());
        drv_sakura_iot_get_date_time(&m_date);
        DPRINTF("date time : %ld\n", m_date.time);
        DPRINTF("echo back test : %02x\n", drv_sakura_iot_echo_back_test(0xFF));
        drv_sakura_iot_tx_enqueue(&txdata);
        drv_sakura_iot_tx_queue_send();
        txdata.data.i++;
    }
*/
}
