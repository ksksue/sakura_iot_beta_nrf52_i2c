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
    uint8_t ret;
    ret = drv_sakura_iot_get_connection_status();
    if((ret&0x80) == 0x80) {
        DPRINT_FUNC_TEST("connected\n");
        return;
    } else {
        DPRINT_FUNC_TEST("no connection\n");
    }

    ret &= 0x7F;

    switch(ret) {
        case DRV_SAKURA_IOT_CONNECTION_STATUS_NO_ERROR :
            DPRINT_FUNC_TEST("no error\n");
            break;
        case DRV_SAKURA_IOT_CONNECTION_STATUS_OUT_OF_SIGNAL_AREA :
            DPRINT_FUNC_TEST("out of signal area\n");
            break;
        case DRV_SAKURA_IOT_CONNECTION_STATUS_CONNECTION_ERROR :
            DPRINT_FUNC_TEST("connection error\n");
            break;
        case DRV_SAKURA_IOT_CONNECTION_STATUS_UNINTENDED_DISCONNECTION :
            DPRINT_FUNC_TEST("unintended disconnection\n");
            break;
        case DRV_SAKURA_IOT_RETURN_ERROR :
            DPRINT_FUNC_TEST("I2C return error\n");
            break;
        default :
            DPRINT_FUNC_TEST("ERROR\n");
            break;
    }
}

static void test_drv_sakura_iot_get_signal_quality(void)
{
    uint8_t ret;
    ret = drv_sakura_iot_get_signal_quality();
    switch(ret) {
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_NO_SERVICE :
            DPRINT_FUNC_TEST("no service\n", ret);
            break;
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_VERY_WEAK :
            DPRINT_FUNC_TEST("very weak\n", ret);
            break;
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_WEAK :
            DPRINT_FUNC_TEST("weak\n", ret);
            break;
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_MIDDLE :
            DPRINT_FUNC_TEST("middle\n", ret);
            break;
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_STRONG :
            DPRINT_FUNC_TEST("strong\n", ret);
            break;
        case DRV_SAKURA_IOT_SIGNAL_QUALITY_VERY_STRONG :
            DPRINT_FUNC_TEST("very strong\n", ret);
            break;
        default :
            DPRINT_FUNC_TEST("error\n", ret);
            break;
    }
}

static void test_drv_sakura_iot_get_date_time(void)
{
    uint8_t ret;
    sakura_iot_date_time_t date;
    ret = drv_sakura_iot_get_date_time(&date);
    DPRINT_FUNC_TEST("date_time : %ld\n", date.time);
}

static void test_drv_sakura_iot_echo_back_test(void)
{
    uint8_t ret;
    uint8_t indata[8] = {1,2,3,4,5,6,7,8};
    uint8_t outdata[8];
    ret = drv_sakura_iot_echo_back_test(8, indata, outdata);

    DPRINT_FUNC_TEST("%02x %02x %02x %02x %02x %02x %02x %02x\n",
        outdata[0], outdata[1], outdata[2], outdata[3],
        outdata[4], outdata[5], outdata[6], outdata[7]);

}


//**********************************************************
// IO commands
//**********************************************************
static void test_drv_sakura_iot_read_adc(void)
{
    uint8_t ret;
    uint16_t v;
    ret = drv_sakura_iot_read_adc(0, &v);
    DPRINT_FUNC_TEST("ch 0 : %d mV (%d)\n",v/10, v);

    ret = drv_sakura_iot_read_adc(1, &v);
    DPRINT_FUNC_TEST("ch 1 : %d mV (%d)\n",v/10, v);
}


