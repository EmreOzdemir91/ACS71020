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
static bool checkSPIhandle();


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
SPI_Transaction            spiTransaction;



/*************************************************************************
 * @fn          ACS71020_SPI_init()
 *
 * @brief       Initializes handle
 *
 * @param       ACS71020_type, SPI Handle, Vmax(Full Scale Voltage
 *
 * @return      True if successful and false if unsuccessful
 */
bool ACS71020_SPI_init(ACS71020_type type,SPI_Handle BusHandle,float vmax)
{
    spiTransaction.count = 4;
    spiTransaction.txBuf = (void *)transmitBuffer;
    spiTransaction.rxBuf = (void *)recieveBuffer;
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val= recieveBuffer[1] | (recieveBuffer[0]<<8);
      register_val = register_val * Imax;
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else
  {
    return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val = recieveBuffer[3] | (recieveBuffer[2]<<8);
      register_val = register_val*Vmax;
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else{
    return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = PACTIVE;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        register_val = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = register_val * Vmax * Imax;
        return(register_val);
    }
    else
    {
        return(NULL);
    }

  }
  else
  {
      return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = PAPPARANT;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        register_val= recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = register_val*Imax*Vmax;
        return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else{
    return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = PREACTIVE;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val = recieveBuffer[3] | (recieveBuffer[2]<<8);
      register_val = register_val*Imax*Vmax;
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else
  {
    return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = PFACTOR;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val= recieveBuffer[3] | (recieveBuffer[2]<<8);
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else{
    return(0);
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
  if (checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = NUMPSTOUT;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val= recieveBuffer[3] | (recieveBuffer[2]<<8);
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else{
    return(0);
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
  if(checkSPIhandle()==true){
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = VCODES;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        register_val = recieveBuffer[3] | (recieveBuffer[2]<<8);
        register_val = register_val*Vmax;
        return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else{
    return(0);
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
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = ICODES;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      register_val = recieveBuffer[3] | (recieveBuffer[2]<<8);
      register_val = register_val*Imax;
      return(register_val);
    }
    else
    {
      return(NULL);
    }
  }
  else
  {
    return(0);
  }
}


/********************************************************
 * @fn      checkSPIhandle()
 *
 * @brief   Checks if SPI handle is present or not
 *
 * @param   None
 *
 * @return  true if present, false if not present
 */

bool checkSPIhandle()
{
  if(spiHandle != NULL)
  {
    return true;
  }
  else
  {
    return false;
  }

}
