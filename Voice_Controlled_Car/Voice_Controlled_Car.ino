#define CUSTOM_SETTINGS
#define INCLUDE_TERMINAL_MODULE
#include <DabbleESP32.h>

#define MOTOR_SPEED 200
#define TURNING_SPEED 150

//Right motor
int enableRightMotor=22; 
int rightMotorPin1=16;
int rightMotorPin2=17;

//Left motor
int enableLeftMotor=23;
int leftMotorPin1=18;
int leftMotorPin2=19;

const int PWMFreq = 1000; /* 1 KHz */
const int PWMResolution = 8;
const int rightMotorPWMSpeedChannel = 4;
const int leftMotorPWMSpeedChannel = 5;

int motorSpeed = MOTOR_SPEED;
int carDirection = 0;

void controlWithVoice()
{
  if (Terminal.available() != 0)
  {
    String serialdata = Terminal.readString();
    serialdata.toLowerCase();
    if (serialdata.indexOf("forward") != -1)                                      //Move car Forward
    {
      carDirection = 1;      
      rotateMotor(motorSpeed, motorSpeed);      
    }
    else if (serialdata.indexOf("backward") != -1 || serialdata == "reverse" || serialdata == "back")     //Move car Backward
    {
      carDirection = -1;
      rotateMotor(-motorSpeed, -motorSpeed);    
    }
    else if (serialdata == "right" || serialdata == "turn right")                 //Quickly Move car Right.
    {
      int turningSpeed = (carDirection == -1 ? -TURNING_SPEED : TURNING_SPEED);
      rotateMotor(-turningSpeed, turningSpeed);
      delay(200); 
      rotateMotor(motorSpeed * carDirection, motorSpeed * carDirection);   
    }
    else if (serialdata == "left" || serialdata == "turn left")                   //Quickly Move car Left
    {
      int turningSpeed = (carDirection == -1 ? -TURNING_SPEED : TURNING_SPEED);
      rotateMotor(turningSpeed, -turningSpeed);    
      delay(200);
      rotateMotor(motorSpeed * carDirection, motorSpeed * carDirection);   
    }
    else if (serialdata == "rotate right")                                        //Keep Moving car Right
    {
      int turningSpeed = (carDirection == -1 ? -TURNING_SPEED : TURNING_SPEED);
      rotateMotor(-turningSpeed, turningSpeed);
    }
    else if (serialdata == "rotate left")                                         //Keep Moving car Left
    {
      int turningSpeed = (carDirection == -1 ? -TURNING_SPEED : TURNING_SPEED);
      rotateMotor(turningSpeed, -turningSpeed);    
    }    
    else if (serialdata == "stop")                                                //Stop the car
    {
      carDirection = 0;         
      rotateMotor(0, 0);
    } 
    else if (serialdata.indexOf("speed ") == 0)                                   //Change speed only if "speed " is found at the begining of command
    {
      serialdata.remove(0, 6);
      int newMotorSpeed = serialdata.toInt();
      if (newMotorSpeed == 0) return;
      motorSpeed = constrain(newMotorSpeed, 0, 255);
      rotateMotor(motorSpeed * carDirection, motorSpeed * carDirection);       
    } 
    else if (serialdata == "dance")                                               //Dance 
    {
      for (int i = 0; i < 5; i++)
      {
        rotateMotor(motorSpeed, motorSpeed);  
        delay(200); 
        rotateMotor(-motorSpeed, -motorSpeed);  
        delay(400); 
        rotateMotor(motorSpeed, motorSpeed);  
        delay(200); 
         
        rotateMotor(-motorSpeed, motorSpeed);  
        delay(200); 
        rotateMotor(motorSpeed, -motorSpeed);  
        delay(400);
        rotateMotor(-motorSpeed, motorSpeed);  
        delay(400); 
        rotateMotor(motorSpeed, -motorSpeed);  
        delay(200);
        
        rotateMotor(-motorSpeed, motorSpeed);  
        delay(100); 
        rotateMotor(motorSpeed, -motorSpeed);  
        delay(200);
        rotateMotor(-motorSpeed, motorSpeed);  
        delay(200); 
        rotateMotor(motorSpeed, -motorSpeed);  
        delay(200);
        rotateMotor(-motorSpeed, motorSpeed);  
        delay(200);         
        rotateMotor(motorSpeed, -motorSpeed);  
        delay(100); 
      }
      carDirection = 0;        
      rotateMotor(0, 0);                      
    }    
  }
}


void rotateMotor(int rightMotorSpeed, int leftMotorSpeed)
{
  if (rightMotorSpeed < 0)
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,HIGH);    
  }
  else if (rightMotorSpeed > 0)
  {
    digitalWrite(rightMotorPin1,HIGH);
    digitalWrite(rightMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(rightMotorPin1,LOW);
    digitalWrite(rightMotorPin2,LOW);      
  }
  
  if (leftMotorSpeed < 0)
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,HIGH);    
  }
  else if (leftMotorSpeed > 0)
  {
    digitalWrite(leftMotorPin1,HIGH);
    digitalWrite(leftMotorPin2,LOW);      
  }
  else
  {
    digitalWrite(leftMotorPin1,LOW);
    digitalWrite(leftMotorPin2,LOW);      
  }
  
  ledcWrite(rightMotorPWMSpeedChannel, abs(rightMotorSpeed));
  ledcWrite(leftMotorPWMSpeedChannel, abs(leftMotorSpeed));  
}

void setup()
{
  // put your setup code here, to run once:
  pinMode(enableRightMotor,OUTPUT);
  pinMode(rightMotorPin1,OUTPUT);
  pinMode(rightMotorPin2,OUTPUT);
  
  pinMode(enableLeftMotor,OUTPUT);
  pinMode(leftMotorPin1,OUTPUT);
  pinMode(leftMotorPin2,OUTPUT);

  //Set up PWM for speed
  ledcSetup(rightMotorPWMSpeedChannel, PWMFreq, PWMResolution);
  ledcSetup(leftMotorPWMSpeedChannel, PWMFreq, PWMResolution);  
  ledcAttachPin(enableRightMotor, rightMotorPWMSpeedChannel);
  ledcAttachPin(enableLeftMotor, leftMotorPWMSpeedChannel); 

  rotateMotor(0,0); 
        
  Dabble.begin("MyVoiceCar");    //set bluetooth name of your device
}

void loop()
{
  Dabble.processInput();
  controlWithVoice();
}
