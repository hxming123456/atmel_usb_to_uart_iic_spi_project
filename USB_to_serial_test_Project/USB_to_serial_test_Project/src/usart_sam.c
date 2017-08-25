#include <asf.h>
#include "usart_sam.h"
#include "main.h"
#include "compiler.h"
#include "string.h"
#include "spi_sam.h"
#include "iic_sam.h"

void buf_clear_data(uint8_t *str);

static sam_usart_opt_t usart_console_settings;

//mcu usart receive data variable
bool usart_buf_full_flag;
uint32_t usart_timeout_cnt;
uint8_t usart_rece_buf[512]={0};
uint32_t usart_rece_cnt=0;

//mcu spi receive data variable
extern uint32_t spi_timeout_cnt;

//mcu usb receive data variable
uint32_t usb_timeout_cnt;
uint8_t usb_rece_head[20]={0};
uint32_t usb_rece_cnt=0;

extern uint8_t iic_address_cnt;
extern uint32_t iic_read_flag;

void Rece_data_deal(const uint8_t *head)
{
	int str;
	
	if(strcmp(head,"UART0 ")==0)
	{
		usart_write(USART0,udi_cdc_getc());
	}
	else if(strcmp(head,"SPI_0 ")==0)
	{
		str = udi_cdc_getc();
		spi_write(SPI0,str,0,0);
		delay_ms(1);
	}
	else if(strcmp(head,"IIC_1 ")==0)
	{
		if(iic_address_cnt < 4)
		{
			iic_address[iic_address_cnt] = udi_cdc_getc();
			iic_address_cnt++;
			return;
		}
		if(iic_address[2] == 'w' && (iic_address[3] == ' '))
		{		
			iic_write_oper();
			delay_ms(1);
			return;
		}
		else if(iic_address[2] == 'r' && (iic_address[3] == ' '))
		{
			iic_read_flag = 1;
			iic_read_count();
			return;
		}
		else
		{
			str = udi_cdc_getc();
		}
	}
	else
	{
		str = udi_cdc_getc();
	}
}

void usart_data_to_pc(void)
{
	if(!udi_cdc_is_tx_ready())
	{
		udi_cdc_signal_overrun();
	}
	else
	{
		if((usart_rece_buf[0] != 0) && usart_buf_full_flag)
		{
			udi_cdc_write_buf(usart_rece_buf,usart_rece_cnt);
			usart_buf_full_flag = 0;
			buf_clear_data(usart_rece_buf);
			usart_rece_cnt = 0;
			return;
		}
		if((usart_rece_buf[0] != 0) && !usart_buf_full_flag)
		{
			udi_cdc_write_buf("UART0 ",6);
			udi_cdc_write_buf(usart_rece_buf,usart_rece_cnt);
			udi_cdc_putc('\n');
			buf_clear_data(usart_rece_buf);
			usart_rece_cnt = 0;
			return;
		}
	}
}

void SysTick_Handler()
{
	usart_timeout_cnt++;
	usb_timeout_cnt++;
	spi_timeout_cnt++;
	if(usb_timeout_cnt >= 5)
	{				
		usb_timeout_cnt = 1;
		buf_clear_data(usb_rece_head);
		usb_rece_cnt=0;
		if(iic_read_flag)
		{
			iic_read_flag = 0;
			iic_read_oper();
		}
		iic_address_cnt = 0;
		buf_clear_data(iic_address);
	}
	if(usart_timeout_cnt >= 5)
	{
		usart_timeout_cnt = 1;
		usart_data_to_pc();
	}
	if(spi_timeout_cnt >= 5)
	{
		spi_timeout_cnt = 1;
		spi_data_to_pc();
	}
}


void buf_clear_data(uint8_t *str)
{
	while(*str != 0)
	{
		*str = 0;
		str++;
	}
}

