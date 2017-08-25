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
	WDT->WDT_MR = WDT_MR_WDDIS;
	ioport_init();
	SysTick_Config(sysclk_get_cpu_hz() / 1000); 
	
	gpio_configure_pin(PIO_PA10_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA11_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	
	gpio_configure_pin(PIO_PB11_IDX, PIO_PERIPH_A | PIO_PULLUP);
	gpio_configure_pin(PIO_PB10_IDX, PIO_PERIPH_A | PIO_DEFAULT);

	gpio_configure_pin(PIO_PA25_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA26_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA27_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PA28_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	
	gpio_configure_pin(PIO_PB12_IDX, PIO_PERIPH_A | PIO_DEFAULT);
	gpio_configure_pin(PIO_PB13_IDX, PIO_PERIPH_A | PIO_DEFAULT);
}
