#include <asf.h>
#include "spi_sam.h"
#include "usart_sam.h"
/*
 * Assembler1.S
 *
 * Created: 2016/10/19 11:18:25
 *  Author: hxm
 */
static uint32_t gs_ul_spi_clock = 500000;

//mcu spi receive data variable
bool spi_buf_full_flag;
bool spi_read_again_flag;
uint32_t spi_timeout_cnt;
uint8_t spi_rece_buf[512]={0};
uint32_t spi_rece_cnt=0;

void SPI0_Handler(void)
{
	uint16_t uc_pcs;
	uint8_t data;
	uint8_t value;

	if (spi_read_status(SPI0) & SPI_SR_RDRF)
	{
		spi_timeout_cnt = 1;
		spi_read(SPI0,&value,&uc_pcs);
		if(value == 0 && spi_rece_cnt == 0)
		{
			spi_read_again_flag = 1;
			return;
		}
		//if(value == 0 && spi_rece_cnt == 1)
		//{
		//	spi_read_again_flag = 0;
		//}
		if(spi_rece_cnt == sizeof(spi_rece_buf))
		{
			if(spi_rece_buf[0] != 0 && !spi_buf_full_flag)
			{
				udi_cdc_write_buf("SPI_0 ",6);
			}
			spi_buf_full_flag=1;
			udi_cdc_write_buf(spi_rece_buf,spi_rece_cnt);
			buf_clear_data(spi_rece_buf);
			spi_rece_cnt=0;
			spi_rece_buf[spi_rece_cnt] = value;
			spi_rece_cnt++;
		}
		else 
		{
			spi_rece_buf[spi_rece_cnt] = value;
			spi_rece_cnt++;
		}
		return;
	}
}

void spi_data_to_pc(void)
{
	if(!udi_cdc_is_tx_ready())
	{
		udi_cdc_signal_overrun();
	}
	else
	{
		if((spi_rece_buf[0] != 0) && spi_buf_full_flag)
		{
			if(spi_read_again_flag)
			{
				spi_read_again_flag = 0;
				spi_write(SPI0,0,0,0);
				return;
			}
			udi_cdc_write_buf(spi_rece_buf,spi_rece_cnt);
			spi_buf_full_flag = 0;
			buf_clear_data(spi_rece_buf);
			spi_rece_cnt = 0;
			return;
		}
		if((spi_rece_buf[0] != 0) && !spi_buf_full_flag)
		{
			if(spi_read_again_flag)
			{
				spi_read_again_flag = 0;
				spi_write(SPI0,0,0,0);
				return;
			}
			udi_cdc_write_buf("SPI_0 ",6);
			udi_cdc_write_buf(spi_rece_buf,spi_rece_cnt);
			buf_clear_data(spi_rece_buf);
			spi_rece_cnt = 0;
			return;
		}
		if(spi_read_again_flag && spi_rece_cnt != 0)
		{
			spi_read_again_flag = 0;
			spi_write(SPI0,0,0,0);
			return;
		}
	}
}

void spi_slave_init(void)
{
	spi_enable_clock(SPI0);
	spi_disable(SPI0);
	spi_reset(SPI0);
	spi_set_slave_mode(SPI0);
	spi_disable_mode_fault_detect(SPI0);
	spi_set_peripheral_chip_select_value(SPI0,spi_get_pcs(0));
	spi_set_clock_polarity(SPI0,0,0);
	spi_set_clock_phase(SPI0,0,0);
	spi_set_bits_per_transfer(SPI0,0,SPI_CSR_BITS_8_BIT);
	spi_enable_interrupt(SPI0, SPI_IER_RDRF);
	spi_enable(SPI0);
}

void spi_master_init(void)
{
	spi_enable_clock(SPI0);
	spi_disable(SPI0);
	spi_reset(SPI0);
	spi_set_master_mode(SPI0);
	spi_set_lastxfer(SPI0);
	spi_disable_mode_fault_detect(SPI0);
	spi_set_peripheral_chip_select_value(SPI0,spi_get_pcs(0));
	spi_set_clock_polarity(SPI0,0,0);
	spi_set_clock_phase(SPI0,0,0);
	
	spi_set_bits_per_transfer(SPI0,0,SPI_CSR_BITS_8_BIT);
	spi_set_baudrate_div(SPI0,0,sysclk_get_peripheral_hz()/gs_ul_spi_clock);
	spi_set_transfer_delay(SPI0,0,0xF0,0x10);
	spi_enable_interrupt(SPI0, SPI_IER_RDRF);
	spi_enable(SPI0);
	
	
}
 
void spi_config(void)
{
	spi_master_init();
	
	NVIC_DisableIRQ(SPI0_IRQn);
	NVIC_ClearPendingIRQ(SPI0_IRQn);
	NVIC_SetPriority(SPI0_IRQn,3);
	NVIC_EnableIRQ(SPI0_IRQn);
}

