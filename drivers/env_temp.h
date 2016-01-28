/*
 * env_temp.h
 *
 * Created: 21/01/2016 21:38:52
 *  Author: GroundLayer
 */ 


#ifndef ENV_TEMP_H_
#define ENV_TEMP_H_

#include "asf.h"


bool ENV_TMP_init(void);
float ENV_TEMP_get_temp(void);
bool ENV_TMP_read_temp(float* temp);
void ENV_TMP_buffer_received(struct usart_module *const module);


#endif /* ENV_TEMP_H_ */