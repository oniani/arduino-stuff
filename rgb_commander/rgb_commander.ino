/*
 * Authors: David Oniani, Mason Donnohue, Thomas Twiton
 * Created: May 12, 2019
 */

// Example 2 - Receive with an end-marker
const byte numChars = 32;
char receivedChars[numChars];   // an array to store the received data
boolean newData = false;
String color = "0";
int redPin = 11;
int greenPin = 10;
int bluePin = 9;


void setup()
{
  pinMode(redPin, OUTPUT);
  pinMode(greenPin, OUTPUT);
  pinMode(bluePin, OUTPUT);
  Serial.begin(9600);
  Serial.println("<Arduino is ready>");
}

void loop()
{

  if(Serial.available()) {
    color = char(Serial.read());
  }

  Serial.print(color);

  if(color == "b") {
    setColor(0, 0, 255);  // blue
    delay(1000);
  }

  if(color == "r") {
    setColor(255, 0, 0);  // red
    delay(1000);
  }

  if(color == "g") {
    setColor(0, 255, 0);  // green
    delay(1000);
  }

  if(color == "y") {
    setColor(255, 255, 0);  // yellow
    delay(1000);
  }

  if (color == "p") {
    setColor(80, 0, 80);  // purple
    delay(1000);
  }

  if (color == "0") {
    setColor(0, 0, 0);  // no color ~ white/transparent
    delay(1000);
  }
}

void setColor(int red, int green, int blue)
{
  #ifdef COMMON_ANODE
    red = 255 - red;
    green = 255 - green;
    blue = 255 - blue;
  #endif

  analogWrite(redPin, red);
  analogWrite(greenPin, green);
  analogWrite(bluePin, blue);
}
