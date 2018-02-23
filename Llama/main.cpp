
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include "controls.h">
#include "performance.h"


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


int main(void)
{
    //initialize control class and performance
    Controls ctrl;
    Performance perf;




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



}
