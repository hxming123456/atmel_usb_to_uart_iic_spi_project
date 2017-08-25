/**
 * \file
 *
 * \brief Empty user application template
 *
 */

/**
 * \mainpage User Application template doxygen documentation
 *
 * \par Empty user application template
 *
 * Bare minimum empty user application template
 *
 * \par Content
 *
 * -# Include the ASF header files (through asf.h)
 * -# "Insert system clock initialization code here" comment
 * -# Minimal main function that starts with a call to board_init()
 * -# "Insert application code here" comment
 *
 */

/*
 * Include header files for all drivers that have been imported from
 * Atmel Software Framework (ASF).
 */
/*
 * Support and FAQ: visit <a href="http://www.atmel.com/design-support/">Atmel Support</a>
 */
#include <asf.h>
#include "conf_usb.h"
#include "usart_sam.h"
#include "spi_sam.h"
#include "main.h"
#include "iic_sam.h"


int main (void)
{
	irq_initialize_vectors();
	cpu_irq_enable();
	sleepmgr_init();

	sysclk_init();
	board_init();
	spi_config();
	iic_init();
	udc_start();
		
	while(1)
	{

	}
}


bool cdc_enable()
{
	usart_open();
	return true;
}

void cdc_disable()
{
	usart_close();
}

void main_sof_action(void)
{

}

void main_cdc_set_dtr(bool b_enable)
{
	
}

void main_suspend_action(void)
{
	
}

void main_resume_action(void)
{

}