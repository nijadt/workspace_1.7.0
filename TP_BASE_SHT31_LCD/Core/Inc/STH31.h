/*
 * SHT31_Sensor.h
 */

#ifndef SHT31_SENSOR_H_
#define SHT31_SENSOR_H_

#include "stm32l1xx_hal.h"
#include "stm32l1xx_hal_i2c.h"


// commandes de mesure du capteur :
#define	SSM_CS_HIGH   0x2C06			 //high repeatability measurement with clock stretching enabled
#define	SSM_CS_MEDIUM 0x2C0D			 //medium repeatability measurement with clock stretching enabled
#define	SSM_CS_LOW    0x2C10			 //low repeatability measurement with clock stretching enabled
#define	SSM_HIGH      0x2400			 //high repeatability measurement with clock stretching disabled
#define	SSM_MEDIUM    0x240B			 //medium repeatability measurement with clock stretching disabled
#define	SSM_LOW       0x2416			 //low repeatability measurement with clock stretching disabled

// adresse i2c du capteur :
#define SHT31_Address 0x44

//flag
typedef struct {
	unsigned Alert_pending_status   	: 1;
	unsigned 					    	: 1;
	unsigned Heater_status		    	: 1;
	unsigned 					    	: 1;
	unsigned RH_tracking_alert	    	: 1;
	unsigned T_tracking_alert	    	: 1;
	unsigned 					    	: 5;
	unsigned System_reset_detected  	: 1;
	unsigned							: 2;
	unsigned Command_status				: 1;
	unsigned Write_data_checksum_status	: 1;

} SHT31_status_t;

typedef struct {
	float T;
	float H;

} SHT31_data_t;


// fonction
SHT31_status_t SHT31_status(I2C_HandleTypeDef* hi2c);
SHT31_data_t SHT31_readData(I2C_HandleTypeDef* hi2c);

#endif /* SHT31_SENSOR_H_ */
