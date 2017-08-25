/**
 * \file
 *
 * \brief User board initialization template
 *
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */

#include <asf.h>
#include <board.h>
#include <conf_board.h>

void board_init(void)
{
	/* This function is meant to contain board-specific initialization code
	 * for, e.g., the I/O pins. The initialization can rely on application-
	 * specific board configuration, found in conf_board.h.
	 */
	//USART0_PORT_CONFIG 
	//gpio_configure_pin(PIO_PA19_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	//gpio_configure_pin(PIO_PA18_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	
	//UART_PORT_CONFIG 
	WDT->WDT_MR = WDT_MR_WDDIS;
	gpio_configure_pin(PIO_PA19_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA18_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	
	gpio_configure_pin(PIO_PA9_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA10_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	
	gpio_configure_pin(PIO_PA13_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA14_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA15_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA16_IDX, PIO_PERIPH_A | PIO_DEFAULT);
}
