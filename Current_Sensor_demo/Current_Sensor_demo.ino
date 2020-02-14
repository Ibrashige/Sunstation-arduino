#include <SPI.h>
#include <SD.h>

int offset = 510;
const float vpp = 0.0048828125;
int points;
int interval;
float voltage;
float amperage;
float amperage_last;

File myFile;

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
  interval = 1; //interval to write data
  points = analogRead(A0)- offset;
  voltage = points * vpp;
  amperage = voltage / 0.185;
  Serial.println(amperage);
  
  write_data(amperage);  //write data
}

void write_data(int amperage_input)
{
  myFile = SD.open("test.txt", FILE_WRITE);

  // if the file opened okay, write to it:
  if (myFile) {
    myFile.print("_amperage_");
    myFile.println(amperage_input);
    myFile.close();
    Serial.println("Write file successful!"); //print out COM Port
  } else {
    Serial.println("error opening test.txt");
  }
}
