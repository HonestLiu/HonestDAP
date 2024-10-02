//
// Created by Hones on 24-10-2.
//

#ifndef RTTHREAD_DAP_MAIN_H
#define RTTHREAD_DAP_MAIN_H

#include "usbd_core.h"
#include "usbd_cdc.h"
#include "usbd_msc.h"
#include "packages/CherryRB/chry_ringbuffer.h"

extern chry_ringbuffer_t g_uartrx;
extern chry_ringbuffer_t g_usbrx;

void chry_dap_usb2uart_handle(void);
void chry_dap_init(uint8_t busid, uintptr_t reg_base);
void serial_send_data(uint8_t *data, uint16_t len);
void chry_dap_usb2uart_uart_send_complete(uint32_t size);
void chry_dap_usb2uart_uart_config_callback(struct cdc_line_coding *line_coding);

#endif //RTTHREAD_DAP_MAIN_H
