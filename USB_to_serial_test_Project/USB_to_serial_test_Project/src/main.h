#ifndef _MAIN_H_
#define _MAIN_H_

#include "usb_protocol_cdc.h"

bool cdc_enable();

void cdc_disable( );

void main_sof_action(void);

void main_cdc_set_dtr(bool b_enable);

void main_suspend_action(void);

void main_resume_action(void);

#endif