#ifndef ACS71020_h
#define ACS71020_h

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

@value_type - User Defined datatype to select the property user wants to fetch
*/
enum ACS71020_type { ACS71020_15A, ACS71020_30A};                                               // Models of ACS71020
enum value_type {irms,vrms,pactive,papparant,preactive,pfactor,numpstout,vcodes,icodes};        // Types of ACS71020 Values


float ACS71020_getValue(value_type value);
bool ACS71020_Setup(ACS71020_type typ, int CS , int mosi,int miso,float vmax);
bool ACS71020_SPI_init(SPI_Handle BusHandle);





#endif


