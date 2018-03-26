#include "controls.h"
#include <fehproteusfirmware/Libraries/FEHIO.h>
#include <fehproteusfirmware/Libraries/FEHLCD.h>
#include <fehproteusfirmware/Libraries/FEHMotor.h>
#include <fehproteusfirmware/Libraries/FEHUtility.h>
#include <fehproteusfirmware/Libraries/FEHSD.h>
#include <fehproteusfirmware/Libraries/FEHServo.h>
#include <fehproteusfirmware/Libraries/FEHRPS.h>
#include <math.h>

#define PI 3.14159265

//wrench degree
int globalDegree;


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
extern AnalogInputPin opto;
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
    opto = AnalogInputPin (FEHIO::P3_0);
    setSuperConstant(super);
    setDistanceConstant(distance);

    fuelServo.SetMin(500);
    fuelServo.SetMax(2452);
    wrenchServo.SetMin(500);
    wrenchServo.SetMax(2410);

    //wrenchServo.SetDegree(90);
    //initializeCrank(RPS.FuelType());
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

    Sleep(0.2);

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
int Controls::startMotors(int power){
    rightMotor.SetPercent(power);
    leftMotor.SetPercent(power);
}

int Controls::stopMotors(){
    rightMotor.Stop();
    leftMotor.Stop();
}

/*****************************************
 * @param degree - the number of degree to turn
 *                 negative degrees to turn left, positive for right
 *
 * @motorPower - the power at which to turn
 *
 * turns the number of degrees
 * ********************************************/
