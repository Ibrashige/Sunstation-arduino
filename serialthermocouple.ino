#include <SD.h>

File myFile;

#define TC_PIN A0          // set to ADC pin used
#define AREF 5           // set to AREF, typically board voltage like 3.3 or 5.0
#define ADC_RESOLUTION 10  // set to ADC bit resolution, 10 is default
 
float reading, voltage, temperature;
 
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

  reading = analogRead(TC_PIN);
  voltage = get_voltage(reading);
  temperature = get_temperature(voltage);
  Serial.print("Temperature = ");
  Serial.print(temperature);
  Serial.println(" C");
  delay(500);
  
  write_data(temperature);  //write data
  delay(60000);
}

void write_data(int temp_input)
{
  myFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print("The Temperature is: ");
    myFile.println(temp_input);
    myFile.close();
    Serial.println("Write file successful!"); //print out COM Port
  } else {
    Serial.println("error opening test.txt");
  }
}
