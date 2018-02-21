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
    rightBumper = DigitalInputPin(FEHIO::P0_0);
    leftBumper = DigitalInputPin(FEHIO::P3_7);
    rightEncoder = DigitalEncoder(FEHIO::P1_7);
    leftEncoder = DigitalEncoder(FEHIO::P2_0);
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
    //figure out the counts need for the distance
    int counts = (int)(distance*318/1.5/3.141592);

    //reset the encoder counts
    rightEncoder.ResetCounts();
    leftEncoder.ResetCounts();

    rightMotor.SetPercent(power);
    leftMotor.SetPercent(power);

    while(leftEncoder.Counts()+rightEncoder.Counts()<counts){}

    //stop the motor
    rightMotor.Stop();
    leftMotor.Stop();

    //log the encoder counts
    SD.OpenLog();
    SD.Printf("Left: %d  Right: %d\n", leftEncoder.Counts(), rightEncoder.Counts());
    SD.CloseLog();

    LCD.Clear( FEHLCD::Green);

    return 0;

}
int Controls::turn(int motorPower, int degrees)
{
  //clean this degrees bs up late
  int counts = (degrees/180.0)*440;

  if(counts<0){
      counts =counts*-1;
  }


  if(degrees>0)
  {
      while(leftEncoder.Counts()+rightEncoder.Counts()<counts){
          rightMotor.SetPercent(motorPower);
          leftMotor.SetPercent(-motorPower);
      }
  }
  else
  {
      while(leftEncoder.Counts()+rightEncoder.Counts()<counts){
          rightMotor.SetPercent(-motorPower);
          leftMotor.SetPercent(motorPower);
      }
  }
}
