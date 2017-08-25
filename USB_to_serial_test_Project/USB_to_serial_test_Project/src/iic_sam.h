#ifndef   _IIC_SAM_H
#define   _IIC_SAM_H

#include "asf.h"
/*
 * iic_sam.h
 *
 * Created: 2016/10/24 14:21:54
 *  Author: hxm
 */ 
#define  WAIT_TIME        10
#define  TWI_CLK          400000
#define  SLAVE_ADDRESS    
#define  MEM_ADDR		  
#define  MEM_ADDR_LENGTH  

twi_options_t opt;
twi_packet_t packet_tx, packet_rx;
uint8_t iic_address[4];
extern uint8_t iic_address_cnt;
extern uint32_t iic_read_flag;

void iic_init(void);
void iic_write_oper(void);
void iic_read_oper(void);
uint8_t Dec_to_addr(uint8_t *buf);
void iic_read_count(void);
uint32_t str_to_dec(uint8_t *buf);

#endif