//**********************************************************
// data tx commands
//**********************************************************
static void test_drv_sakura_iot_tx_enqueue(void)
{
    // unsigned int
    m_txdata.ch = 0;
    m_txdata.type = 'i';
    m_txdata.data.ui = 0x12345678;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // signed int
    m_txdata.ch = 1;
    m_txdata.type = 'I';
    m_txdata.data.i = 0x12345678;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // unsigned long
    m_txdata.ch = 2;
    m_txdata.type = 'l';
    m_txdata.data.ul = 0x123456789ABCDEF0;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // signed long
    m_txdata.ch = 3;
    m_txdata.type = 'L';
    m_txdata.data.l = 0x123456789ABCDEF0;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // float
    m_txdata.ch = 4;
    m_txdata.type = 'f';
    m_txdata.data.f = 1.23456;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // double
    m_txdata.ch = 5;
    m_txdata.type = 'd';
    m_txdata.data.d = 1.23456;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();

    // array
    m_txdata.ch = 6;
    m_txdata.type = 'a';
    m_txdata.data.ar[0] = 1;
    m_txdata.data.ar[1] = 2;
    m_txdata.data.ar[2] = 3;
    m_txdata.data.ar[3] = 4;
    m_txdata.data.ar[4] = 5;
    m_txdata.data.ar[5] = 6;
    m_txdata.data.ar[6] = 7;
    m_txdata.data.ar[7] = 8;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();
}

static void test_drv_sakura_iot_tx_immediately(void)
{
    // unsigned int
    m_txdata.ch = 0;
    m_txdata.type = 'i';
    m_txdata.data.ui = 0x12345678;

    drv_sakura_iot_tx_immediately(&m_txdata);
}

static void test_drv_sakura_iot_tx_available_queue_length(void)
{
    uint8_t ret;

    // unsigned int
    m_txdata.ch = 0;
    m_txdata.type = 'i';
    m_txdata.data.ui = 0x12345678;

    drv_sakura_iot_tx_enqueue(&m_txdata);

    ret = drv_sakura_iot_tx_available_queue_length();
    DPRINT_FUNC_TEST("available queue length : %d\n",ret);
}

static void test_drv_sakura_iot_tx_queued_length(void)
{
    uint8_t ret;
    uint16_t v;
    ret = drv_sakura_iot_tx_queued_length();
    DPRINT_FUNC_TEST("queued length : %d\n",ret);
}

static void test_drv_sakura_iot_tx_queue_flush(void)
{
    uint8_t ret;
    uint16_t v;

    ret = drv_sakura_iot_tx_queued_length();
    DPRINT_FUNC_TEST("queued length : %d\n",ret);

    ret = drv_sakura_iot_tx_queue_flush();
    DPRINT_FUNC_TEST("flush : %d\n",ret);

    ret = drv_sakura_iot_tx_queued_length();
    DPRINT_FUNC_TEST("queued length : %d\n",ret);

}

static void test_drv_sakura_iot_tx_queue_send(void)
{
    // unsigned int
    m_txdata.ch = 0;
    m_txdata.type = 'i';
    m_txdata.data.ui = 0x12345678;

    drv_sakura_iot_tx_enqueue(&m_txdata);
    drv_sakura_iot_tx_queue_send();
}

static void test_drv_sakura_iot_get_tx_queue_status(void)
{
    uint8_t ret;
    ret = drv_sakura_iot_get_tx_queue_status();

    switch(ret) {
        case DRV_SAKURA_IOT_TX_STATUS_NON :
            DPRINT_FUNC_TEST("tx status non\n");
            break;
        case DRV_SAKURA_IOT_TX_STATUS_SENDING :
            DPRINT_FUNC_TEST("tx status sending\n");
            break;
        case DRV_SAKURA_IOT_TX_STATUS_SENDING_FAILED :
            DPRINT_FUNC_TEST("tx status sending failed\n");
            break;
        default :
            DPRINT_FUNC_TEST("tx status error\n");
            break;
    }
}

static void test_drv_sakura_iot_get_tx_immediately_status(void)
{
    uint8_t ret;
    ret = drv_sakura_iot_get_tx_immediately_status(void);

    switch(ret) {
        case DRV_SAKURA_IOT_TX_STATUS_NON :
            DPRINT_FUNC_TEST("tx status non\n");
            break;
        case DRV_SAKURA_IOT_TX_STATUS_SENDING :
            DPRINT_FUNC_TEST("tx status sending\n");
            break;
        case DRV_SAKURA_IOT_TX_STATUS_SENDING_FAILED :
            DPRINT_FUNC_TEST("tx status sending failed\n");
            break;
        default :
            DPRINT_FUNC_TEST("tx status error\n");
            break;
    }
}


//**********************************************************
// data rx commands
//**********************************************************
static void test_drv_sakura_iot_rx_dequeue(void)
{
    drv_sakura_iot_rx_dequeue(&m_rxdata);
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

    all_check();
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
