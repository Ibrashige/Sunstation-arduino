
/*
Connect the following pins to your Arduino:
VIO to VCC
GND to GND
INT to D3
POL to D4
CLR to D6
Note that if you solder headers to the bottom of the board,
you can plug the breakout board directly into Arduino header
pins D2 (VIO) through D7 (SHDN).
RUNNING THE SKETCH:
This sketch monitors current moving into and out of a battery.
Whenever it detects a low INT signal from the LTC4150, it will
clear INT by making the CLR pin low, update the battery state-
of-charge (how full the battery is), current draw, etc.
The sketch is hardcoded for a 2000mAh battery that is 100% full
when the sketch starts. You can easily change this by editing
the following lines:
  double battery_mAh = 2000.0; // milliamp-hours (mAh)
  double battery_percent = 100.0;  // state-of-charge (percent)
After uploading the sketch, open the Serial Monitor and set the
baud rate to 9600. Whenever the sketch detects an INT pulse, it
will update its calculations and print them out.
*/

// For this sketch you only need the first five of the 
// following pins, but you can plug the board directly
// into the Arduino header (D2-D7) for convenience.

// (If you are not plugging the board directly into the
// header, you can remove all references to VIO, GND,
// and SHDN.)

#define VIO 2 // Just used for the HIGH reference voltage
#define INT 3
#define POL 4
#define GND 5 // Just used for the LOW reference voltage
#define CLR 6
#define SHDN 7 // Unneeded in this sketch, set to input

#define LED 13 // Standard Arduino LED

// Change the following two lines to match your battery
// and its initial state-of-charge:

double battery_mAh = 150.0; // milliamp-hours (mAh)
double battery_percent = 100.0;  // state-of-charge (percent)

// Global variables:

double ah_quanta = 0.17067759; // mAh for each INT
double percent_quanta; // calculate below

void setup()
{
  // Set up I/O pins:
  
  pinMode(GND,OUTPUT);
  digitalWrite(GND,LOW);

  pinMode(VIO,OUTPUT);
  digitalWrite(VIO,HIGH);

  pinMode(INT,INPUT);

  pinMode(POL,INPUT);

  pinMode(CLR,OUTPUT);
  digitalWrite(CLR,HIGH);
  
  pinMode(SHDN,INPUT); // Unneeded, disabled by setting to input

  pinMode(LED,OUTPUT);
  digitalWrite(LED,LOW);  

  // Enable serial output:

  Serial.begin(9600);
  Serial.println("LTC4150 Coulomb Counter BOB no-interrupt example");

  // One INT is this many percent of battery capacity:
  
  percent_quanta = 1.0/(battery_mAh/1000.0*5859.0/100.0);
}

void loop()
{
  static long int time, lasttime;
  double mA;
  boolean polarity;

  if (digitalRead(INT)==0) // INT has gone low
  {
    // Determine delay since last interrupt (for mA calculation)
    // Note that first interrupt will be incorrect (no previous time!)

    lasttime = time;
    time = micros();

    // Get the polarity value
    
    polarity = digitalRead(POL);
    if (polarity) // high = charging
    {
      battery_mAh += ah_quanta;
      battery_percent += percent_quanta;
    }
    else // low = discharging
    {
      battery_mAh -= ah_quanta;
      battery_percent -= percent_quanta;
    }

    // Calculate mA from time delay (optional)
  
    mA = 614.4/((time-lasttime)/1000000.0);
  
    // If charging, we'll set mA negative (optional)
    
    if (polarity) mA = mA * -1.0;

    // Clear the interrupt signal

    digitalWrite(CLR,LOW);
    delayMicroseconds(40); // CLR needs to be low > 20us
    digitalWrite(CLR,HIGH);

    // Blink the LED (optional)
    
    digitalWrite(LED,HIGH);
    delay(100);
    digitalWrite(LED,LOW);

    // Print out the current battery status

    Serial.print("mAh: ");
    Serial.print(battery_mAh);
    Serial.print(" soc: ");
    Serial.print(battery_percent);
    Serial.print("% time: ");
    Serial.print((time-lasttime)/1000000.0);
    Serial.print("s mA: ");
    Serial.println(mA);

   delay(500);
  }
}
