/*
 * Author:  David Oniani, Kevin Cobble
 * Libraries: Stepper, Servo, dht, Built-in(s)
 * Date: February 25, 2019
 */

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <dht.h>

const int VRx = 0; // Connect to Analog Pin 0
const int VRy = 1; // Connect to Analog Pin 1
const int SW = 4; // Connect to Digital Pin 4
bool paused = false; //text not paused by default
const int c = 261;
const int d = 294;
const int e = 329;
const int f = 349;
const int g = 391;
const int gS = 415;
const int a = 440;
const int aS = 455;
const int b = 466;
const int cH = 523;
const int cSH = 554;
const int dH = 587;
const int dSH = 622;
const int eH = 659;
const int fH = 698;
const int fSH = 740;
const int gH = 784;
const int gSH = 830;
const int aH = 880;

const int buzzerPin = 8; // Digital Pin 8
const int ledPin1 = 12;  // Digital Pin 12
const int ledPin2 = 13;  // Digital Pin 13 Built In Led can Change it if you want

int counter = 0;
const char text[500] = "Turmoil has engulfed the Galactic Republic. The taxation of trade routes to outlying star systems is in dispute. Hoping to resolve the matter with a blockade of deadly battleships, the greedy Trade Federation has stopped all shipping to the small planet of Naboo. While the Congress of the public endlessly debates this alarming chain of events, the Supreme Chancellor has secretly dispatched two Jedi Knights, the guardians of peace and justice in the galaxy, to settle the conflict...              ";
int index1 = 0;
int index2 = 16;
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

dht DHT;
#define DHT11_PIN 7
bool temp = false;

void setup(){
  lcd.begin(16,2);
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STAR WARS");
  pinMode(SW, INPUT); //button
  digitalWrite(SW, HIGH);
  Serial.begin(9600); // Start Serial Monitor
  Serial.print(text);
  pinMode(buzzerPin, OUTPUT); // Digital Pin 8
  pinMode(ledPin1, OUTPUT);  // Digital Pin 12
  pinMode(ledPin2, OUTPUT); // Digital Pin 13 Built In Led can Change it if you want
}

void loop(){
  //moveLeft(); //scrolls characters on screen to the left
  Serial.print("Playing Starwars Theme");
  Serial.print("Puased: ");
  Serial.println(paused);
  if (digitalRead(SW) == 0){
    paused = true;
    delay(1000);
    pause();
  }


  //Play first section
  firstSection();

  //Play second section
  secondSection();

  //Variant 1
  beep(f, 250);
  beep(gS, 500);
  beep(f, 350);
  beep(a, 125);
  beep(cH, 500);
  beep(a, 375);
  beep(cH, 125);
  beep(eH, 650);

  delay(500);

  //Repeat second section
  secondSection();

  //Variant 2
  beep(f, 250);
  beep(gS, 500);
  beep(f, 375);
  beep(cH, 125);
  beep(a, 500);
  beep(f, 375);
  beep(cH, 125);
  beep(a, 650);

  delay(650);
}
void humidTemp(){
  lcd.clear();
  int chk = DHT.read11(DHT11_PIN);
  lcd.setCursor(0,0);
  lcd.print("Temp: ");
  lcd.print(DHT.temperature);
  lcd.print((char)223);
  lcd.print("C");
  lcd.setCursor(0,1);
  lcd.print("Humidity: ");
  lcd.print(DHT.humidity);
  lcd.print("%");
  Serial.print("Temp: ");
  Serial.print(DHT.temperature);
  Serial.print((char)223);
  Serial.print("C\n");
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity);
  Serial.print("%\n");
}
void pause(){
  Serial.print("Song paused \n");
  while (paused){
    if (analogRead(VRx) > 520 || analogRead(VRx) < 480){
        Serial.print("Switching to Temperature & Humidity\n");
        temp=true;
    }
    delay(500);
    while (analogRead(VRy) > 550){
      Serial.print("Scrolling Left to Right\n");
      moveRight();
    }
    while (analogRead(VRy) < 480){
      Serial.print("Scrolling Right to Left\n");
      moveLeft();
    }
    if (digitalRead(SW) == 0){
      paused = false;
    }
    while (temp){
      humidTemp();
      if (analogRead(VRx) > 520 || analogRead(VRx) < 480){
        temp=false;
        Serial.print("Switching to song\n");
        lcd.clear();
        lcd.print("STAR WARS");
        moveRight();
      }
      delay(1000);
    }
  }

  Serial.print("Resumed playing: Starwars Theme\n");
}

