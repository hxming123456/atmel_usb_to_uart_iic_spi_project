#ifndef _USART_SAM_H_
#define _USART_SAM_H_

void usart_open();
void usart_close();
void usart_rx_notify();
void usart_config(usb_cdc_line_coding_t *cfg);
void buf_clear_data(uint8_t *str);

#endif