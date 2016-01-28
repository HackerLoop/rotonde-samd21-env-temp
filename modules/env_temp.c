/*
 * env_temp.c
 *
 * Created: 21/01/2016 22:02:02
 *  Author: GroundLayer
 */ 

#include "modules/drivers/env_temp.h"
#include "modules/modules/env_temp.h"

#include "registrar/registrar.h"
#include "tasks/modules.h"

#define MODULE_NAME "ENV_TMP_TEMP"
#define MODULE_DATA_SIZE 1
#define MODULE_TEMP "temperature"

data_name ENV_TEMP_names[MODULE_DATA_SIZE] = { MODULE_TEMP  };
data_type ENV_TEMP_types[] = { FLOAT};

void ENV_TEMP_tick(void);
definition *ENV_TEMP_def;


void ENV_TEMP_module_init(void){
	ENV_TMP_init();
	ENV_TEMP_def = register_definition(EVENT , MODULE_NAME , ENV_TEMP_names ,ENV_TEMP_types , MODULE_DATA_SIZE , NULL, ENV_TEMP_tick , 1000);
}

void ENV_TEMP_tick(void) {
	float temp;
	
	if(ENV_TMP_read_temp(&temp)) {
		instance inst = create_instance(MODULE_NAME);
		if(inst.def != NULL && inst.values != NULL){
			(*(float*) inst.values[0]) = temp;
			portBASE_TYPE xStatus = xQueueSendToBack(module_out, &inst , 10 / portTICK_RATE_MS );
			if(xStatus != pdPASS){
				release_instance(inst);
			}
		}
	}
}