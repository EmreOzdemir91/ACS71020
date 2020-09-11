#include<ACS71020.h>
#include<SPI.h>
#include<Arduino.h>


// Counstrustor of ACS71020 Class
ACS71020::ACS71020(ACS71020_type type, int CS , int mosi,int miso,int sclk,float vmax){
  switch(type){
    case ACS71020_15A:
      Imax = 15.00;
      break;
    case ACS71020_30A:
      Imax = 30.00;
      break;    
    }
    ChS = CS;
    MOSI = mosi;
    MISO = miso;
    SCLK = sclk;
    Vmax = vmax;
}



// This function takes a parameter of the value user is reading for and returns the value in float.
float ACS71020::getvalue(value_type typ){
  long temp;
  float register_val;

  switch (typ)
  {
    //Case to find Effective Current
  case irms:
        ConnectbySPI();
        temp = SPI.transfer(0x20);
        temp = temp >> 16;
        register_val = convertToDecimal(temp);

        register_val = register_val * Imax;
        DisconnectbySPI();
        return(register_val);
        break;
    //Case to find Effective Voltage
  case vrms:
        ConnectbySPI();
        temp = SPI.transfer(0x20);
        temp = temp << 16;
        register_val = convertToDecimal(temp);

        register_val = temp*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    // Case to find Active Power
  case pactive:
        ConnectbySPI();
        temp = SPI.transfer(0x21);
        if(temp/100000000000000000 == 1){
        //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
        // Range of Active power is -2 : ~2
        register_val = convertToDecimal(temp%100000000000000000);
        }
        else
        {
          register_val = convertToDecimal(temp);
        }
        DisconnectbySPI();
        return(register_val);
        break;
    // Case to find Apparent Power
    // Range of output Apparent power is 0 : ~2
  case papparant:
        ConnectbySPI();
        temp = SPI.transfer(0x22);
        register_val = convertToDecimal(temp);
        
        register_val = temp*Imax*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    //case to find reactive power
    //Range of reactive power is 0 : ~2
  case preactive:
        ConnectbySPI();
        temp = SPI.transfer(0x23);
        register_val = convertToDecimal(temp);
        register_val = temp*Imax*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    //Case to find Power Factor
    //Range of Power Factor is  -2 : ~2
  case pfactor:
        ConnectbySPI();
        temp = SPI.transfer(0x24);
        if(temp/10000000000 == 1){
        //To see if value is positive or negative value is divided by 10^16 as output is 17bit long
        // Range of Active power is -2 : ~2
        register_val = convertToDecimal(temp%10000000000);
        }
        else{
        register_val = convertToDecimal(temp);
        }
        DisconnectbySPI();
        return(register_val);
        break;
    //Number of samples of current and voltage used for calculations
    // Range 0 to 511
  case numpstout:
        ConnectbySPI();
        temp = SPI.transfer(0x25);
        register_val = convertToDecimal(temp);
        DisconnectbySPI();
        return(register_val);
        break;
    // To find Instantaneous voltage measurement
    // Rangs of vcodes is -1 : ~1
  case vcodes:
        ConnectbySPI();
        temp = SPI.transfer(0x2A);
        register_val = convertToDecimal(temp);
        register_val = register_val*Vmax;
        DisconnectbySPI();
        return(register_val);
        break;
    // To find Instantaneous current measurement
    // Rangs of vcodes is -2 : ~2
  case icodes:
        ConnectbySPI();
        temp = SPI.transfer(0x2B);
        register_val = convertToDecimal(temp);
        register_val = register_val*Imax;
        DisconnectbySPI();
        return(register_val);
        break;
  
  default:
      break;
  }
     
}
// This functions connects to ACS71020 by using SPI for data transmission
void ACS71020::ConnectbySPI()
{
  pinMode(ChS, OUTPUT);
  SPI.begin();
  SPI.setBitOrder(MSBFIRST);
  SPI.setDataMode(SPI_MODE0);
  digitalWrite(ChS, LOW);
}
// This functions disconnects to ACS71020 by using SPI after the data transmission
void ACS71020::DisconnectbySPI()
{
  digitalWrite(ChS, HIGH);
}
//As values returned from SPI are binary, function coverts binary to decimal.
float ACS71020::convertToDecimal(long n) {
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

// Function is called to setup Pinmodes
void ACS71020::setup()
{
    pinMode(ChS, OUTPUT);
    pinMode(MOSI, OUTPUT);
    pinMode(MISO, INPUT);
    
}