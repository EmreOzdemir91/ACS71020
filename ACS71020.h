/*
 * ACS71020.h
 *
 *  Created on: Sep 18, 2020
 *      Author: Aniket Paluskar
 */

#ifndef APPLICATION_ACS71020_ACS71020_H_
#define APPLICATION_ACS71020_ACS71020_H_
//Invoking SPI Library
#include <ti/drivers/SPI.h>

/*
MACROS :-
Addresses of registers
*/

#define VRMS_IRMS_ADDRESS 0x20 // Vrms, Irms
#define PACTIVE 0x21           // Active Power
#define PAPPARANT 0x22         // Apparant Power
#define PREACTIVE 0x23         // Reactive Power
#define PFACTOR 0x24           // Power Factor
#define NUMPSTOUT 0x25         // Number of Samples of Current and Voltage used for calculation
#define VCODES 0x2A            // Instantaneous Voltage Measurement
#define ICODES 0x2B            // Instantaneous Current Measurement


#define TRIMMING_REGISTER 0x1B // Offset Adjustment
#define RMS_AVG_LEN 0x1C       // Number of averages
#define CUSTOMER_ACCESS 0x2F   // Customer Access

#define MSGSIZE 4
#define READ_COMMAND_ACS71020 1
#define WRITE_COMMAND_ACS71020 0

#define OFFSET_REGISTER_VALUE 2716176          // In Binary : 00000000 00101001 01110010 00010000
#define RMS_CALCULATION_NUMBER_AVERAGES 130686 // In Binary : 00000000 00000001 11111110 01111110 In Decimal : 126 for first stage, 1020 for second stage

#define CUSTOMER_ACCESS_CODE 0x4F70656E         // For writing on Offsets and Trim Registers
/*
@ACS71020_type User defined datatypes to select the Model of ACS71020
*/
typedef enum
{
    ACS71020_15A,
    ACS71020_30A
} ACS71020_type; // Models of ACS71020

float ACS71020_getIrms();
float ACS71020_getVrms();
float ACS71020_getPactive();
float ACS71020_getPapparent();
float ACS71020_getPreactive();
float ACS71020_getPfactor();
uint16_t ACS71020_getNumpstout();
float ACS71020_getVcodes();
float ACS71020_getIcodes();
bool ACS71020_SPI_init(ACS71020_type type, SPI_Handle BusHandle, float vmax);

#endif /* APPLICATION_ACS71020_ACS71020_H_ */
