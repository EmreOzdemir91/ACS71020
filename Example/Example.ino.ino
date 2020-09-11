#include <ACS71020.h>

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
}

void loop() {
  // put your main code here, to run repeatedly:
  // To get the energy metering values
  
  int cs = 10, mosi = 11, miso = 12, sclk = 13; // cs = Chip Select, mosi = Master in Slave Out, miso = Master out Slave in, sclk = Sereial Clocl
  float max_voltage = 230.00; // 230 volts of power supply given or Full scale voltage 
  ACS71020 OBJ(ACS71020_30A, cs, mosi, miso, sclk, max_voltage); // Object Created
  Serial.println("object Created");
  OBJ.setup(); // Pinmodes are selected
 
  Serial.println("Irms = ",OBJ.getvalue(irms));
  Serial.println("Vrms = %f", OBJ.getvalue(vrms));
  Serial.println("Active Power = %f", OBJ.getvalue(pactive));
  Serial.println("Apparant Power = %f", OBJ.getvalue(papparant));
  Serial.println("Reactive Power = %f", OBJ.getvalue(preactive));
  Serial.println("Power Factor = %f", OBJ.getvalue(pfactor));
  Serial.println("Number of samples of Current and Voltage for calculation = %f", OBJ.getvalue(numpstout));
  Serial.println("Instantaneous voltage measurement = %f", OBJ.getvalue(vcodes));
  Serial.println("Instantaneous current measurement = %f", OBJ.getvalue(icodes));
 
  delay(100);
}
