/*
 * Author:  David Oniani, Samantha Sixta
 * Libraries: Stepper, Servo, dht, Built-in(s)
 * Date: March 8, 2019
 */

#include <Stepper.h>
#include <Servo.h>
#include <dht.h>

// PIN numbers for the stepper motor
#define IN1_PIN 3
#define IN2_PIN 4
#define IN3_PIN 5
#define IN4_PIN 6

// PIN number for the servo motor
#define SERVO_PIN 9

// PIN number for the temperature/humidity sensor
#define DHT11_PIN 11

// Miscellaneous variables (better to have them initialized)
int step_value = 64;
int rotate_value = 0;
char command = ' ';
bool environment_call_status = false;
bool stepper_activation_status = false;
bool servo_activation_status = false;

// Stepper motor, servo motor, and the temperature/humidity sensor
Stepper stepper_motor(200, IN1_PIN, IN2_PIN, IN3_PIN, IN4_PIN);
Servo servo_motor;
dht DHT;

// Setup
void setup()
{
  Serial.begin(9600);

  // Stepper setup
  pinMode(IN1_PIN, OUTPUT);
  pinMode(IN2_PIN, OUTPUT);
  pinMode(IN3_PIN, OUTPUT);
  pinMode(IN4_PIN, OUTPUT);

  stepper_motor.setSpeed(60);

  // Servo setup
  servo_motor.attach(SERVO_PIN);

  // Temperature and humidity sensor setup
  int chk = DHT.read11(DHT11_PIN);
}

// Display the current temperature and humidity
void environment_info()
{
  Serial.print("Temperature: ");
  Serial.print(DHT.temperature);
  Serial.print(" °C\n");
  Serial.print("Humidity: ");
  Serial.print(DHT.humidity);
  Serial.print("%\n");
}

// Stepper activation function
void stepper_activate()
{
  Serial.println("The stepper motor has been activated!");
  stepper_activation_status = true;
}

// Stepper deactivation function
void stepper_deactivate()
{
  Serial.println("The stepper motor has been deactivated.");
  stepper_activation_status = false;
}

// Servo activation function
void servo_activate()
{
  Serial.println("The servo motor has been activated!");
  servo_activation_status = true;
}

// Servo deactivation function
void servo_deactivate()
{
  Serial.println("The servo motor has been deactivated.");
  servo_activation_status = false;
}

// One complete sweep with the servo motor
void servo_sweep()
{
  for (rotate_value = 0; rotate_value <= 160; rotate_value += 1) {
      servo_motor.write(rotate_value);
      delay(15);
    }

  for (rotate_value = 160; rotate_value >= 0; rotate_value -= 1) {
      servo_motor.write(rotate_value);
      delay(15);
    }
}

// Main loop
void loop()
{
  if (!environment_call_status) {
    environment_info();
    environment_call_status = true;
  }

  if (Serial.available() > 0) {
    command = Serial.read();
  }

  /*
   * Recall that commands 'x' and 'y' are reserverd for the stepper motor
   * while the commands 'm' and 'n' are used to control the servo motor.
   * Commands 'a' and 'd' are used for the force-activatation of the motors.
   *
   * 'x' - activate stepper motor
   * 'y' - deactivate stepper motor
   *
   * 'm' - activate servo motor
   * 'n' - deactivate servo
   *
   * 'a' - activate both the stepper and the servo motor
   * 'd' - deactivate both the stepper and the servo motor
   *
   * The commands are rather random characters and do not specifically
   * reflect anything from the real world applications of these motors.
   *
   * NOTE: Commands 'a' and 'd' which prompt the system to do the
   * force-activation and force-deactivation (respectively) of both of the
   * motors do not make both motors work at the same time. Simultaneous motion
   * is not possible without having more than one arduino board.
   */

  switch(command) {
    // Stepper motor activation command
    case 'x':
      if (DHT.temperature >= 20) {
        stepper_activate();
      }
      else {
        Serial.println("Temperature is below 20 °C!");
        stepper_deactivate();
      }
      break;

    // Stepper motor deactivation command
    case 'y':
      stepper_deactivate();
      break;

    // Servo motor activation command
    case 'm':
      if (DHT.humidity <= 40) {
        servo_activate();
      }
      else {
        Serial.println("Humidity is above 40 percent");
        servo_deactivate();
      }
      break;

    // Servo motor deactivation command
    case 'n':
      servo_deactivate();
      break;

    // Force activate both the stepper and the servo motors
    case 'a':
      stepper_activate();
      servo_activate();
      break;

    // Force deactivate both the stepper and the servo motors
    case 'd':
      stepper_deactivate();
      servo_deactivate();
      break;
  }

  if (stepper_activation_status) {
    stepper_motor.step(step_value);
  }

  if (servo_activation_status) {
    servo_sweep();
  }
}
