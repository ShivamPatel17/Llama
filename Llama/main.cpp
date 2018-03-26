
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include "controls.h">
#include "performance.h"
#include "proteusinterface.h"
#include <FEHServo.h>
#include <FEHSD.h>


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

//optosensor
AnalogInputPin opto(FEHIO::P3_0);

//idk why i need this
Controls ctrl(680,0.95);

int main(void)
{
    /*
    while(true){
        LCD.WriteLine(cds.Value());
        Sleep(0.5);
        LCD.Clear(BLACK);
    }*/
    //initialize control class and performance
    Controls ctrl(680,0.95);
    Performance perf;
    ProteusInterface pi;
    pi.homepage();
}