int Controls::turn(float degrees, int motorPower)
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
          leftMotor.SetPercent(-1*motorPower);
          rightMotor.SetPercent(motorPower);
      }
      leftMotor.Stop();
      rightMotor.Stop();

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
   float init = cds.Value();
   while(init-cds.Value()<thres){
       LCD.WriteLine(init-cds.Value());
       Sleep(0.5);
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
    float t = TimeNow();
    leftMotor.SetPercent(motorPower-0.85);
    rightMotor.SetPercent(motorPower);

    //go until both bumpers are pressed
    while(cds.Value()>1.2 &&(TimeNow()-t)<3.0){}

    //stop
    rightMotor.Stop();
    leftMotor.Stop();

    Sleep(1.0);
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
        float t = TimeNow();
        driveDistance(3.6,-35);
        Sleep(0.1);
        turn(90,25);
        rightMotor.SetPercent(40);
        leftMotor.SetPercent(40);
        while(!(RPS.IsDeadzoneActive()==2)||(TimeNow()-t<8.0)){}
        rightMotor.Stop();
        leftMotor.Stop();
        if(!RPS.IsDeadzoneActive()){
            driveDistance(6.0,-25);
            turn(-30,25);
            driveDistance(3.0,25);
            turn(25,25);
        }
        t=TimeNow();
        rightMotor.SetPercent(40);
        leftMotor.SetPercent(40);
        while(!(RPS.IsDeadzoneActive()==2)){}
        rightMotor.Stop();
        leftMotor.Stop();
        LCD.Clear(GREEN);
    }
    else if(color==-1){
        float t=TimeNow();
        driveDistance(1.0,-25);
        Sleep(0.1);
        turn(90,25);
        drive(4.0,35);
        rightMotor.SetPercent(40);
        leftMotor.SetPercent(40);
        while(!(RPS.IsDeadzoneActive()==2)){}
        rightMotor.Stop();
        leftMotor.Stop();
        LCD.Clear(GREEN);
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

/****************************************************
 * @param direction
 *      1 = clockwise
 *      2 = counter clock wise
 *
 * the direction is read from RPS
 *
 * turns the fuel crank disk 180 degree in the given direction
 * ********************************************************/
int Controls::turnCrank(int direction){
    if(direction==1){
        LCD.WriteLine(1);
        int degree = 0;
        fuelServo.SetDegree(degree);
        Sleep(0.5);
        while(degree<179){
            degree++;
            fuelServo.SetDegree(degree);
            Sleep(15);
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
            Sleep(15);
        }
    }
    else{
        LCD.Clear(RED);
        LCD.WriteLine("Fuel Crank direction not detected");
    }
}

void Controls::setWrenchDegree(int degree, int ms){
    if(degree>globalDegree){
        while(globalDegree<degree){
            wrenchServo.SetDegree(globalDegree);
            globalDegree++;
            Sleep(ms);
        }
    }else if(degree<globalDegree){
        while(globalDegree>degree){
            wrenchServo.SetDegree(globalDegree);
            globalDegree--;
            Sleep(ms);
        }
    }
    globalDegree = degree;
}

/************************
 * Assume we are on the right of a line and then follow
 * ***********************/
int Controls::followLine(float time){
    float ON = 2.7;
    float R= 2.66;
    float L = 2.83;
    bool right=true;
    bool onLine = true;

    //time
    float t = TimeNow();
    while(TimeNow()-t<time){
        if(right&&!onLine){
            LCD.Clear(GREEN);
            rightMotor.SetPercent(12);
            leftMotor.SetPercent(8);
            while(opto.Value()<ON && (TimeNow()-t<time)){}
            onLine = true;
            right = false;
        }
        if(onLine){
            LCD.Clear(RED);
            rightMotor.SetPercent(9);
            leftMotor.SetPercent(9);
            while((opto.Value()<R&&opto.Value()<L)&& (TimeNow()-t<time)){}
            if(opto.Value()>L){
                right = false;
            }
            if(opto.Value()<R){
                right = true;
            }
            onLine = false;
        }
        if(!onLine&&!right){
            LCD.Clear(BLUE);
            leftMotor.SetPercent(12);
            rightMotor.SetPercent(8);
            while(opto.Value()>ON && (TimeNow()-t<time)){}
            onLine = true;
            right = true;
        }
        if(!right&&onLine){
            LCD.Clear(RED);
            rightMotor.SetPercent(10);
            leftMotor.SetPercent(10);
            while((opto.Value()<R&&opto.Value()<L) && (TimeNow()-t<time)){}
            if(opto.Value()>L){
                right = false;
            }
            if(opto.Value()<R){
                right = true;
            }
            onLine = false;
        }


    }
    stopMotors();
}

//*************************************************************************************

//RPS RPS  RPS  RPS  RPS

//************************************************************************************


/****************************************************
 * moves the robot to desired x coordinate, given the
 * robot is facing in the positive x direction
 * ****************************************************/
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
/****************************************************
 * moves the robot to desired x coordinate, given the
 * robot is facing in the negative x direction
 * ****************************************************/
void Controls::checkXMinus(float x_coordinate) //using RPS while robot is in the +x direction
{
    if(x_coordinate<RPS.X()){
        driveDistance(RPS.X()-x_coordinate,15);
    }
    else{
        driveDistance(x_coordinate-RPS.X(),-15);
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
/****************************************************
 * moves the robot to desired y coordinate, given the
 * robot is facing in the negative y direction
 * ****************************************************/
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
/****************************************************
 * moves the robot to desired y coordinate, given the
 * robot is facing in the positive y direction
 * ****************************************************/
void Controls::checkYPlus(float y_coordinate) //using RPS while robot is in the +y direction
{
    if(y_coordinate<RPS.Y()){
        driveDistance(RPS.Y()-y_coordinate,-25);
    }
    else{
        driveDistance(y_coordinate-RPS.Y(),25);
    }
    //check whether the robot is within an acceptable range
    while(RPS.Y() < y_coordinate - 1 || RPS.Y() > y_coordinate + 1)
    {
        LCD.WriteLine(RPS.Y());
        if(RPS.Y() > y_coordinate)
        {
            driveDistance(.5,-20);
            Sleep(0.1);
        }
        else if(RPS.Y() < y_coordinate)
        {
            driveDistance(0.5,20);
            Sleep(0.1);
        }
    }
}

/*****************************************************
 * @param heading
 *       the desired heading the robot should be facing
 *
 * @tolerance
 *       the acceptable range the current robot's heading is within the
 *       desired heading
 *
 * turns the robot to the desired heading
 * *******************************************************/
void Controls::checkHeading(float heading,float tolerance,float degree) //using RPS
{
    //you will need to fill out this one yourself and take into account
    //the edge conditions (when you want the robot to go to 0 degrees
    //or close to 0 degrees)

        if(heading == 0||heading ==360){
            while((RPS.Heading() >2 && RPS.Heading()<180 || (RPS.Heading()<358 && RPS.Heading()>180))){
               float tempDegree = RPS.Heading();
               if(tempDegree>=0 && tempDegree<180){
                   tempDegree +=360;
               }
               LCD.WriteLine(RPS.Heading());
               LCD.WriteLine(tempDegree);
               Sleep(500);
               if(tempDegree>360){
                   turn(degree,30);
               }
               else if(tempDegree<360){
                   turn(-1*degree,30);
               }
            }
        }
        else{
            while((RPS.Heading() >heading +tolerance || RPS.Heading()<heading-tolerance)){
               LCD.WriteLine(RPS.Heading());
               Sleep(100);
               if(RPS.Heading()>heading){
                   turn(degree,30);
               }
               else if(RPS.Heading()<heading){
                   turn(-1*degree,30);
               }
               Sleep(0.1);
            }
        }
}

/***********************************************
 * @param x - the desired x coordinate
 * @param y - the desired y coordinate
 *
 * @param code - special cases
 *  0 = standard
 *  1 = not sure
 *  2 = depositing the wrench
 *
 * moves the robot to a the x and y coordinate
 * ************************************************/
void Controls::XYRPS(float x, float y , int code){
    Sleep(0.5);
    if(code ==0 ){
        //robot is less than and left of desired position
        if(RPS.X()<x && RPS.Y()<y){
            float n = (y-RPS.Y())/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn(-1*degree,15);
            n = ((x-RPS.X())*(x-(RPS.X()))+((RPS.Y()-y)*(RPS.Y()-y)));
            float distance = sqrt(n);
            SD.OpenLog();
            SD.Printf("below--Angle: %f Distance %f",degree, distance);
            SD.CloseLog();
            driveDistance(distance,12);
        }
        //roboot is greater than and left of desired position
        else if(RPS.Y()>y && RPS.X()<x){
            LCD.WriteLine("above--Y greater and X Lesser");
            float n = (RPS.Y()-y)/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn(degree,15);
            n = ((x-RPS.X())*(x-RPS.X()))+((RPS.Y()-y)*(RPS.Y()-y));
            float distance = sqrt(n);
            SD.OpenLog();
            SD.Printf("ablow--Angle: %f Distance %f",degree, distance);
            SD.CloseLog();
            driveDistance(distance,12);
        }
        //this one is used for the wrench deposit
        else if(RPS.X()>x && RPS.Y()<y){
            LCD.Clear(WHITE);
            float n = (RPS.X()-x)/(y-RPS.Y());
            float degree = atan(n)*180/PI;
            checkHeading(90+degree,2,0.9);
            n = ((RPS.X()-x)*((RPS.X()-x))+((RPS.Y()-y)*(RPS.Y()-y)));
            float distance = sqrt(n);
            driveDistance(distance,37);
        }

        else if(RPS.Y()<y && RPS.X()<x){
            float n = (x-RPS.X())/(y-RPS.Y());
            float degree = atan(n)*180/PI;
            turn((n*-1),25);
            n = ((x-RPS.X())*(x-RPS.X()))+((y-RPS.Y())*(y-RPS.Y()));
            float distance = sqrt(n);
            driveDistance(distance,35);
        }
    }
    if(code==1){
        if(RPS.X()<x && RPS.Y()<y ){
            float n = (x-RPS.X())/(RPS.Y()-y);
            float degree = atan(n)*180/PI;
            turn(n,25);
            n = ((x-RPS.X())*(x-(RPS.X()))+((RPS.Y()-y)*(RPS.Y()-y)));
            float distance = sqrt(n);
            driveDistance(distance,25);
        }
    }
    if(code==2){
        if(RPS.X()<x && RPS.Y()<y){
            float n =(RPS.Y()-y)/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn(n,25);
            n = ((x-RPS.X())*(x-(RPS.X()))+((RPS.Y()-y)*(RPS.Y()-y)));
            float distance = sqrt(n);
            driveDistance(distance,25);
        }
        if(RPS.Y()>y && RPS.X()<x){
            LCD.WriteLine("Y greater and X Lesser");
            float n = (RPS.Y()-y)/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn((n),25);
            n = ((x-RPS.X())*(x-RPS.X()))+((RPS.Y()-y)*(RPS.Y()-y));
            float distance = sqrt(n);
            driveDistance(distance,25);
        }
        //fuel crank case

    }
    if(code ==3){
        if(RPS.X()<x &&RPS.Y()<y){
            float n = (y-RPS.Y())/(x-RPS.X());
            float degree = atan(n)*180/PI;
            checkHeading(180+degree,1,0.6);
            n = (y-RPS.Y())*(y-RPS.Y())+(x-RPS.X())*(x-RPS.X());
            float distance = sqrt(n);
            driveDistance(distance,-25);
        }
    }
    if(code==4){
        if(RPS.Y()>y){
            float n = (RPS.Y()-y)/(x-RPS.X());
            float degree = atan(n)*180/PI;
            SD.OpenLog();
            SD.Printf("RPS Greater. Desired Degree: %f",degree);
            checkHeading((360-degree),1,0.7);
            n = ((x-RPS.X())*(x-RPS.X())+(RPS.Y()-y)*(RPS.Y()-y));
            float distance = sqrt(n);
            driveDistance(distance,30);
        }
        if(RPS.Y()<y){
            float n = (y-RPS.Y())/(x-RPS.X());
            float degree = atan(n)*180/PI;
            SD.OpenLog();
            SD.Printf("RPS less. Desired Degree: %f",degree);
            checkHeading(degree,1,0.7);
            n = ((x-RPS.X())*(x-RPS.X())+(y-RPS.Y())*(y-RPS.Y()));
            float distance = sqrt(n);
            driveDistance(distance,30);
        }
    }
    if(code==5){
        if(RPS.X()<x && RPS.Y()<y){
            float n = (y-RPS.Y())/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn(-1*degree,15);
        }
        //roboot is greater than and left of desired position
        else if(RPS.Y()>y && RPS.X()<x){
            LCD.WriteLine("above--Y greater and X Lesser");
            float n = (RPS.Y()-y)/(x-RPS.X());
            float degree = atan(n)*180/PI;
            turn(degree,15);
        }
    }



}



