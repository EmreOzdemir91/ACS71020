/*
 * ACS71020.c
 *
 *  Created on: Sep 18, 2020
 *      Author: Aniket Paluskar
 */
#include "ACS71020.h"
#include "math.h"

/*  MACRO :- MSG size which will be recieving from Slave or ACS71020  */
#define MSGSIZE 4
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
static uint8_t transmitBuffer[2] = {0, READ_COMMAND_ACS71020};
static uint8_t recieveBuffer[MSGSIZE];
static float Vmax, Imax;
SPI_Handle spiHandle;
SPI_Transaction spiTransaction;

/*************************************************************************
 * @fn          ACS71020_SPI_init()
 *
 * @brief       Initializes handle
 *
 * @param       ACS71020_type, SPI Handle, Vmax(Full Scale Voltage)
 *
 * @return      True if successful and false if unsuccessful
 */
bool ACS71020_SPI_init(ACS71020_type type, SPI_Handle BusHandle, float vmax)
{
  spiTransaction.count = 4;
  spiTransaction.txBuf = (void *)transmitBuffer;
  spiTransaction.rxBuf = (void *)recieveBuffer;
  switch (type)
  { // Imax is defined by model name
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
  if (spiHandle == NULL)
  {
    return (false);
  }
  else
  {
    return (true);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b01111111;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[3] & normalizing_Number;
      tempRegister_val = tempRegister_val << 8 | recieveBuffer[2];
      register_val = tempRegister_val / pow(2/*
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
 * @param1      ACS71020_type -> ACS71020_15A or ACS71020_30A
 *
 * @param2      SPI_Handle -> SPI Handle which s returned when SPI_open() is called
 *
 * @param3      vmax -> Full scale Voltage
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
 * @return      Float
 */
float ACS71020_getIrms()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b01111111;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      tempRegister_val = recieveBuffer[3] & normalizing_Number;
      tempRegister_val = tempRegister_val<<8 | recieveBuffer[2];
      register_val = tempRegister_val;
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
 * @return      Float
 */
float ACS71020_getVrms()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b01111111;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      tempRegister_val = recieveBuffer[1] & normalizing_Number;
      tempRegister_val = tempRegister_val<<8 | recieveBuffer[0];
      register_val = tempRegister_val;
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
 * @return      Float
 */
float ACS71020_getPactive()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    uint32_t tempRegister_val;
    uint8_t normalizing_Number = 0b00000001;
    transmitBuffer[0] = PACTIVE;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        tempRegister_val = recieveBuffer[2] & normalizing_Number;
        if (tempRegister_val == 0b1)
        {
            tempRegister_val = recieveBuffer[1] <<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2,15)) * Vmax * Imax * -1;
            return(register_val);
        }
        else
        {
            tempRegister_val = recieveBuffer[1] <<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2,15)) * Vmax * Imax;
            return(register_val);
        }
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
 * @return      Float
 */
float ACS71020_getPapparent()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    uint16_t tempRegister_val;
    bool transferStatus = false;
    transmitBuffer[0] = PAPPARANT;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        tempRegister_val = recieveBuffer[1]<<8 | recieveBuffer[0];
        register_val = (tempRegister_val/pow(2,15))*Imax*Vmax;
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
 * @return      Float
 */
float ACS71020_getPreactive()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    uint16_t tempRegister_val;
    bool transferStatus = false;
    transmitBuffer[0] = PREACTIVE;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
      tempRegister_val = recieveBuffer[1]<<8 | recieveBuffer[0];
      tempRegister_val = tempRegister_val/pow(2,15);
      register_val = tempRegister_val*Imax*Vmax;
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
 * @returns     Float
 */
float ACS71020_getPfactor()
{
  if(checkSPIhandle()==true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number= 0b00000111, negativeChecker =0b00000100;
    transmitBuffer[0] = PFACTOR;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        tempRegister_val = recieveBuffer[1] & normalizing_Number;
        if(tempRegister_val & negativeChecker == negativeChecker)
        {
            tempRegister_val = tempRegister_val & 0b00000011;
            tempRegister_val = tempRegister_val <<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2,9))*-1;
        }
        else
        {
            tempRegister_val = tempRegister_val & 0b00000011;
            tempRegister_val = tempRegister_val <<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2,9));
        }

        return register_val;
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
 * @return      16 bit integer
 *
 */
