#include "ACS71020.h"

/* Driver Header file */
#include <ti/drivers/SPI.h>


/*  MACRO :- MSG size which will be recieving from Slave or ACS71020  */
#define MSGSIZE  4


/*****************************************
 * Local Functions */
static float convertToDecimal(long n);


/*****************************************
 * Local Variables 
 * ChS = Chip Select,
 * MOSI = Master Out Slave In Pin,
 * MISO = Master In Slave Out Pin,
 * SCLK = Serial Clock Pin
 * Vmax = Full Scale Voltage
 * Imax = Full Scale Current
 */
static int ChS, MOSI, MISO,SCLK;            
static float Vmax, Imax;
static SPI_Handle          spiHandle;
static SPI_Transaction     spiTransaction;

static uint8_t             transmitBuffer[2];
static unit8_t             recieveBuffer[MSGSIZE];

/*
The Master writes on the MOSI line the 7-bit address of the
register to be read from or written to.
The next bit on the MOSI line is the read/write (RW) indicator.
A high state indicates a Read and a low state indicates a Write.

*/


transmitBuffer[1] = 1;        // Read command Indicator

/*********************************************
 *  Setting Up transaction parameters */

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

float ACS71020_getValue(value_type typ){
  long temp;
  float register_val;
  bool transferStatus = FALSE;  

  switch (typ)
  {
    /*Case to find Effective Current
      Range of Vrms ouput is 0 : ~1
        */
  case irms:
        transmitBuffer[0] = VRMS_IRMS_ADDRESS;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[1] | (recieveBuffer[0]<<8);
          register_val = convertToDecimal(temp);
          register_val = register_val * Imax;
          return(register_val);
        }
        else
        {
          return(NULL);
        }
        
        break;
    /*Case to find Effective Voltage
      Range of Vrms ouput is 0 : ~1
        */ 
  case vrms:
        transmitBuffer[0] = VRMS_IRMS_ADDRESS;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
          register_val = convertToDecimal(temp);
          register_val = temp*Vmax;
          return(register_val);
        }
        else
        {
          return(NULL);
        }
        

        break;
    /* Case to find Active Power
        Range of Vrms ouput is -2 : ~2
        */ 
  case pactive:
        transmitBuffer[0] = PACTIVE;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
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

        }
        else
        {
          return(NULL);
        }
        break;
    /* Case to find Apparent Power
        Range of output Apparent power is 0 : ~2
     */
  case papparant:

        transmitBuffer[0] = PAPPARANT;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
            temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
            register_val = convertToDecimal(temp);
            register_val = temp*Imax*Vmax;
            return(register_val);
        }
        else
        {
          return(NULL);
        }
        break;
    /*case to find reactive power
      Range of reactive power is 0 : ~2
      */
  case preactive:
        transmitBuffer[0] = PREACTIVE;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
          register_val = convertToDecimal(temp);
          register_val = temp*Imax*Vmax;
          return(register_val);
        }
        else
        {
          return(NULL);
        }

        break;
    /*Case to find Power Factor
      Range of Power Factor is  -2 : ~2
      */
  case pfactor:
        transmitBuffer[0] = PFACTOR;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
          if(temp/10000000000 == 1){
          //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
          register_val = convertToDecimal(temp%10000000000);
          }
          else{
          register_val = convertToDecimal(temp);
          }
          return(register_val);
        }
        else
        {
          return(NULL);
        }

        break;
    /*Number of samples of current and voltage used for calculations
      Range 0 to 511
      */
  case numpstout:
        transmitBuffer[0] = NUMPSTOUT;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
          register_val = convertToDecimal(temp);
          return(register_val);
        }
        else
        {
          return(NULL);
        }

        break;
    /* To find Instantaneous voltage measurement
     Rangs of vcodes is -1 : ~1
    */
  case vcodes:
        transmitBuffer[0] = VCODES;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
            temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
            register_val = convertToDecimal(temp);
            register_val = register_val*Vmax;
            return(register_val);
        }
        else
        {
          return(NULL);
        }
        break;
    /* To find Instantaneous current measurement
       Rangs of vcodes is -2 : ~2
       */
  case icodes:
        transmitBuffer[0] = ICODES;
        transferStatus = SPI_transfer(spiHandle , &spiTransaction);
        if(transferStatus==TRUE)
        {
          temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
          register_val = convertToDecimal(temp);
          register_val = register_val*Imax;
          return(register_val);
        }
        else
        {
          return(NULL);
        }
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
 * @brief       Initialize the pins
 *
 * @param       ACS71020_type , Chip Select , MOSI Pin of Master, MISO pin of Master, Full Scale Voltage
 *
 * @return      None 
 */
bool ACS71020_Setup(ACS71020_type typ, int CS , int mosi,int miso,float vmax)
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
      break;    
    }
    pinMode(ChS, OUTPUT);
    pinMode(MISO, INPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(SCLK, OUTPUT);

    return(TRUE);
    
}

/*************************************************************************
 * @fn          ACS71020_SPI_init()
 * 
 * @brief       Initializes handle
 * 
 * @param       SPI Handle
 * 
 * @return      True if successful and false id unsuccessful
 */
bool ACS71020_SPI_init(SPI_Handle BusHandle)
{
  spiHandle = BusHandle;
  if(spiHandle == NULL)
  {
    return(FALSE);
  }
  else
  {
    return(TRUE);
  }
  
}