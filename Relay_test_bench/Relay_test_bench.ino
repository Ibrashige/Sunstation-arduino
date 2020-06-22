#include <SPI.h>
#include <SD.h>

int offset_1 = 516; // not used
int offset_2 = 513; //not used
int offset_3 = 519; 
int enable_pin=3;     //Relay signal pin
int readIndex = 0;              // the index of the current reading
int total = 0;                  // the running total
int period = 30000;
unsigned long time_now = 0;
float averageamperage = 0;
const int numReadings = 10;   // value to determine the size of the readings array.
double currentcharge = 2880;
double average = 0;                // the average
const float vpp = 0.00458984375;    //5V/1024 multiplier
double readings[numReadings];      // the readings from the analog input
int points_one;     //variable for retrieving analog input from A0
int points_two;     //variable for retrieving analog input from A3                   
int points_three;   //variable for retrieving analog input from A4
int points;       //variable for retrieving analog input from A4 to calculate the average current of the phone charging battery
int interval;
float voltage;
float voltage_one;  
float voltage_two;
float voltage_three;
float amperage_battery1;   //variable to return battery current from A0
float amperage_battery2;   //variable to return battery current from A3
float amperage_solar;    //variable to return solar current from A4

File myFile;

void setup() {
  Serial.begin(9600);
  pinMode(enable_pin, LOW); // sets the digital pin 4 as output
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

  for (int thisReading = 0; thisReading < numReadings; thisReading++) 
  {
    readings[thisReading] = 0;
  }
}

void loop() {
  interval = 1; //interval to write data
  
  average_amperage();//Calculate Amperage Average
 
  batterycharge();

  
//  points_one = analogRead(A1)- offset_1;
//  voltage_one = points_one * vpp;
//  amperage_battery1 = voltage_one / 0.136;
//  Serial.println(points_one);
//
//  points_two = analogRead(A2)- offset_2;
//  voltage_two = points_two * vpp;
//  amperage_solar = voltage_two / 0.136;
//  Serial.println(points_two);
  
 float temp = calculatecurrent();
  
  if(currentcharge<300)
  {
    digitalWrite(enable_pin,HIGH);
  }
  else
  {
    digitalWrite(enable_pin,LOW);
  }
  
  write_data(amperage_battery2);  //write data
}

void average_amperage()
{
   // subtract the last reading:
  total = total - readings[readIndex];
  // read from the sensor:
  readings[readIndex] = analogRead(A0);
  // add the reading to the total:
  total = total + readings[readIndex];
  // advance to the next position in the array:
  readIndex = readIndex + 1;
 
  // if we're at the end of the array...
  if (readIndex >= numReadings) {
    // ...wrap around to the beginning:
    readIndex = 0;
  }
 
  // calculate the average:
  average = total / numReadings;
 
 
  //
  points = average - offset_3;
  voltage = points * vpp;
  averageamperage = (voltage / 0.136);
 
  // send it to the computer as ASCII digits
  Serial.print("Average:");
  Serial.println(averageamperage);
}

void batterycharge()
{
  currentcharge = currentcharge + (averageamperage*0.1);
  if (currentcharge < 10 ){
    currentcharge = 10;
  }
 
  if (currentcharge > 20000){
    currentcharge = 20000;
  }
 
Serial.print("Current Charge:");
Serial.println(currentcharge);
 
}

float calculatecurrent(){
  points_three = analogRead(A0)- offset_3;
  voltage_three = points_three * vpp;
  amperage_battery2 = voltage_three / 0.136;
  Serial.println(amperage_battery2);
  delay(750);
  return amperage_battery2;
}

void write_data(float amperage_three)
{
  myFile = SD.open("test.csv", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
//    myFile.print("The first battery current is:,");
//    myFile.println(amperage_one);
//    myFile.print("The solar current is:, ");
//    myFile.println(amperage_two);
    myFile.print("The second battery current is:, ");
    myFile.println(amperage_three);
    myFile.println("---------------------------");
    myFile.close();
    delay(30000);
    //Serial.println("Write file successful!"); //print out COM Port
  } else {
   //Serial.println("error opening test.txt");
  }
  
}
