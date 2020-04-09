#include <SD.h>

File myFile;

#define TC_PIN A0        // set to ADC pin used
#define TD_PIN A1        // set to ADC pin used
#define AREF 5           // set to AREF, typically board voltage like 3.3 or 5.0
#define ADC_RESOLUTION 10  // set to ADC bit resolution, 10 is default
 
float reading1, reading2, voltage1, voltage2, temperature1,temperature2;
 
float get_voltage(int raw_adc) {
  return raw_adc * (AREF / (pow(2, ADC_RESOLUTION)-1));  
}
 
float get_temperature(float voltage) {
  return (voltage - 1.25) / 0.005;
}

void setup() {
  Serial.begin(9600);
  Serial.print("Initializing SD card...");
  if (!SD.begin(10)) {
    Serial.println("initialization failed!");
  }
  else
  {
    Serial.println("initialization done.");
  }
}

void loop() {

  reading1 = analogRead(TC_PIN);
  voltage1 = get_voltage(reading1);
  temperature1 = get_temperature(voltage1);
  reading2 = analogRead(TD_PIN);
  voltage2 = get_voltage(reading2);
  temperature2 = get_temperature(voltage2);
  Serial.print("Temperature1 = ");
  Serial.print(temperature1);
  Serial.println(" C");
  Serial.print("Temperature2 = ");
  Serial.print(temperature2);
  Serial.println(" C");
  delay(500);
  
  write_data(temperature1,temperature2);  //write data
  delay(60000);
}

void write_data(int temp1_input, int temp2_input)
{
  myFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print("The Temperature 1 is: ");
    myFile.println(temp1_input);
    myFile.print("The Temperature 2 is: ");
    myFile.println(temp2_input);
    myFile.println("---------------------------");
    myFile.close();
    Serial.println("Write file successful!"); //print out COM Port
  } else {
    Serial.println("error opening test.txt");
  }
}
