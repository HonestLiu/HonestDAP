//
// Created by Hones on 24-10-1.
//

#include <rtthread.h>
#include "dap_main.h"

#define DAP_THREAD_PRIORITY         30
#define DAP_THREAD_TIMESLICE        20

static uint32_t IDCODE = 0x00000000;
char volatile current_dap_mode = 0;//DAP当前的模式

extern chry_ringbuffer_t g_uartrx;

rt_align(RT_ALIGN_SIZE)
__attribute__((section (".TCM"))) static char dap_thread_stack[1024];

static struct rt_thread dap_thread;

/* DAPLINK 线程 入口 */
static void dap_thread_entry(void *param) {
    while (1) {
        chry_dap_handle();
        chry_dap_usb2uart_handle();
    }
}

/* 启动DAPLINK线程 */
int dap_thread_startup(void) {
    rt_thread_init(&dap_thread,
                   "dap_loop",
                   dap_thread_entry,
                   RT_NULL,
                   &dap_thread_stack[0],
                   sizeof(dap_thread_stack),
                   DAP_THREAD_PRIORITY - 1, DAP_THREAD_TIMESLICE);
    rt_thread_startup(&dap_thread);

    return 0;
}

INIT_APP_EXPORT(dap_thread_startup);