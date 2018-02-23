#include "controls.h"
#include <fehproteusfirmware/Libraries/FEHIO.h>
#include <fehproteusfirmware/Libraries/FEHLCD.h>
#include <fehproteusfirmware/Libraries/FEHMotor.h>
#include <fehproteusfirmware/Libraries/FEHUtility.h>
#include <fehproteusfirmware/Libraries/FEHSD.h>

//makes the variables from main accessible here
extern FEHMotor rightMotor;
extern FEHMotor leftMotor;
extern DigitalInputPin leftBumper;
extern DigitalInputPin rightBumper;
extern DigitalEncoder rightEncoder;
extern DigitalEncoder leftEncoder;

Controls::Controls()
{
    rightMotor = FEHMotor(FEHMotor::Motor0,9.0);
    leftMotor = FEHMotor(FEHMotor::Motor1, 9.0);
    rightBumper = DigitalInputPin(FEHIO::P0_7);
    leftBumper = DigitalInputPin(FEHIO::P3_7);
    rightEncoder = DigitalEncoder(FEHIO::P1_3);
    leftEncoder = DigitalEncoder(FEHIO::P2_4);
}



/************************
 * @param time - how long to drive straight for
 *
 * @param power - with what power
 *
 * @param right, left - the motors
 *
 *
 * This method should have the robot drive straight! :)
 * ***********************/
int Controls::drive(double time, int power)
{
    //reset the encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    //move the bot forward
    rightMotor.SetPercent(power);
    leftMotor.SetPercent(power);

    float t = TimeNow();
    //spin the motor for a certain time
    while(TimeNow()-t<time){
        if(!leftBumper.Value() || !rightBumper.Value()){
            rightMotor.Stop();
            leftMotor.Stop();
            LCD.Clear( FEHLCD::Green);
            return 0;
        }
    }

    //stop the motor
    rightMotor.Stop();
    leftMotor.Stop();


    //log the encoder counts
    SD.OpenLog();
    SD.Printf("Left: %d  Right: %d\n", leftEncoder.Counts(), rightEncoder.Counts());
    SD.CloseLog();

    return 0;
}
/************************************
 * @param power - the power the motors should go
 *
 * @param distance - the desired distance
 *
 * This method should make the robot drive a certain distance
 * ***********************************/
int Controls::driveDistance(double distance, int power){
    double distanceConstant = 0.95;
    //figure out the counts need for the distance
    int counts = (int)(distanceConstant*distance*318/1.5/3.141592);

    //reset the encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    leftMotor.SetPercent(power-1);
    rightMotor.SetPercent(power);


    while(leftEncoder.Counts()+rightEncoder.Counts()<counts){}

    //stop the motor
    rightMotor.Stop();
    leftMotor.Stop();

    LCD.Clear( FEHLCD::Green);

    return 0;

}
int Controls::turn(int degrees, int motorPower)
{
  int superConstant = 680;
  //clean this degrees bs up late
  int counts = (degrees/180.0)*superConstant;

  if(counts<0){
      counts =counts*-1;
  }


  if(degrees<0)
  {
      leftEncoder.ResetCounts();
      rightEncoder.ResetCounts();

      while(leftEncoder.Counts()+rightEncoder.Counts()<counts){
          rightMotor.SetPercent(motorPower);
          leftMotor.SetPercent(-1*motorPower);
      }
      rightMotor.Stop();
      leftMotor.Stop();
  }
  else
  {
      leftEncoder.ResetCounts();
      rightEncoder.ResetCounts();

      while(leftEncoder.Counts()+rightEncoder.Counts()<counts){
          rightMotor.SetPercent(-1*motorPower);
          leftMotor.SetPercent(motorPower);
      }

      rightMotor.Stop();
      leftMotor.Stop();

  }
}

int Controls::straightUntilWall(int motorPower){
    LCD.WriteLine("In the sUW method");
    //start moving first
    leftMotor.SetPercent(motorPower);
    rightMotor.SetPercent(motorPower);

    //go until both bumpers are pressed
    while(leftBumper.Value()||rightBumper.Value()){}

    //stop
    rightMotor.Stop();
    leftMotor.Stop();
}