void moveRight(){
  lcd.setCursor(0,1);
  int i;
  String chunk;
  if (index1 == -1){
    index1 = 489;
  }
  if (index2 == -1){
    index2 = 489;
  }
  if (index1 < 490 && index2 < 490){
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  else if (index1 < 490){
    for (i=index1; i<490; i=i+1){
      chunk += text[i];
    }
    if (index2>490){
      index2 = 0;
    }
    else {
      for (i=0; i<index2;i=i+1){
        chunk+=text[i];
      }
    }
  }
  else {
    index1 = 0;
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STAR WARS");
  lcd.setCursor(0,1);
  lcd.print(chunk);
  index1 -= 1;
  index2 -= 1;

  delay(150);
}

void moveLeft(){
  lcd.setCursor(0,1);
  int i;
  String chunk;
  if (index1 == 490){
    index1 = 0;
  }
  if (index2 == 490){
    index2 = 0;
  }
  if (index1 < 490 && index2 < 490){
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  else if (index1 < 490){
    for (i=index1; i<490; i=i+1){
      chunk += text[i];
    }
    if (index2>499){
      index2 = 0;
    }
    else {
      for (i=0; i<index2;i=i+1){
        chunk+=text[i];
      }
    }
  }
  else {
    index1 = 0;
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STAR WARS");
  lcd.setCursor(0,1);
  lcd.print(chunk);
  index1 += 1;
  index2 += 1;

  delay(150);
}

void autoMoveLeft(){
  lcd.setCursor(0,1);
  int i;
  String chunk;
  if (index1 == 490){
    index1 = 0;
  }
  if (index2 == 490){
    index2 = 0;
  }
  if (index1 < 490 && index2 < 490){
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  else if (index1 < 490){
    for (i=index1; i<490; i=i+1){
      chunk += text[i];
    }
    if (index2>499){
      index2 = 0;
    }
    else {
      for (i=0; i<index2;i=i+1){
        chunk+=text[i];
      }
    }
  }
  else {
    index1 = 0;
    for (i=index1; i<index2; i=i+1){
    chunk += text[i];
    }
  }
  lcd.clear();
  lcd.setCursor(0,0);
  lcd.print("STAR WARS");
  lcd.setCursor(0,1);
  lcd.print(chunk);
  index1 += 1;
  index2 += 1;
}

void beep(int note, int duration)
{
  //Play tone on buzzerPin
  tone(buzzerPin, note, duration);
  autoMoveLeft();
  if (digitalRead(SW) == 0){
    paused = true;
    delay(1000);
    pause();
  }

  //Play different LED depending on value of 'counter'
  if(counter % 2 == 0)
  {
    digitalWrite(ledPin1, HIGH);
    delay(duration);
    digitalWrite(ledPin1, LOW);
  }else
  {
    digitalWrite(ledPin2, HIGH);
    delay(duration);
    digitalWrite(ledPin2, LOW);
  }

  //Stop tone on buzzerPin
  noTone(buzzerPin);

  delay(50);
  if (digitalRead(SW) == 0){
    paused = true;
    delay(1000);
    pause();
  }

  //Increment counter
  counter++;
}

void firstSection()
{
  beep(a, 500);
  beep(a, 500);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);

  beep(eH, 500);
  beep(eH, 500);
  beep(eH, 500);
  beep(fH, 350);
  beep(cH, 150);
  beep(gS, 500);
  beep(f, 350);
  beep(cH, 150);
  beep(a, 650);

  delay(500);
}

void secondSection()
{
  beep(aH, 500);
  beep(a, 300);
  beep(a, 150);
  beep(aH, 500);
  beep(gSH, 325);
  beep(gH, 175);
  beep(fSH, 125);
  beep(fH, 125);
  beep(fSH, 250);

  delay(325);

  beep(aS, 250);
  beep(dSH, 500);
  beep(dH, 325);
  beep(cSH, 175);
  beep(cH, 125);
  beep(b, 125);
  beep(cH, 250);

  delay(350);
}
