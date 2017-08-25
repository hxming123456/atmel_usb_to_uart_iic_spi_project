#include "iic_sam.h"

/*
 * iic_sam.c
 *
 * Created: 2016/10/24 14:21:39
 *  Author: hxm
 */ 

uint8_t  iic_address_cnt=0;
uint8_t  iic_read_buf[512];
uint8_t  iic_read_cnt;
uint32_t iic_read_flag;

void iic_init(void)
{
	pmc_enable_periph_clk(ID_TWI1);
	
	opt.master_clk = sysclk_get_cpu_hz();
	opt.speed      = TWI_CLK;
	opt.smbus      = 0;
	
	packet_tx.chip        = 0;
	packet_tx.addr_length = 0;
	packet_tx.buffer      = NULL;
	packet_tx.length      = 1;
	
	twi_master_init(TWI1, &opt);
}

uint8_t Dec_to_addr(uint8_t *buf)
{
	uint8_t data[2];
	uint8_t addr=0;
	
	for(uint32_t i=0;i<2;i++)
	{
		data[i] = *buf;
		if(data[i] >= '0' && data[i] <= '9')
		{
			data[i] = data[i] - '0';
		}
		else if(data[i] >= 'A' && data[i] <= 'F')
		{
			data[i] = 10 + (data[i] - 'A');
		}
		else if(data[i] >= 'a' && data[i] <= 'f')
		{
			data[i] = 10 + (data[i] - 'a');
		}
		buf++;
	}
	
	addr = addr | (data[0]<<4);
	addr = addr | (data[1]);
	
	return addr;
}

void iic_write_oper(void)
{
	uint8_t buf;
	
	buf = udi_cdc_getc();
	packet_tx.chip = Dec_to_addr(iic_address);
	packet_tx.buffer = &buf;
	packet_tx.length = 1;
	twi_master_write(TWI1, &packet_tx);
}

void iic_read_oper(void)
{
	uint8_t buf[512]={0};
	
	packet_rx.chip  = Dec_to_addr(iic_address);
	packet_rx.buffer = buf;
	packet_rx.length = str_to_dec(iic_read_buf);
	
	if(packet_rx.length > 512)
	{
		packet_rx.length = 512;
	}
	
	twi_master_read(TWI1,&packet_rx);
	udi_cdc_write_buf("IIC_1 ",6);
	udi_cdc_write_buf(buf,packet_rx.length);
	udi_cdc_putc('\n');
	
	buf_clear_data(iic_read_buf);
	iic_read_cnt = 0;
}

void iic_read_count(void)
{
	int str;
	
	if(iic_read_cnt >= 3)
	{
		str = udi_cdc_getc();
		return;
	}
	else
	{
		iic_read_buf[iic_read_cnt] = udi_cdc_getc();
		iic_read_cnt++;
	}
}

uint32_t str_to_dec(uint8_t *buf)
{
	uint8_t i=0,j=0;
	uint8_t iic_buf[512]={0};
	uint32_t data=0;
	
	while(*buf != '\0')
	{
		if(*buf >= '0' && *buf <= '9')
		{
			iic_buf[i] = *buf - '0';
			i++;
			buf++;
		}
		else
		{
			break;
		}	
	}
	
	while(i > 0)
	{
		if(i==3)
		{
			data += (iic_buf[j] * 100);
		}
		if(i==2)
		{
			data += (iic_buf[j] * 10);
		}
		if(i==1)
		{
			data += iic_buf[j];
		}
		j++;
		i--;
	}
	
	return data;
}

