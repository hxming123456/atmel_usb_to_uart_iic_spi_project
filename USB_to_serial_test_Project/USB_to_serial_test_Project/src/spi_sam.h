#ifndef _SPI_SAM_H
#define _SPI_SAM_H
/*
 * spi_sam.h
 *
 * Created: 2016/10/19 11:19:56
 *  Author: hxm
 */ 
void spi_config(void);
void spi_master_init(void);
void spi_slave_init(void);
void spi_data_to_pc(void);

extern uint32_t spi_timeout_cnt;

#endif