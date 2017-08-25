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

#define SLAVE_ADDRESS                0xA0
#define USART_SERIAL                 USART0
#define USART_SERIAL_ID              ID_USART0
#define USART_SERIAL_BAUDRATE        115200
#define USART_SERIAL_CHAR_LENGTH     US_MR_CHRL_8_BIT
#define USART_SERIAL_PARITY          US_MR_PAR_NO
#define USART_SERIAL_STOP_BIT        US_MR_NBSTOP_1_BIT

static uint32_t gs_ul_spi_clock = 500000;

void TWI0_Handler(void)
{
	uint32_t status;
	uint8_t data;
	uint8_t buf='1';

	status = twi_get_interrupt_status(TWI0);
	
	if((status & TWI_SR_RXRDY) == TWI_SR_RXRDY)
	{
		data = twi_read_byte(TWI0);
		usart_write(USART0,data);
		for(uint32_t i=0;i<10000;i++);
		//delay_ms(1);
	}
	
	if(((status & TWI_SR_SVACC) == TWI_SR_SVACC)
		&& ((status & TWI_SR_GACC) == 0)
		&& ((status & TWI_SR_SVREAD) == TWI_SR_SVREAD)
		&& ((status & TWI_SR_TXRDY) == TWI_SR_TXRDY)
		&& ((status & TWI_SR_NACK) == 0))
	{
		twi_write_byte(TWI0,buf);
	}
	
}

void SPI_Handler(void)
{
	uint16_t uc_pcs;
	uint32_t status;
	uint8_t data;	
	static uint8_t cnt=0;
	
	if (spi_read_status(SPI) & SPI_SR_RDRF)
	{
		spi_read(SPI,&data,&uc_pcs);
		//if(data == 'a')
		//{
			spi_write(SPI,data,0,0);
		//}
		//spi_write(SPI,data,0,0);
		usart_write(USART0,data);
		//cnt++;
	}
}

void spi_slave_init(void)
{
	spi_enable_clock(SPI);
	spi_disable(SPI);
	spi_reset(SPI);
	spi_set_slave_mode(SPI);
	spi_disable_mode_fault_detect(SPI);
	spi_set_peripheral_chip_select_value(SPI,spi_get_pcs(0));
	spi_set_clock_polarity(SPI,0,0);
	spi_set_clock_phase(SPI,0,0);
	spi_set_bits_per_transfer(SPI,0,SPI_CSR_BITS_8_BIT);
	spi_enable_interrupt(SPI, SPI_IER_RDRF);
	spi_enable(SPI);
}

void spi_master_init(void)
{
	spi_enable_clock(SPI);
	spi_disable_clock(SPI);
	spi_set_master_mode(SPI);
	spi_set_lastxfer(SPI);
	spi_disable_mode_fault_detect(SPI);
	spi_set_peripheral_chip_select_value(SPI,spi_get_pcs(0));
	spi_set_clock_polarity(SPI,0,0);
	spi_set_clock_phase(SPI,0,0);
	
	spi_set_bits_per_transfer(SPI,0,SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI,0,sysclk_get_peripheral_hz()/gs_ul_spi_clock);
	spi_set_transfer_delay(SPI,0,0xF0,0x10);
	spi_enable(SPI);
}

void usart_init(void)
{
	const sam_usart_opt_t usart_console_settings = {
		USART_SERIAL_BAUDRATE,
		USART_SERIAL_CHAR_LENGTH,
		USART_SERIAL_PARITY,
		USART_SERIAL_STOP_BIT,
		US_MR_CHMODE_NORMAL,
		0
	};
	
	sysclk_enable_peripheral_clock(USART_SERIAL_ID);
	usart_init_rs232(USART_SERIAL, &usart_console_settings,sysclk_get_cpu_hz());
	usart_enable_tx(USART_SERIAL);
	usart_enable_rx(USART_SERIAL);
}

int main (void)
{
	/* Insert system clock initialization code here (sysclk_init()). */
	sysclk_init();
	board_init();
	ioport_init();
	usart_init();
	delay_init(sysclk_get_cpu_hz);
	
	pmc_enable_periph_clk(ID_TWI0);
	twi_slave_init(TWI0,SLAVE_ADDRESS);
	
	NVIC_DisableIRQ(SPI_IRQn);
	NVIC_ClearPendingIRQ(SPI_IRQn);
	NVIC_SetPriority(SPI_IRQn, 0);
	NVIC_EnableIRQ(SPI_IRQn);
	
	spi_slave_init();
	
	NVIC_DisableIRQ(TWI0_IRQn);
	NVIC_ClearPendingIRQ(TWI0_IRQn);
	NVIC_SetPriority(TWI0_IRQn, 0);
	NVIC_EnableIRQ(TWI0_IRQn);
	twi_enable_interrupt(TWI0, TWI_SR_SVACC|TWI_IER_RXRDY | TWI_IER_GACC
	| TWI_IER_NACK | TWI_IER_EOSACC | TWI_IER_SCL_WS|TWI_SR_TXRDY|TWI_SR_SVREAD);
	
	 while(1)
	 {
		 
	 }
}



