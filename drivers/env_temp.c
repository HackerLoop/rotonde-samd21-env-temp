/*
 * env_temp.c
 *
 * Created: 21/01/2016 21:38:59
 *  Author: GroundLayer
 */ 

#include "modules/drivers/env_temp.h"
#include "string.h"

static float global_temp = 25.2;

static struct usart_module ENV_TMP_uart;
static char buffer[20];
static uint16_t get_char_buffer;
static int index_data;

bool ENV_TMP_init(void){
	struct usart_config usart_conf;
	
	usart_get_config_defaults(&usart_conf);
	usart_conf.mux_setting = USART_RX_3_TX_2_XCK_3;
	usart_conf.pinmux_pad0 = PINMUX_UNUSED;
	usart_conf.pinmux_pad1 = PINMUX_UNUSED;
	usart_conf.pinmux_pad2 = PINMUX_PA20C_SERCOM5_PAD2;
	usart_conf.pinmux_pad3 = PINMUX_PA21C_SERCOM5_PAD3;
	usart_conf.baudrate    = 38400;

	while (usart_init(&ENV_TMP_uart, SERCOM5, &usart_conf) != STATUS_OK ) {
		//add a timeout if blocked here
	}
	usart_register_callback(&ENV_TMP_uart, ENV_TMP_buffer_received , USART_CALLBACK_BUFFER_RECEIVED);
	usart_enable_callback(&ENV_TMP_uart, USART_CALLBACK_BUFFER_RECEIVED);

	
	usart_enable(&ENV_TMP_uart);
	usart_read_job(&ENV_TMP_uart , &get_char_buffer);
	return true;
	
}

float ENV_TEMP_get_temp(void){
	return global_temp;
}

bool ENV_TMP_read_temp(float* temp){
	if(index_data < 8){
		return false;
	}
	
	int j;
	int i;
	char temporary[10];
	memset(temporary , 0 , 10);
	i = index_data -1;
	while(buffer[i] != '\r'  && i >= 7){
		i--;
	}
	
	j = i - 1;
	while(buffer[j] != '\r' && j > 0){
		j--;
	}
	
	if(buffer[j] != '\r') {
		memset(buffer , 0 , 20);
		usart_read_job(&ENV_TMP_uart , &get_char_buffer);
	}
	
	strncpy(temporary , buffer + j + 1 , i-j-1);
	*temp = atof(temporary);
	memset(buffer , 0 , 20);
	index_data = 0;
	global_temp = *temp;
	usart_read_job(&ENV_TMP_uart , &get_char_buffer);
	return true;
}

void ENV_TMP_buffer_received(struct usart_module *const module){
	buffer[index_data] = (char) get_char_buffer;
	if(index_data < 19){
		index_data++;
		usart_read_job(&ENV_TMP_uart , &get_char_buffer);
	}
}