uint16_t ACS71020_getNumpstout()
{
  if (checkSPIhandle()==true)
  {
    uint16_t register_val;
    uint8_t normalizing_Number = 0b00000001;
    bool transferStatus = false;
    transmitBuffer[0] = NUMPSTOUT;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        /*
         * normalizing_Number is used to get the correct bit and making other bits as zero.
         */
      register_val= recieveBuffer[1] & normalizing_Number;
      register_val= register_val<<8 | recieveBuffer[0];
      return(register_val);
    }
    else
    {
      return(21); //Testing Purpose
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
    uint8_t normalizing_Number = 0b00000001;
    uint16_t tempRegister_val;
    transmitBuffer[0] = VCODES;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        tempRegister_val = recieveBuffer[2] & normalizing_Number;

        /*
         * If bit no. 17 = 0b1(1 in decimal), then output is negative
         */
        if(tempRegister_val == 1){
            tempRegister_val = tempRegister_val <<18 | recieveBuffer[1]<<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2, 16))*Vmax*-1;
        }
        else{
            tempRegister_val = tempRegister_val <<18 | recieveBuffer[1]<<8 | recieveBuffer[0];
            register_val = (tempRegister_val/pow(2, 16))*Vmax;
        }
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
    uint8_t normalizing_Number = 0b00000001;
    uint16_t tempRegister_val;
    transferStatus = SPI_transfer(spiHandle , &spiTransaction);
    if(transferStatus==true)
    {
        tempRegister_val = recieveBuffer[2] & normalizing_Number;

                /*
                 * If bit no. 17 = 0b1(1 in decimal), then output is negative
                 */
                if(tempRegister_val == 1){
                    tempRegister_val = recieveBuffer[1]<<8 | recieveBuffer[0];
                    register_val = (tempRegister_val/pow(2, 16))*Imax*-1;
                }
                else{
                    tempRegister_val = recieveBuffer[1]<<8 | recieveBuffer[0];
                    register_val = (tempRegister_val/pow(2, 16))*Imax;
                }
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
, 14);
      register_val = register_val * Imax;
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b01111111;
    transmitBuffer[0] = VRMS_IRMS_ADDRESS;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[1] & normalizing_Number;
      tempRegister_val = tempRegister_val << 8 | recieveBuffer[0];
      register_val = tempRegister_val / pow(2, 15);
      register_val = register_val * Vmax;
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b00000001, negativeChecker = 0b00000001;
    transmitBuffer[0] = PACTIVE;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = transmitBuffer[2] & negativeChecker;
      if (tempRegister_val == negativeChecker)
      {
        tempRegister_val = transmitBuffer[1] << 8 | transmitBuffer[0]; 
        register_val = (tempRegister_val / pow(2, 15)) * Vmax * Imax * -1;
        return (register_val );
      }
      else
      {
        tempRegister_val = transmitBuffer[1] << 8 | transmitBuffer[0]; 
        register_val = (tempRegister_val / pow(2, 15)) * Vmax * Imax;3750
        return (register_val );
      }
      

    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    uint16_t tempRegister_val;
    bool transferStatus = false;
    transmitBuffer[0] = PAPPARANT;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[1] << 8 | recieveBuffer[0];
      register_val = (tempRegister_val / pow(2, 15)) * Imax * Vmax;
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    uint16_t tempRegister_val;
    bool transferStatus = false;
    transmitBuffer[0] = PREACTIVE;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[1] << 8 | recieveBuffer[0];
      tempRegister_val = tempRegister_val / pow(2, 15);
      register_val = tempRegister_val * Imax * Vmax;
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    uint16_t tempRegister_val;
    uint8_t normalizing_Number = 0b00000111, negativeChecker = 0b00000100;
    transmitBuffer[0] = PFACTOR;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[1] & normalizing_Number;
      if (tempRegister_val & negativeChecker == negativeChecker)
      {
        tempRegister_val = tempRegister_val << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 9)) * -1;
      }
      else
      {
        tempRegister_val = tempRegister_val << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 9));
      }

      return register_val;
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
  }
}

/*******************************************************************************
 * @fn          ACS71020_getNumpstout()
 *
 * @brief       NUmber of Samples used for calculation
 *
 * @param       None
 *
 * @return      16 bit integer
 *
 */
uint16_t ACS71020_getNumpstout()
{
  if (checkSPIhandle() == true)
  {
    uint16_t register_val;
    uint8_t normalizing_Number = 0b00000001;
    bool transferStatus = false;
    transmitBuffer[0] = NUMPSTOUT;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      /*
         * normalizing_Number is used to get the correct bit and making other bits as zero.
         */
      register_val = recieveBuffer[1] & normalizing_Number;
      register_val = register_val << 8 | recieveBuffer[0];
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    uint8_t normalizing_Number = 0b00000001;
    uint16_t tempRegister_val;
    transmitBuffer[0] = VCODES;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[2] & normalizing_Number;

      /*
         * If bit no. 17 = 0b1(1 in decimal), then output is negative
         */
      if (tempRegister_val == 1)
      {
        tempRegister_val = tempRegister_val << 18 | recieveBuffer[1] << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 16)) * Vmax * -1;
      }
      else
      {
        tempRegister_val = tempRegister_val << 18 | recieveBuffer[1] << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 16)) * Vmax;
      }
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (checkSPIhandle() == true)
  {
    float register_val;
    bool transferStatus = false;
    transmitBuffer[0] = ICODES;
    uint8_t normalizing_Number = 0b00000001;
    uint16_t tempRegister_val;
    transferStatus = SPI_transfer(spiHandle, &spiTransaction);
    if (transferStatus == true)
    {
      tempRegister_val = recieveBuffer[2] & normalizing_Number;

      /*
                 * If bit no. 17 = 0b1(1 in decimal), then output is negative
                 */
      if (tempRegister_val == 1)
      {
        tempRegister_val = recieveBuffer[1] << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 16)) * Vmax * -1;
      }
      else
      {
        tempRegister_val = recieveBuffer[1] << 8 | recieveBuffer[0];
        register_val = (tempRegister_val / pow(2, 16)) * Vmax;
      }
      return (register_val);
    }
    else
    {
      return (NULL);
    }
  }
  else
  {
    return (0);
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
  if (spiHandle != NULL)
  {
    return true;
  }
  else
  {
    return false;
  }
}
