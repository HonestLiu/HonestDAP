#include <rtthread.h>
#include "packages/CherryRB/chry_ringbuffer.h"
//#include "dap_main.h"
#include "dap_main.h"
#include "main.h"


#define CONNECT_UART_NAME       "uart3"      /* 串口设备名称 */

/* 串口接收消息结构*/
struct rx_msg {
    rt_device_t dev;
    rt_size_t size;
};
/* 串口设备句柄 */
static rt_device_t serial;
/* 消息队列控制块 */
static struct rt_messagequeue rx_mq;

/* 接收数据回调函数 */
static rt_err_t uart_input(rt_device_t dev, rt_size_t size) {
    struct rx_msg msg;
    rt_err_t result;
    msg.dev = dev;
    msg.size = size;

    result = rt_mq_send(&rx_mq, &msg, sizeof(msg));//将接收的数据发送到消息队列，后继需要从消息队列中取出数据发送到USB
    if (result == -RT_EFULL) {
        /* 消息队列满 */
        rt_kprintf("message queue full！\n");
    }
    return result;
}

//串口线程，从消息队列中读取数据发送到USB
static void serial_thread_entry(void *parameter) {
    struct rx_msg msg;
    rt_err_t result;
    rt_uint32_t rx_length;
    static char rx_buffer[RT_SERIAL_RB_BUFSZ + 1];

    while (1) {
        rt_memset(&msg, 0, sizeof(msg));
        /* 从消息队列中读取消息*/
        result = rt_mq_recv(&rx_mq, &msg, sizeof(msg), RT_WAITING_FOREVER);
        if (result > 0) {
            /* 从串口读取数据*/
            rx_length = rt_device_read(msg.dev, 0, rx_buffer, msg.size);
            rx_buffer[rx_length] = '\0';
            /* 通过串口设备 serial 输出读取到的消息 */
            chry_ringbuffer_write(&g_uartrx, rx_buffer, rx_length);
        }
    }
}


//发送数据函数，将从USB获取到数据发送到串口
void serial_send_data(uint8_t *data, uint16_t len) {
    rt_device_write(serial, 0, data, len);
    chry_dap_usb2uart_uart_send_complete(len);//发送完成后通知USB，增加读指针；重要!!!!!否则缓冲区数据会没法发出去
}

int uart3_dma(void) {
    char uart_name[RT_NAME_MAX];
    static char msg_pool[256];
    rt_err_t ret = RT_EOK;

    rt_strncpy(uart_name, CONNECT_UART_NAME, RT_NAME_MAX);

    /* 查找串口设备 */
    serial = rt_device_find(uart_name);
    if (!serial) {
        rt_kprintf("find %s failed!\n", uart_name);
        return RT_ERROR;
    }

    /* 初始化消息队列 */
    rt_mq_init(&rx_mq, "rx_mq",
               msg_pool,                 /* 存放消息的缓冲区 */
               sizeof(struct rx_msg),    /* 一条消息的最大长度 */
               sizeof(msg_pool),         /* 存放消息的缓冲区大小 */
               RT_IPC_FLAG_FIFO);        /* 如果有多个线程等待，按照先来先得到的方法分配消息 */

    /* 以 DMA 接收及轮询发送方式打开串口设备 */
    rt_device_open(serial, RT_DEVICE_FLAG_DMA_RX);
    /* 设置接收回调函数 */
    rt_device_set_rx_indicate(serial, uart_input);

    /* 创建 serial 线程 */
    rt_thread_t thread = rt_thread_create("serial", serial_thread_entry, RT_NULL, 1024, 25, 10);
    /* 创建成功则启动线程 */
    if (thread != RT_NULL) {
        rt_thread_startup(thread);
    } else {
        ret = RT_ERROR;
    }
    return ret;
}

/* 初始化UART接收线程 */
INIT_APP_EXPORT(uart3_dma);


/*
UART_HandleTypeDef huart3;
// CDC UART配置回调函数
void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding){

    //struct serial_configure config = RT_SERIAL_CONFIG_DEFAULT;
    rt_kprintf("baud_rate:%d,\n data_bits:%d,\n stop_bits:%d,\n parity:%d\n", line_coding->dwDTERate, line_coding->bDataBits, line_coding->bCharFormat, line_coding->bParityType);

*/
/*    //串口校验位配置
    if (line_coding->bParityType == 0) {
        config.parity = PARITY_NONE;
    } else if (line_coding->bParityType == 1) {
        config.parity = PARITY_ODD;
    } else if (line_coding->bParityType == 2) {
        config.parity = PARITY_EVEN;
    } else {
        config.parity = PARITY_NONE;
    }
    //数据长度配置
    config.data_bits = line_coding->bDataBits;
    //停止位配置
    if (line_coding->bCharFormat == 1){
        config.stop_bits = STOP_BITS_1;
    } else if (line_coding->bCharFormat == 2){
        config.stop_bits = STOP_BITS_2;
    } else {
        config.stop_bits = STOP_BITS_1;
    }
    //波特率配置
    config.baud_rate = line_coding->dwDTERate;

    //控制串口设备，设置串口参数
    rt_device_control(serial, RT_DEVICE_CTRL_CONFIG, &config);

    rt_device_open(serial,RT_DEVICE_FLAG_INT_RX);*//*

}
*/
