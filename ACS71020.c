#include<ACS71020.h>
#include<SPI.h>


// Static Function Declaration
static void ConnectbySPI();
static void DisconnectbySPI();
static float convertToDecimal(long n); 
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
        ConnectbySPI();
        temp = SPI.transfer(VRMS_IRMS_ADDRESS);
        temp = temp >> 16;
        register_val = convertToDecimal(temp);

        register_val = register_val * Imax;
        DisconnectbySPI();
        return(register_val);
        break;
    /*Case to find Effective Voltage
      Range of Vrms ouput is 0 : ~1
        */ 
  case vrms:
        ConnectbySPI();
        temp = SPI.transfer(VRMS_IRMS_ADDRESS);
        temp = temp << 16;
        register_val = convertToDecimal(temp);

        register_val = temp*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    /* Case to find Active Power
        Range of Vrms ouput is -2 : ~2
        */ 
  case pactive:
        ConnectbySPI();
        temp = SPI.transfer(PACTIVE);
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
        DisconnectbySPI();
        return(register_val);
        break;
    /* Case to find Apparent Power
        Range of output Apparent power is 0 : ~2
     */
  case papparant:
        ConnectbySPI();
        temp = SPI.transfer(PAPPARANT);
        register_val = convertToDecimal(temp);
        
        register_val = temp*Imax*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    /*case to find reactive power
      Range of reactive power is 0 : ~2
      */
  case preactive:
        ConnectbySPI();
        temp = SPI.transfer(PREACTIVE);
        register_val = convertToDecimal(temp);
        register_val = temp*Imax*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    /*Case to find Power Factor
      Range of Power Factor is  -2 : ~2
      */
  case pfactor:
        ConnectbySPI();
        temp = SPI.transfer(PFACTOR);
        if(temp/10000000000 == 1){
        //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
        register_val = convertToDecimal(temp%10000000000);
        }
        else{
        register_val = convertToDecimal(temp);
        }
        DisconnectbySPI();
        return(register_val);
        break;
    /*Number of samples of current and voltage used for calculations
      Range 0 to 511
      */
  case numpstout:
        ConnectbySPI();
        temp = SPI.transfer(NUMPSTOUT);
        register_val = convertToDecimal(temp);
        DisconnectbySPI();
        return(register_val);
        break;
    /* To find Instantaneous voltage measurement
     Rangs of vcodes is -1 : ~1
    */
  case vcodes:
        ConnectbySPI();
        temp = SPI.transfer(VCODES);
        register_val = convertToDecimal(temp);
        register_val = register_val*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    /* To find Instantaneous current measurement
       Rangs of vcodes is -2 : ~2
       */
  case icodes:
        ConnectbySPI();
        temp = SPI.transfer(ICODES);
        register_val = convertToDecimal(temp);
        register_val = register_val*Imax;
        DisconnectbySPI();
        return(register_val);
        break;
  
  default:
      break;
  }
     
}
/********************************************************************
 * @fn          ACS71020_ConnectbySPI()
 *
 * @brief       Connects to ACS71020 using SPI protocol
 *
 * @param       None
 *
 * @return      None
 */
void ACS71020_ConnectbySPI()
{
  pinMode(ChS, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  digitalWrite(ChS, LOW);
}
/********************************************************************
 * @fn          ACS71020_ConnectbySPI()
 *
 * @brief       Disconnects ACS71020 with MCU
 *
 * @param       None
 *
 * @return      None
 */
void ACS71020_DisconnectbySPI()
{
  digitalWrite(ChS, HIGH);
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
 * @fn          convertToDecimal(long n)
 *
 * @brief       Converts output given by ACS71020 (Binary) to decimal
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
      Imax = 30.00    
    }
    pinMode(ChS, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    
}