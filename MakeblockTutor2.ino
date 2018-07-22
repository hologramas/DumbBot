//
// This program controls a Makeblock tank-mode
// robot. It was created to ilustrate trivial computer
// programming concepts to first and second grade
// studends.
// 
// The program uses the following hardware components:
// 1) A Makeblock Orion main control board.
// 2) An infrared receiver at PORT 6.
// 3) An ultrasonic sensor at PORT 3.
// 4) A sound sensor at PORT 7.
// 5) Two DC motors to move the tank.
//
// The program boots, initializes the infrared receiver
// and sets the state to standing.
// The program can then be ser to the following modes
// by using infrared signals:
// A) Walker : instructs the robot to move forward until
//      otherwise told to stop.
// B) WalkerObstacle : instructs the robot to move forward
//      and stop if an obstable is detected using the ultrasonic
//      sensor.
// C) WalkerObstableTurn : instructs the robot to move forward
//      and stop if an obstable is detected using the ultrasonic
//      sensor. If a loud noise is perceived through the sound
//      sensor, turn left.
// D) Dancer : instructs the robot to stand still and turn left
//      if a load noise is preceived through the sound sensor.
// E) Control : sets the robot into remote control mode, the robot
//      stands still and responds to requests to move from the IR
//      controller (forward, back, left, right)

#include <SoftwareSerial.h>
#include <Wire.h>
#include <MeOrion.h>

enum App
{
  App_Stand,
  App_Walker,
  App_WalkerObstacle,
  App_WalkerObstableTurn,
  App_Dancer,
  App_Control
};

MeDCMotor g_rightMotor(M1);
MeDCMotor g_leftMotor(M2);
int g_speed = 200;

MeInfraredReceiver g_infraReceiver(PORT_6);
MeUltrasonicSensor g_ultrasonicSensor(PORT_3);
MeSoundSensor g_soundSensor(PORT_7);

void setup()
{
  g_infraReceiver.begin();
}

App SelectApplication()
{
  static App currentApp = App_Stand;
  if (g_infraReceiver.available())
  {
    uint8_t code = g_infraReceiver.read();
    switch(code)
    {
      case IR_BUTTON_A:
        currentApp = App_Walker;
        break;
      case IR_BUTTON_B:
        currentApp = App_WalkerObstacle;
        break;
      case IR_BUTTON_C:
        currentApp = App_WalkerObstableTurn;
        break;
      case IR_BUTTON_D:
        currentApp = App_Dancer;
        break;
      case IR_BUTTON_E:
        currentApp = App_Control;
        break;
      case IR_BUTTON_0:
        currentApp = App_Stand;
        break;
    }
  }
  return currentApp;
}

void loop()
{
  App app = SelectApplication();
  switch(app)
  {
    case App_Stand:
      Stop();
      break;

    case App_Walker:
      MoveForward();
      break;

    case App_WalkerObstacle:  
      WalkWithObstable();
      break;

    case App_WalkerObstableTurn:
      WalkWithObstable();
      if (g_soundSensor.strength() > 400)
      {
        TurnLeft();
        delay(1500);
      }
      break;

    case App_Dancer:
      if (g_soundSensor.strength() > 400)
      {
        TurnLeft();
        delay(1500);
      }
      break;

    case App_Control:
      ObeyControl();
      break;
  }
}

void WalkWithObstable()
{
  int distance = GetDistanceToObstacle();
  if (distance < 15)
  {
    Stop();
  }
  else
  {
    MoveForward();
  }
}

void ObeyControl()
{
  if (!g_infraReceiver.buttonState())
  {
    Stop();
  }

  if (g_infraReceiver.available())
  {
    uint8_t code = g_infraReceiver.read();
    switch(code)
    {
      case IR_BUTTON_UP:
        MoveForward();
        break;

      case IR_BUTTON_DOWN:
        MoveBackward();
        break;

      case IR_BUTTON_LEFT:
        TurnLeft();
        break;

      case IR_BUTTON_RIGHT:
        TurnRight();
        break;
    }
  }
}

void Stop()
{
  g_leftMotor.stop();
  g_rightMotor.stop();
}

void MoveForward()
{
  g_leftMotor.run(g_speed);
  g_rightMotor.run(g_speed);
  delay(100);
}

void MoveBackward()
{
  g_leftMotor.run(-g_speed);
  g_rightMotor.run(-g_speed);
  delay(100);
}

void TurnLeft()
{
  g_leftMotor.run(-g_speed);
  g_rightMotor.run(g_speed);
  delay(100);
}

void TurnRight()
{
  g_leftMotor.run(g_speed);
  g_rightMotor.run(-g_speed);
  delay(100);
}

int GetDistanceToObstacle()
{
    int accumulatedSample = 0;
    for (int i = 0; i < 10; i++)
    {
      accumulatedSample += g_ultrasonicSensor.distanceCm();
      delay(1);
    }
    
    return (accumulatedSample / 10);
}

