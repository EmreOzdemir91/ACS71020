#ifndef ACS71020_h
#define ACS71020_h
#include<Arduino.h>

/*
Addresses of registers
*/ 

#define Vrms_Irms_address     0x20                // Vrms, Irms 
#define pactive               0x21                // Active Power
#define papparant             0x22                // Apparant Power
#define preactive             0x23                // Reactive Power
#define pfactor               0x24                // Power Factor
#define numpstout             0x25                // Number of Samples of Current and Voltage used for calculation
#define vcodes                0x2A                // Instantaneous Voltage Measurement
#define icodes                0x2B                // Instantaneous Current Measurement







/*
@ACS71020_type User defined datatypes to select the Model of ACS71020

@value_type - User Defined datatype to select the property user wants to fetch
*/
enum ACS71020_type { ACS71020_15A, ACS71020_30A};                                               // Models of ACS71020
enum value_type {irms,vrms,pactive,papparant,preactive,pfactor,numpstout,vcodes,icodes};        // Types of ACS71020 Values

/*
ChS = Chip Select,
MOSI = Master Out Slave In Pin,
MISO = Master In Slave Out Pin,
SCLK = Serial Clock Pin
Vmax = Full Scale Voltage
Imax = Full Scale Current
*/
int ChS, MOSI, MISO,SCLK;            
float Vmax, Imax;

float getvalue(value_type value);
void setup(ACS71020_type typ, int CS , int mosi,int miso,float vmax);
void ConnectbySPI();
void DisconnectbySPI();
float convertToDecimal(long n);





#endif


