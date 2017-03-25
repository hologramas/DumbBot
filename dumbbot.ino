// 
// MIT License.
//

//
// This program was created for a career day demo to first grade children uing a Makerblock starter robot kit (tank).
// The goal is to ilustrate that a machine controlled by a computer follows the steps given my the programmer, even if the steps do not make sense and that some times you can play with trial an error.
// To achieve the goal:
// First: The robot is instructed to move forward regardless of any obstacles. The robot will then move forward and hit the wall and keep moving (even if that doesn't make sense).
// Then: The robot is instructed to move forward and stop if any obstacles are in the way. The robot will then move forward and stop before hitting the wall.
// Then: The robot is instructed to turn around when a loud sound is detected. This is to show how the machine can do neat tricks.
// Finally: The robot is instructed to move forward and turn away from any detected obstacles and keep moving. This is to show a more automatic behavior (which will likely lead to mistakes as well).
//

#include <Makeblock.h>

#define STATE_STOPPED 0
#define STATE_MOVING_FORWARD 1
#define STATE_AUTO 2

MeDCMotor g_rightMotor(M1);
MeDCMotor g_leftMotor(M2);
MeInfraredReceiver g_infraReceiver(PORT_6);
MeUltrasonicSensor g_ultrasonicSensor(PORT_3);
MeSoundSensor g_soundSensor(PORT_8);

int g_moveSpeed = 190;
int g_state = STATE_STOPPED;
bool g_stopOnObstacle = false;

void setup() 
{
    g_infraReceiver.begin();
}

void loop() 
{
    SelectRoutine();
    if (g_state == STATE_MOVING_FORWARD)
    {
        bool isBlocked = IsObstacleDetected(20);   
        if (isBlocked)
        {
            if (g_stopOnObstacle && (g_state == STATE_MOVING_FORWARD))
            {
                Stop();
                g_state = STATE_STOPPED;
            }
        } 
    }
    else if (g_state == STATE_AUTO)
    {
        MoveAuto();
    }
    else if (g_state == STATE_STOPPPED)
    {
        if (g_soundSensor.strength() > 400)
        {
            Turn();
            delay(3000);
            Stop();
            g_state = STATE_STOPPED;
        }
    }
}

bool IsObstacleDetected(int inRow)
{
    int i = 0;
    while (i < inRow)
    {
        if (g_ultrasonicSensor.distanceCm() > 10)
        {
            return false;
        }
        delay(1);
        i++;
    }
    return true;
}

void SelectRoutine()
{
    if (g_infraReceiver.available() || g_infraReceiver.buttonState())
    {
        int readIR = g_infraReceiver.read();
        switch(readIR)
        {
          case IR_BUTTON_0:
              Stop();
              g_state = STATE_STOPPED;
              g_stopOnObstacle = false;
              break;

          case IR_BUTTON_A:
              Stop();
              Forward();
              g_state = STATE_MOVING_FORWARD;
              g_stopOnObstacle = false;
              break;
          
          case IR_BUTTON_B:
              Stop();
              Forward();
              g_state = STATE_MOVING_FORWARD;
              g_stopOnObstacle = true;
              break;
          
          case IR_BUTTON_D:
              g_state = STATE_AUTO;
              break;
        }
    }
}

void MoveAuto()
{
    while (IsObstacleDetected(10))
    {
        Turn();
        delay(1000);
    }
    Forward();
    delay(50);
}

void Forward()
{
    g_rightMotor.run(g_moveSpeed);
    g_leftMotor.run(g_moveSpeed);
}

void Turn()
{
    g_rightMotor.run(-g_moveSpeed);
    g_leftMotor.run(g_moveSpeed);
}

void Stop()
{
    g_rightMotor.run(0);
    g_leftMotor.run(0);
}



