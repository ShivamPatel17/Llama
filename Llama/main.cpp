#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include "controls.h">


//initialize motors
FEHMotor rightMotor(FEHMotor::Motor0, 9.0);
FEHMotor leftMotor(FEHMotor::Motor1, 9.0);

//initialize buttons
DigitalInputPin rightBumper(FEHIO::P0_0);
DigitalInputPin leftBumper(FEHIO::P3_7);

//initialize encoders
DigitalEncoder rightEncoder(FEHIO::P1_7);
DigitalEncoder leftEncoder(FEHIO::P2_0);


int main(void)
{
    //initialize control class
    Controls ctrl;


    //clear screen
    LCD.Clear( FEHLCD::Green);
    LCD.SetFontColor( FEHLCD::White );


    /********************
     * Time to do shit!
     ********************/
    LCD.DrawVerticalLine(160,0,239);
    LCD.DrawHorizontalLine(120,0,320);
    float x, y;

    while(true){


       //waiting for either switch to be pressed
       while(rightBumper.Value() && leftBumper.Value()){}
       while(!rightBumper.Value() || !leftBumper.Value()){}

       if(!rightBumper.Value()){
         LCD.Clear( FEHLCD::Blue);
         Sleep(1.0);
         LCD.WriteLine("this should drive");
         ctrl.drive(1.5,50);
       }


       if(!leftBumper.Value()){
           LCD.Clear( FEHLCD::Red);
           Sleep(1.0);
           LCD.WriteLine("this should drive a set distance");
           ctrl.driveDistance(6.0,35);
       }

    }


}
