int ledPin = 13;  // LED connected to digital pin 8
int inPin = A2;    // pushbutton connected to digital pin A2
int val = 0;      // variable to store the read value

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);  // sets the digital pin 13 as output
  pinMode(inPin, INPUT);    // sets the digital pin A2 as input
}

void loop() {
  val = digitalRead(inPin);   // read the input pin
  digitalWrite(ledPin, val); // sets the LED to the button's value
  Serial.println(val);
  delay(2000);
}
