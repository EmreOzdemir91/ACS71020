/*
 * ACS71020.h
 *
 *  Created on: Sep 18, 2020
 *      Author: Aniket Paluskar
 */

#ifndef APPLICATION_ACS71020_ACS71020_H_
#define APPLICATION_ACS71020_ACS71020_H_
#include <ti/drivers/SPI.h>
//Invoking SPI Library
#include <ti/drivers/SPI.h>


/*
MACROS :-
Addresses of registers
*/

#define VRMS_IRMS_ADDRESS     0x20                // Vrms, Irms
#define PACTIVE               0x21                // Active Power
#define PAPPARANT             0x22                // Apparant Power
#define PREACTIVE             0x23                // Reactive Power
#define PFACTOR               0x24                // Power Factor
#define NUMPSTOUT             0x25                // Number of Samples of Current and Voltage used for calculation
#define VCODES                0x2A                // Instantaneous Voltage Measurement
#define ICODES                0x2B                // Instantaneous Current Measurement







/*
@ACS71020_type User defined datatypes to select the Model of ACS71020
*/
typedef enum {ACS71020_15A,
              ACS71020_30A
              }ACS71020_type;                                               // Models of ACS71020

float ACS71020_getIrms();
float ACS71020_getVrms();
float ACS71020_getPactive();
float ACS71020_getPapparent();
float ACS71020_getPreactuve();
float ACS71020_getPfactor();
uint16_t ACS71020_getNumpstout();
float ACS71020_getVcodes();
float ACS71020_getIcodes();
bool ACS71020_SPI_init(ACS71020_type type, SPI_Handle BusHandle, float vmax);



#endif /* APPLICATION_ACS71020_ACS71020_H_ */
