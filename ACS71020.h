#ifndef ACS71020_h
#define ACS71020_h
#include<Arduino.h>


enum ACS71020_type { ACS71020_15A,
                      ACS71020_30A
                  }; // Types of ACS71020
enum value_type {irms,
 vrms,
 pactive,
 papparant,
 preactive,
 pfactor,
 numpstout,
 vcodes,
 icodes
       }; // Types of ACS71020 Values

class ACS71020 {
  public:
    int ChS, MOSI, MISO,SCLK;
    float Vmax, Imax;
    ACS71020(ACS71020_type type, int CS , int mosi, int miso, int sclk, float vmax);
    float getvalue(value_type value);
    void setup();
  private:
    void ConnectbySPI();
    void DisconnectbySPI();
    float convertToDecimal(long n);
};




#endif