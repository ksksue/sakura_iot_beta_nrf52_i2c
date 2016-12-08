#include "drv_sakura_iot.h"
#include "nrf_delay.h"
#include "debug.h"

sakura_iot_date_time_t m_date;

void func_test(void)
{
    txrxdata_t txdata;
    txrxdata_t rxdata;
    uint8_t rx_queue_num;
    uint8_t con_status;

    drv_sakura_iot_init();

    txdata.ch = 0;
    txdata.type = 'l';
    txdata.data.ul = 0x123456789ABCDEF0;
    txdata.data.ul = 1311768467463790300;
    while(1) {
        con_status = drv_sakura_iot_get_connection_status();
/*
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
*/
        rx_queue_num = drv_sakura_iot_get_rx_queue_length();
        if(rx_queue_num == 0) {
            DPRINTF("no rx data\n");
        } else {
            drv_sakura_iot_rx_dequeue(&rxdata);
            D_RF_PRINTF("rxdata ch:%d type:%02x data:%d\n", rxdata.ch, rxdata.type, rxdata.data.ui);
        }
        nrf_delay_ms(10);
    }
}