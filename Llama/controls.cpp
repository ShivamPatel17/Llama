#include "controls.h"
#include <fehproteusfirmware/Libraries/FEHIO.h>
#include <fehproteusfirmware/Libraries/FEHLCD.h>
#include <fehproteusfirmware/Libraries/FEHMotor.h>
#include <fehproteusfirmware/Libraries/FEHUtility.h>
#include <fehproteusfirmware/Libraries/FEHSD.h>
#include <fehproteusfirmware/Libraries/FEHServo.h>
#include <fehproteusfirmware/Libraries/FEHRPS.h>


//makes the variables from main accessible here
extern FEHMotor rightMotor;
extern FEHMotor leftMotor;
extern DigitalInputPin leftBumper;
extern DigitalInputPin rightBumper;
extern DigitalEncoder rightEncoder;
extern DigitalEncoder leftEncoder;
extern FEHServo wrenchServo;
extern FEHServo fuelServo;
extern AnalogInputPin cds;
extern int superConstant;
extern double distanceConstant;

Controls::Controls(int super, double distance)
{
    rightMotor = FEHMotor(FEHMotor::Motor0,9.0);
    leftMotor = FEHMotor(FEHMotor::Motor1, 9.0);
    rightBumper = DigitalInputPin(FEHIO::P0_7);
    leftBumper = DigitalInputPin(FEHIO::P3_7);
    rightEncoder = DigitalEncoder(FEHIO::P1_3);
    leftEncoder = DigitalEncoder(FEHIO::P2_4);
    wrenchServo = FEHServo(FEHServo::Servo0);
    fuelServo = FEHServo(FEHServo::Servo7);
    cds = AnalogInputPin(FEHIO::P2_1);
    setSuperConstant(super);
    setDistanceConstant(distance);

    fuelServo.SetMin(500);
    fuelServo.SetMax(2452);
    wrenchServo.SetMin(500);
    wrenchServo.SetMax(2410);

    wrenchServo.SetDegree(90);
    initializeCrank(RPS.FuelType());
}

void Controls::setSuperConstant(int i){
    superConstant = i;
}

int Controls::getSuperConstant(){
    return superConstant;
}
void Controls::setDistanceConstant(double i){
    distanceConstant = i;
}
double Controls::getDistanceConstant(){
    return distanceConstant;
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

    if(distance>4.0){
        leftMotor.SetPercent(power-0.85);
        rightMotor.SetPercent(power);


        while(leftEncoder.Counts()+rightEncoder.Counts()<counts){}

        //stop the motor
        rightMotor.Stop();
        leftMotor.Stop();
    }
    else{
        leftMotor.SetPercent(power);
        rightMotor.SetPercent(power);


        while(leftEncoder.Counts()+rightEncoder.Counts()<counts){}

        //stop the motor
        rightMotor.Stop();
        leftMotor.Stop();
    }

    return 0;

}
int Controls::turn(int degrees, int motorPower)
{

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
/***************************
 * drive straight until a wall is hit
 *
 * @param - motorPower, the power given to the motors
 * *********************************************/
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


/*********************
 * Runs until the cds is on
 *
 * @param threshold
 * *******************/
void Controls::waitForCDS(int thres){
   while(cds.Value()>thres){

   }
}


/*****************************************
 * reads the color from the course
 *
 * return 1 if the color is red and -1 if blue
 * ****************************************/
int Controls::cdsColor(){
    if(cds.Value()>0.55){
        return -1;
    }
    else{
        return 1;
    }
}

/***********************************
 * drives until a light has been sensed
 *
 * @param - motorPower, the power at which the igwans go
 * *******************************/
int Controls::driveUntilLight(int motorPower){
    LCD.WriteLine("In dUL method");
    //start moving first
    leftMotor.SetPercent(motorPower-0.85);
    rightMotor.SetPercent(motorPower);

    //go until both bumpers are pressed
    while(cds.Value()>1.2){}

    //stop
    rightMotor.Stop();
    leftMotor.Stop();
}

/**********************************************
 * performs the control panel task
 * hopefully regardless of how far away from the panel
 * the robot is, so long as it lines up with the wall
 * to the right of the panel
 *
 * @param color - the color the robot needs to push
 *      1 = red
 *      -1 = blue
 * ************************************************/

int Controls::controlPanel(int color){
    if(color==1){
        driveDistance(5.0,-35);
        Sleep(0.1);
        turn(90,25);
        drive(3.0,35);
        //add a drive for time when needed
    }
    else if(color==-1){
        driveDistance(0.5,-25);
        Sleep(0.1);
        turn(90,25);
        drive(4.0,35);
        //add a drive for time when needed
    }
    else{
        LCD.Clear(BLACK);
        LCD.WriteLine("Bad parameter for control panel mathod");
    }
}
int Controls::initializeCrank(int direction){
    if(direction==1){
        int degree = 0;
        fuelServo.SetDegree(degree);
    }
    else if(direction ==2){
        int degree = 180;
        fuelServo.SetDegree(degree);

    }
    else{
        LCD.Clear(RED);
        LCD.WriteLine("Fuel Crank direction not detected");
    }
}

int Controls::turnCrank(int direction){
    if(direction==1){
        LCD.WriteLine(1);
        int degree = 0;
        fuelServo.SetDegree(degree);
        Sleep(0.5);
        while(degree<179){
            degree++;
            fuelServo.SetDegree(degree);
            Sleep(10);
        }
    }
    else if(direction ==2){
        LCD.WriteLine(2);
        int degree = 180;
        fuelServo.SetDegree(degree);
        Sleep(0.5);
        while(degree>1){
            degree--;
            fuelServo.SetDegree(degree);
        }
    }
    else{
        LCD.Clear(RED);
        LCD.WriteLine("Fuel Crank direction not detected");
    }
}

void Controls::setWrenchDegree(int degree){
    wrenchServo.SetDegree(degree);
}


//*************************************************************************************

//RPS RPS  RPS  RPS  RPs

//************************************************************************************
void Controls::checkXPlus(float x_coordinate) //using RPS while robot is in the +x direction
{
    if(x_coordinate<RPS.X()){
        driveDistance(RPS.X()-x_coordinate,-25);
    }
    else{
        driveDistance(x_coordinate-RPS.X(),25);
    }
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 2 || RPS.X() > x_coordinate + 2)
    {
        if(RPS.X() > x_coordinate)
        {
            driveDistance(.1,-20);
            Sleep(0.1);
        }
        else if(RPS.X() < x_coordinate)
        {
            driveDistance(.1,20);
            Sleep(0.1);
        }
    }
}

