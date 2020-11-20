//https://forum.arduino.cc/index.php?topic=711928.
#include <Arduino.h>
#include <SPI.h>
#include <SD.h>
File outputFile;
File inputFile;
File latestFile;

long readVcc()
{ long result;
  // Read 1.1V reference against AVcc
  ADMUX = _BV(REFS0) | _BV(MUX3) | _BV(MUX2) | _BV(MUX1);
  delay(2); // Wait for Vref to settle
  ADCSRA |= _BV(ADSC); // Convert
  while (bit_is_set(ADCSRA, ADSC));
  result = ADCL;
  result |= ADCH << 8;
  result = 1126400L / result; // Back-calculate AVcc in mV
  return result;
}

int points = 0;
float voltage;
double averageamperage = 0;
double totalPowerProduced;
double totalAmps = 0;
double averageAmps;
double powerProduced;

double lastsavedvalue = 0;
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
double average = 0;                // the average
int inputPin = A0;
unsigned long previousMillis = 0;        // will store last time currentcharge was updated
unsigned long previousMs = 0;        // will store last time total power was updated
const long interval = 300;           // interval at which to blink (milliseconds) 
 
void saveToFile();
void readFromFile();
void saveLatest();

uint8_t latestRecord;

void setup()
{
  randomSeed(analogRead(A0));
  Serial.begin(115200);
  Serial.print("Initializing SD card...");
  if (!SD.begin(10))
  {
    Serial.println("initialization failed!");
    while (true)
      ;
  }
  Serial.println("initialization done.");
   readFromFile();
  for (int x = 0; x < 8; x++)
  {
        unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= interval) {
    // save the last time you blinked the LED
    previousMillis = currentMillis;
    //Calculate Amperage Average
    average_amperage();
    // Calculate Total Amperage
    total_amperage();
  }
   if (currentMillis - previousMs >= 15000) {
    previousMs = currentMillis;
  averageAmps = totalAmps / (45);
   }
    saveToFile();
    saveLatest();
  }
 
}

void loop()
{

}

void average_amperage()
{ //
  points = analogRead(inputPin);
  voltage = ((points / 1023.0) * readVcc()) ;
  averageamperage = (( voltage / readVcc()) * 36.7) - 18.3 ;

  if (averageamperage > 0 && averageamperage < 0.11)
  {
    averageamperage = 0;
  }
  else if (averageamperage < 0 && averageamperage > -0.11)
  {
    averageamperage = 0;
  }
  // send it to the computer as ASCII digits
  Serial.print("Average:");
  Serial.println(averageAmps);
}

void total_amperage()
{
  totalAmps = totalAmps + averageamperage;
}

void saveLatest()
{
  SD.remove("latest.txt");
  latestFile = SD.open("latest.txt", FILE_WRITE);
  if (latestFile)
  {
    Serial.print("Writing to latest.txt...");
    latestFile.println(latestRecord);
    latestFile.close();
    Serial.print(latestRecord);
    Serial.println(" saved to latest.txt");
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening latest.txt");
    while (true)
      ;
  }
}

void saveToFile()
{

  outputFile = SD.open("test.txt", FILE_WRITE);
  if (outputFile)
  {
    Serial.print("Writing to test.txt...");
    powerProduced = 1.3 * 3.7;
    latestRecord = latestRecord + powerProduced;
    outputFile.println(latestRecord);
    outputFile.close();
    Serial.println("done.");
  }
  else
  {
    Serial.println("error opening test.txt");
    while (true)
      ;
  }
}

void readFromFile()
{
  inputFile = SD.open("test.txt");
  if (inputFile)
  {
    Serial.println("test.txt:");
    while (inputFile.available())
    {
      Serial.write(inputFile.read());
    }
    inputFile.close();
  }
  else
  {
    Serial.println("error opening test.txt");
  }
}
