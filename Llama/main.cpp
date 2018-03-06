
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include "controls.h">
#include "performance.h"
#include "proteusinterface.h"
#include <FEHServo.h>


//initialize motors
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
FEHMotor leftMotor(FEHMotor::Motor1, 9.0);

//initialize buttons
DigitalInputPin rightBumper(FEHIO::P0_7);
DigitalInputPin leftBumper(FEHIO::P3_7);

//initialize encoders
DigitalEncoder rightEncoder(FEHIO::P1_3);
DigitalEncoder leftEncoder(FEHIO::P2_4);

//initialize cds
AnalogInputPin cds(FEHIO::P2_1);

//initialze servos! :) 2/28/2018
FEHServo wrenchServo(FEHServo::Servo0);
FEHServo fuelServo(FEHServo::Servo7);

//constant
int superConstant;
double distanceConstant;

//idk why i need this
Controls ctrl(680,0.95);

int main(void)
{

    //initialize control class and performance
    Controls ctrl(680,0.95);
    Performance perf;
    ProteusInterface pi;
    pi.homepage();


/*
    //clear screen
    LCD.Clear( FEHLCD::Green);
    LCD.SetFontColor( FEHLCD::White );

    //wait for cds to pick up some light then start the performance test
   while(true){

       while(rightBumper.Value() || leftBumper.Value()){
           LCD.WriteLine(cds.Value());
           Sleep(1.0);
           LCD.Clear(BLACK);
       }
       //waiting for either switch to be pressed
       while(rightBumper.Value()&& leftBumper.Value()){}

       LCD.Clear(PINK);
       Sleep(1.0);

       //START! :)
       while(cds.Value()>1.2){}
       perf.test1();
       Sleep(5.0);
       LCD.WriteLine("RESET!!");

   }
   */



}
