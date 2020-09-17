#include "ACS71020.h"

/* Driver Header files */

#include <ti/drivers/SPI.h>


//MSG size which will be recieving from Slave or ACS71020
#define MSGSIZE  4


// Static Function Declaration
static float convertToDecimal(long n); 

//Declaration of SPI Properties
SPI_Handle          spiHandle;
SPI_Params          spiParams;
SPI_Transaction     spiTransaction;

uint8_t             transmitBuffer[2];
unit8_t             recieveBuffer[MSGSIZE];

/*
The Master writes on the MOSI line the 7-bit address of the
register to be read from or written to.
The next bit on the MOSI line is the read/write (RW) indicator.
A high state indicates a Read and a low state indicates a Write.

*/


transmitBuffer[1] = 1;        // Read command Indicator

//Initializing SPI Instance and other properties
SPI_init();
SPI_Params_init(&spiParams);
spiParams.dataSize = 8;
spiParams.transferMode = SPI_MODE_BLOCKING;

//New SPI Instance declaraton
spiHandle = SPI_open(CONFIG_SPI_MASTER, &spiParams);

//Checking if SPI Instamce is opened or not
if(spiHandle == NULL)
{
    while(1);
}
else
{
    printf("Master SPI initialized\n");
}
// Setting Up transaction parameters
spiTransaction.count = MSGSIZE;
spiTransaction.txBuf = (void *)transmitBuffer;
spiTransaction.rxBuf = (void *)receiveBuffer;



/********************************************************************
 * @fn          getvalue(value_type typ)
 *
 * @brief       Get the property or value of energy
 *
 * @param       Value_type keyword to fetch the data
 *
 * @return      float value of the demanded data
 */
float getvalue(value_type typ){
  long temp;
  float register_val;

  switch (typ)
  {
    /*Case to find Effective Current
      Range of Vrms ouput is 0 : ~1
        */
  case irms:
        transmitBuffer[0] = VRMS_IRMS_ADDRESS;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[1] | (recieveBuffer[0]<<8);
        register_val = convertToDecimal(temp);
        register_val = register_val * Imax;
        return(register_val);
        break;
    /*Case to find Effective Voltage
      Range of Vrms ouput is 0 : ~1
        */ 
  case vrms:
        transmitBuffer[0] = VRMS_IRMS_ADDRESS;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        register_val = temp*Vmax;
        return(register_val);
        break;
    /* Case to find Active Power
        Range of Vrms ouput is -2 : ~2
        */ 
  case pactive:
        transmitBuffer[0] = PACTIVE;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        if(temp/100000000000000000 == 1){
        /*To see if value is positive or negative value is divided by 10^16 as output is 17bit long
         Range of Active power is -2 : ~2
         */
        register_val = convertToDecimal(temp%100000000000000000);
        }
        else
        {
          register_val = convertToDecimal(temp);
        }
        return(register_val);
        break;
    /* Case to find Apparent Power
        Range of output Apparent power is 0 : ~2
     */
  case papparant:

        transmitBuffer[0] = PAPPARANT;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        register_val = temp*Imax*Vmax;
        return(register_val);
        break;
    /*case to find reactive power
      Range of reactive power is 0 : ~2
      */
  case preactive:
        transmitBuffer[0] = PREACTIVE;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        register_val = temp*Imax*Vmax;
        return(register_val);
        break;
    /*Case to find Power Factor
      Range of Power Factor is  -2 : ~2
      */
  case pfactor:
        transmitBuffer[0] = PFACTOR;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        if(temp/10000000000 == 1){
        //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
        register_val = convertToDecimal(temp%10000000000);
        }
        else{
        register_val = convertToDecimal(temp);
        }
        return(register_val);
        break;
    /*Number of samples of current and voltage used for calculations
      Range 0 to 511
      */
  case numpstout:
        transmitBuffer[0] = NUMPSTOUT;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        return(register_val);
        break;
    /* To find Instantaneous voltage measurement
     Rangs of vcodes is -1 : ~1
    */
  case vcodes:
        transmitBuffer[0] = VCODES;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        register_val = register_val*Vmax;
        return(register_val);
        break;
    /* To find Instantaneous current measurement
       Rangs of vcodes is -2 : ~2
       */
  case icodes:
        transmitBuffer[0] = ICODES;
        SPI_transfer(spiHandle , &spiTransaction);
        temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = convertToDecimal(temp);
        register_val = register_val*Imax;
        return(register_val);
        break;
  
  default:
      break;
  }
     
}



/********************************************************************
 * @fn          convertToDecimal(long n)
 *
 * @brief       Converts output given by ACS71020 (Binary) to decimal
 *
 * @param       long n (Output from SPI.transfer())
 *
 * @return      float value in Decimal 
 */
float convertToDecimal(long n) {
    float dec = 0;
    int i = 0, rem;
    while (n != 0) {
        rem = n % 10;
        n /= 10;
        dec += rem * pow(2, i);
        ++i;
    }
    return dec;
}



/********************************************************************
 * @fn          ACS71020_setup(ACS71020_type typ, int CS , int mosi,int miso,float vmax)
 *
 * @brief       Initialize the pine
 *
 * @param       ACS71020_type , Chip Select , MOSI Pin of Master, MISO pin of Master, Full Scale Voltage
 *
 * @return      None 
 */
void ACS71020_setup(ACS71020_type typ, int CS , int mosi,int miso,float vmax)
{   
    ChS = CS;
    MOSI = mosi;
    MISO = miso;
    Vmax = vmax;
  switch(typ){                                // Imax is defined by model name
    case ACS71020_15A:
      Imax = 15.00;
      break;
    case ACS71020_30A:
      Imax = 30.00;
      break;
    default:
      Imax = 30.00;    
    }
    pinMode(ChS, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCLK, OUTPUT);
    
}