void Controls::checkXMinus(float x_coordinate) //using RPS while robot is in the +x direction
{
    if(x_coordinate<RPS.X()){
        driveDistance(RPS.X()-x_coordinate,25);
    }
    else{
        driveDistance(x_coordinate-RPS.X(),-25);
    }
    //check whether the robot is within an acceptable range
    while(RPS.X() < x_coordinate - 2 || RPS.X() > x_coordinate + 2)
    {
        if(RPS.X() > x_coordinate)
        {
            driveDistance(.1,20);
            Sleep(0.1);
        }
        else if(RPS.X() < x_coordinate)
        {
            driveDistance(.1,-20);
            Sleep(0.1);
        }
    }
}

void Controls::checkYMinus(float y_coordinate) //using RPS while robot is in the -y direction
{
    if(y_coordinate<RPS.Y()){
        driveDistance(RPS.Y()-y_coordinate,25);
    }
    else{
        driveDistance(y_coordinate-RPS.Y(),-25);
    }
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        if(RPS.Y() > y_coordinate)
        {
           driveDistance(.1,20);
            Sleep(0.1);
            rightMotor.Stop();
            leftMotor.Stop();

            //<ADD CODE HERE>
        }
        else if(RPS.Y() < y_coordinate)
        {
            driveDistance(.1,-20);
            Sleep(0.1);
        }
    }
}

void Controls::checkYPlus(float y_coordinate) //using RPS while robot is in the +y direction
{
    if(y_coordinate<RPS.Y()){
        driveDistance(RPS.Y()-y_coordinate,-25);
    }
    else{
        driveDistance(y_coordinate-RPS.Y(),25);
    }
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 2 || RPS.Y() > y_coordinate + 2)
    {
        if(RPS.Y() > y_coordinate)
        {
            driveDistance(.1,20);
            Sleep(0.1);
        }
        else if(RPS.Y() < y_coordinate)
        {
            driveDistance(0.1,-20);
            Sleep(0.1);
        }
    }
}
void Controls::checkHeading(float heading) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)


        while((RPS.Heading() >heading +3 || RPS.Heading()<heading-3)){
           LCD.WriteLine(RPS.Heading());
           Sleep(100);
           if(RPS.Heading()>heading){
               turn(.3,20);
           }
           else if(RPS.Heading()<heading){
               turn(-.3,20);
           }
        }
}




