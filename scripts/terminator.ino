const int analogOutPin = 9; // Analog output pin that the LED is attached to
int outputValue = 0;
char c = '\0';

void setup() {
  // initialize serial communications at 9600 bps:
  Serial.begin(115200);
}

void loop() {

  if (Serial.available() > 0)
  {
    c = Serial.read();

    if(c == 'i'){
      outputValue = 255;
    }

    if(c == 'o'){
      outputValue = 0;
    }

    Serial.print("\t output = ");
    Serial.println(outputValue);
  }
  
  analogWrite(analogOutPin, outputValue);
 
  delay(50);
}