void USART0_Handler(void)
{
	bool b_error;
	uint32_t value;
	int str;
	uint32_t sr = usart_get_status(USART0);
	
	if(sr & US_CSR_RXRDY)//mcu usb send data
	{
		usart_timeout_cnt = 1;
		b_error = usart_read(USART0,&value) || (sr & (US_CSR_FRAME | US_CSR_TIMEOUT | US_CSR_PARE));
		if(b_error)
		{
			usart_reset_rx(USART0);
			usart_enable_rx(USART0);
			udi_cdc_signal_framing_error();
		}
		if(usart_rece_cnt == sizeof(usart_rece_buf))
		{
			if(usart_rece_buf[0] != 0 && !usart_buf_full_flag)
			{
				udi_cdc_write_buf("UART0 ",6);
			}
			usart_buf_full_flag=1;
			udi_cdc_write_buf(usart_rece_buf,usart_rece_cnt);
			buf_clear_data(usart_rece_buf);
			usart_rece_cnt=0;
			usart_rece_buf[usart_rece_cnt] = value;
			usart_rece_cnt++;
		}
		else
		{
			usart_rece_buf[usart_rece_cnt] = value;
			usart_rece_cnt++;
		}
		return;
	}
	
	if(sr & US_CSR_TXRDY) //mcu usb receive data
	{
		if(udi_cdc_is_rx_ready()) 
		{
			usb_timeout_cnt = 1;
			if(usb_rece_cnt < 6)
			{
				usb_rece_head[usb_rece_cnt] = udi_cdc_getc();
				usb_rece_cnt++;
				return;
			}
			Rece_data_deal(usb_rece_head);
		}
		else
		{
			usart_disable_tx(USART0);
			usart_disable_interrupt(USART0,US_IDR_TXRDY);
		}
	}
}

void usart_open()
{
	sysclk_enable_peripheral_clock(ID_USART0);
	if(usart_init_rs232(USART0, &usart_console_settings,sysclk_get_cpu_hz()))
	{
		return;
	}
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	NVIC_SetPriority(USART0_IRQn, 3);
	NVIC_EnableIRQ(USART0_IRQn);
	usart_enable_interrupt(USART0, US_IER_RXRDY | US_IER_TXRDY);
}

void usart_close()
{
	usart_disable_interrupt(USART0, 0xFFFFFFFF);
}

void usart_rx_notify()
{
	if(usart_get_interrupt_mask(USART0) & US_IMR_RXRDY)
	{
		usart_enable_tx(USART0);
		usart_enable_interrupt(USART0,US_IER_TXRDY);
	}
}

void usart_config(usb_cdc_line_coding_t *cfg)
{
	uint32_t stopbits,parity,databits;
	uint32_t imr;
	
	switch(cfg->bCharFormat)
	{
		case CDC_STOP_BITS_2:
			stopbits = CDC_STOP_BITS_2;
			break;
		case CDC_STOP_BITS_1_5:
			stopbits = CDC_STOP_BITS_1_5;
			break;
		case CDC_STOP_BITS_1:
			default:
			// Default stop bit = 1 stop bit
			stopbits = US_MR_NBSTOP_1_BIT;
			break;
	}
	
	switch(cfg->bParityType)
	{
		case CDC_PAR_EVEN:
			parity = US_MR_PAR_EVEN;
			break;
		case CDC_PAR_ODD:
			parity = US_MR_PAR_ODD;
			break;
		case CDC_PAR_MARK:
			parity = US_MR_PAR_MARK;
			break;
		case CDC_PAR_SPACE:
			parity = US_MR_PAR_SPACE;
			break;
		default:
		case CDC_PAR_NONE:
			parity = US_MR_PAR_NO;
			break;
	}
	
	switch(cfg->bDataBits) {
		case 5: case 6: case 7:
			databits = cfg->bDataBits - 5;
			break;
		default:
		case 8:
			databits = US_MR_CHRL_8_BIT;
			break;
	}
	
	usart_console_settings.baudrate = LE32_TO_CPU(cfg->dwDTERate);
	usart_console_settings.char_length = databits;
	usart_console_settings.parity_type = parity;
	usart_console_settings.stop_bits = stopbits;
	usart_console_settings.channel_mode = US_MR_CHMODE_NORMAL;
	
	imr = usart_get_interrupt_mask(USART0);
	usart_disable_interrupt(USART0, 0xFFFFFFFF);
	usart_init_rs232(USART0, &usart_console_settings,
	sysclk_get_cpu_hz());
	
	usart_enable_tx(USART0);
	usart_enable_rx(USART0);
	usart_enable_interrupt(USART0, imr);
}