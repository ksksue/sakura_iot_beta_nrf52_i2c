#ifndef  DEBUG_H_
#define DEBUG_H_

/** 
 * -DDEBUG : printf有効/無効
 *  -DNRF_LOG_USES_RTT="1"  : J-Linkでデバッグログ
 *  -DNRF_LOG_USES_UART="1" : UART経由でデバッグログ
 */

#define DEBUG_SHOW_RF
#define DEBUG_SHOW_FUNC_TEST
#define DEBUG_SHOW_ERR
#define DEBUG_SHOW

#if ((NRF_LOG_USES_RTT == 1) || (NRF_LOG_USES_UART == 1))
    #include "nrf_log.h"

    #if defined DEBUG_SHOW_RF
        #define D_RF_PRINTF( ... )     \
            NRF_LOG_PRINTF( ## __VA_ARGS__ )
    #else
        #define D_RF_PRINTF( ... )      ((void)0)
    #endif // End of defined DEBUG_SHOW_RF

    #if defined DEBUG_SHOW_FUNC_TEST
        #define DPRINT_FUNC_TEST( fmt, ... )     \
            NRF_LOG_PRINTF("%s # " fmt , __func__, ## __VA_ARGS__ )
    #else
        #define DPRINT_FUNC_TEST( ... ) ((void)0)
    #endif // End of defined DEBUG_SHOW_ERR


    #if defined DEBUG_SHOW_ERR
        #define DPRINT_ERR( ... )     \
            NRF_LOG_PRINTF( ## __VA_ARGS__ )
    #else
        #define DPRINT_ERR( ... )      ((void)0)
    #endif // End of defined DEBUG_SHOW_ERR


    #if defined DEBUG_SHOW
        #define DPRINTF( ... )     \
            NRF_LOG_PRINTF( ## __VA_ARGS__ )

        static void DPRINTF_8BIT_HEX_ARRAY(uint8_t *array, uint32_t len, bool lf)
        {
            for(int i=0; i<len; i++) {
                DPRINTF("%02x ", array[i]);
            }

            if(lf) {
                DPRINTF("\n");
            }
        }

        static void DPRINTF_TXDATA(uint8_t *array, uint32_t len)
        {
            DPRINTF("tx : ");
            DPRINTF_8BIT_HEX_ARRAY(array, len, true);
        }

        static void DPRINTF_RXDATA(uint8_t *array, uint32_t len)
        {
            DPRINTF("rx : ");
            DPRINTF_8BIT_HEX_ARRAY(array, len, true);
        }

        #define DDPRINTF( fmt, ... )                \
            NRF_LOG_PRINTF("[%s] %s:%u # " fmt ,    \
                            __DATE__, __FILE__,     \
                            __LINE__, __VA_ARGS__   \
            )

        #define DERRPRINTF( fmt, ... )     \
            NRF_LOG_PRINTF("\033[31m%s():%u # \033[39m " fmt , __func__, __LINE__, ## __VA_ARGS__ )
        #define DTRACE( fmt, ... )     \
            NRF_LOG_PRINTF("\033[32m%s() # \033[39m " fmt "\n" , __func__, ## __VA_ARGS__ )
    #else
        #define DPRINTF( ... )          ((void)0)
        #define DPRINTF_8BIT_HEX_ARRAY( ... ) ((void)0)
        #define DPRINTF_TXDATA( ... )   ((void)0)
        #define DPRINTF_RXDATA( ... )   ((void)0)
        #define DDPRINTF( fmt, ... )    ((void)0)
        #define DERRPRINTF( fmt, ... )  ((void)0)
        #define DTRACE( fmt, ... )      ((void)0)
    #endif // End of defined DEBUG_SHOW
#endif  // End of ifdef DEBUG

#endif // ifdef DEBUG_H_
