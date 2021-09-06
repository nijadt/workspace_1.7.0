/*
 * SHT31_Sensor.c
 */

#include "STH31.h"
#include "i2c.h"

/*------------------------------------------------------------------------*/
// fonction pour verifier le statut du capteur
SHT31_status_t SHT31_status(I2C_HandleTypeDef* hi2c)
{
	uint8_t data[3];
	SHT31_status_t SHT31_status;
	data[1] = 0x2D;
	data[0] = 0xF3;
	HAL_I2C_Master_Transmit(hi2c,SHT31_Address,data,2,1000);
	HAL_I2C_Master_Receive(hi2c,SHT31_Address,data,3,1000);
	SHT31_status.Write_data_checksum_status = data[0] & 0x01;
	SHT31_status.Command_status = (data[0] >> 1) & 0x01;
	SHT31_status.System_reset_detected = (data[0] >> 4) & 0x01;
	SHT31_status.T_tracking_alert = (data[1] >> 2) & 0x01;
	SHT31_status.T_tracking_alert = (data[1] >> 3) & 0x01;
	SHT31_status.T_tracking_alert = (data[1] >> 5) & 0x01;
	SHT31_status.T_tracking_alert = (data[1] >> 7) & 0x01;
	return SHT31_status;

}
/*------------------------------------------------------------------------*/
/*------------------------------------------------------------------------*/
// fonction de mesure de la temperature et humidit�
SHT31_data_t SHT31_readData(I2C_HandleTypeDef* hi2c)
{
	uint8_t dataraw[6];
	uint16_t Traw, Hraw;
	SHT31_data_t data;

	// SSM_CS_HIGH : Measurement Commands with a high repeatability measurement with clock stretching enabled
	dataraw[0] =  (SSM_CS_HIGH >>8) & 0xFF;  // adresse du registre
	dataraw[1] = SSM_CS_HIGH & 0xFF;			// la commande

	// envoi de l'adresse du registre et de la commande
	HAL_I2C_Master_Transmit(hi2c,SHT31_Address<<1,(uint8_t *)dataraw,2,1000);

	// reception des donn�e lues par le capteur
	HAL_I2C_Master_Receive(hi2c,SHT31_Address<<1,(uint8_t *)dataraw,6,1000);

	// donn�e de mesure sur 16 bit
	Traw = (((uint16_t)dataraw[0]) << 8) | dataraw[1];  //  concat�nation des deux donn�es de la temperature
	Hraw = (((uint16_t)dataraw[3]) << 8) | dataraw[4];  //  concat�nation des deux donn�es de l'humidit�e

	// conversion du signal de sortie du capteur vers des valeurs physique
	data.T = -45 + 175.0 *Traw / 65535.0;
	data.H = 100.0 * Hraw / 65535.0;

	return data;
}
/*------------------------------------------------------------------------*/
