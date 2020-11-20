#include <SPI.h>
#include <SD.h>

File inputFile;
File outputFile;
File latestFile;
long lastRecordStart;
long fileLength;
long recordCount;

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
// value to determine the size of the readings array.
double lastsavedvalue = 0;
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
double average = 0;                // the average
int inputPin = A0;
unsigned long previousMillis = 0;        // will store last time currentcharge was updated
unsigned long previousMs = 0;        // will store last time total power was updated
const long interval = 300;           // interval at which to blink (milliseconds)
bool flag = false;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }

  delay(2000);  //afer reset Arduino, 2s to take out SD

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
    //Retrieve last saved value from SD card if arduino turns off
    //SD_set();
    Serial.println("Last saved value is:");

    averageAmps = totalAmps / (45);
    total_power_produced();
    Serial.print(lastsavedvalue);
    write_data(totalPowerProduced);  //write data
    write_new_data(totalPowerProduced);
    Serial.println("data written");
    //write_counter();
  }
   totalAmps = 0;
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
  Serial.println(averageamperage);
}

void total_amperage()
{
  totalAmps = totalAmps + averageamperage;
}

void SD_set() {
  // open the file for reading:
  latestFile = SD.open("last.txt");
  if (latestFile) {

    String powerstring = "";
    // read from the file until there's nothing else in it:
    while (latestFile.available()) {
      char power = latestFile.read();
      powerstring = powerstring + power;
    }
    lastsavedvalue = powerstring.toFloat();
    //close the file:
    latestFile.close();
   
  } else {
    // if the file didn't open, print an error:
    Serial.println("error opening test.txt");
  }
}

void total_power_produced()
{
  powerProduced = averageAmps * 3.7;
  latestFile = SD.open("last.txt");
  if (latestFile.available()) {
    latestFile.seek(0);
    lastsavedvalue = latestFile.parseFloat();
    totalPowerProduced = lastsavedvalue;
    totalPowerProduced = totalPowerProduced + powerProduced;
  } //else {
  // create totalPowerProduced
  //  totalPowerProduced = totalPowerProduced + powerProduced;
  //}
  //close the file:
  latestFile.close();
  
}
void write_data(double power_input)
{

  inputFile = SD.open("pow.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (inputFile) {
    inputFile.println(power_input);
    inputFile.close();
    flag = true;
    //Serial.println("Write file successful!"); //print out COM Port
  } else {
    flag = false;
  }
}
void write_new_data(double last_input)
{
   // SD.remove("last.txt");
    
  outputFile = SD.open("last.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (outputFile) {
    outputFile.seek(0);
    outputFile.println(last_input);
    outputFile.close();
    //Serial.println("Write file successful!"); //print out COM Port
  } else {
    //Serial.println("error opening test.txt");
  }
  
}
