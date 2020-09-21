/*
 * ACS71020.c
 *
 *  Created on: Sep 18, 2020
 *      Author: Aniket Paluskar
 */
#include "ACS71020.h"
#include "math.h"
/* Driver Header file */
#include <ti/drivers/SPI.h>


/*  MACRO :- MSG size which will be recieving from Slave or ACS71020  */
#define MSGSIZE  4
#define READ_COMMAND_ACS71020 1


/*****************************************
 * Local Functions */
static float convertToDecimal(long n);


/*****************************************
 * Local Variables
 * Vmax = Full Scale Voltage
 * Imax = Full Scale Current
 *
 * The Master writes on the MOSI line the 7-bit address of the
   register to be read from or written to.
   The next bit on the MOSI line is the read/write (RW) indicator.
   A high state indicates a Read and a low state indicates a Write.
*/
static uint8_t             transmitBuffer[2] = {0,READ_COMMAND_ACS71020};
static uint8_t             recieveBuffer[MSGSIZE];
static float               Vmax, Imax;
SPI_Handle                 spiHandle;
SPI_Transaction            spiTransaction{spiTransaction.count = MSGSIZE,
                           spiTransaction.txBuf = (void *)transmitBuffer,
                           spiTransaction.rxBuf = (void *)receiveBuffer,
                           };



/*
The Master writes on the MOSI line the 7-bit address of the
register to be read from or written to.
The next bit on the MOSI line is the read/write (RW) indicator.
A high state indicates a Read and a low state indicates a Write.

*/



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


/*************************************************************************
 * @fn          ACS71020_SPI_init()
 *
 * @brief       Initializes handle
 *
 * @param       SPI Handle
 *
 * @return      True if successful and false if unsuccessful
 */
bool ACS71020_SPI_init(ACS71020_type type,SPI_Handle BusHandle,float vmax)
{
    switch(type){                                // Imax is defined by model name
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
    Vmax = vmax;
  spiHandle = BusHandle;
  if(spiHandle == NULL)
  {
    return(false);
  }
  else
  {
    return(true);
  }
}


/*************************************************************
 * @fn          ACS71020_getIrms()
 *
 * @brief       Fetches Effective Current
 *
 * @param       None
 *
 * @return      Float Value
 */
float ACS71020_getIrms()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = VRMS_IRMS_ADDRESS;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
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
}


/*************************************************************
 * @fn          ACS71020_getVrms()
 *
 * @brief       Fetches Effective Voltage
 *
 * @param       None
 *
 * @return      Float Value
 */
float ACS71020_getVrms()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = VRMS_IRMS_ADDRESS;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
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
}


/*************************************************************
 * @fn          ACS71020_getPactive()
 *
 * @brief       Fetches Active Power
 *
 * @param       None
 *
 * @return      Float Value
 */
float ACS71020_getPactive()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = PACTIVE;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
  {
      temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
      if(temp/100000000000000000 == 1)
      {
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
}


/*************************************************************
 * @fn          ACS71020_getPapparent()
 *
 * @brief       Fetches Apparent Power
 *
 * @param       None
 *
 * @return      Float Value
 */
float ACS71020_getPapparent()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = PAPPARANT;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
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
}


/*************************************************************
 * @fn          ACS71020_getPreactive()
 *
 * @brief       Fetches Reactive Power
 *
 * @param       None
 *
 * @return      Float Value
 */
float ACS71020_getPreactive()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = PREACTIVE;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
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

}


/********************************************************************
 * @fn          ACS71020_getPfactor()
 *
 * @brief       Fetches power factor
 *
 * @param       None
 *
 * @returns     Float Value
 */
float ACS71020_getPfactor()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = PFACTOR;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
  {
    temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
    if(temp/10000000000 == 1)
    {
    //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
      register_val = convertToDecimal(temp%10000000000);
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
}


/*******************************************************************************
 * @fn          ACS71020_getNumpstout()
 *
 * @brief       NUmber of Samples used for calculation
 *
 * @param       None
 *
 * @return      Float Value
 *
 */
float ACS71020_getNumpstout()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = NUMPSTOUT;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
  {
    temp = recieveBuffer[3] | (recieveBuffer[2]<<8);
    register_val = convertToDecimal(temp);
    return(register_val);
  }
  else
  {
    return(NULL);
  }
}


/*********************************************************************
 * @fn          ACS71020_getIcodes()
 *
 * @brief       Instantaneous Voltage Measurement
 *
 * @param       None
 *
 * @returns    float
 */
float ACS71020_getVcodes()
{
  long temp;
  float register_val;
  bool transferStatus = false;
   transmitBuffer[0] = VCODES;
   transferStatus = SPI_transfer(spiHandle , &spiTransaction);
   if(transferStatus==true)
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

}


/*********************************************************************
 * @fn          ACS71020_getIcodes()
 *
 * @brief       Instantaneous Current Measurement
 *
 * @param       None
 *
 * @returns     float
 */
float ACS71020_getIcodes()
{
  long temp;
  float register_val;
  bool transferStatus = false;
  transmitBuffer[0] = ICODES;
  transferStatus = SPI_transfer(spiHandle , &spiTransaction);
  if(transferStatus==true)
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
}
