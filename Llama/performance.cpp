#include "performance.h"
#include "controls.h"
#include "proteusinterface.h"
#include "fehproteusfirmware/Libraries/FEHRPS.h"

Performance::Performance()
{

}
void Performance::fuelCrank(){
    Controls ctrl(680,0.95);
}

void Performance::test2(){
    ProteusInterface pi;
    Controls ctrl(680,0.95);

    LCD.WriteLine("Ptest 2!");


    //waits to read a color
    ctrl.waitForCDS(1.0);

    //drive
    ctrl.driveDistance(3.5,35);

    //turn a little bit
    ctrl.turn(-45,35);

    //turn until the light
    ctrl.driveUntilLight(35);

    Sleep(1.0);

    //determinelight color
    int color = ctrl.cdsColor();
    if(color==-1){
        LCD.Clear(BLUE);
    }
    else{
        LCD.Clear(RED);
    }

    //turn a little left and then align yo self with the wall
    ctrl.turn(-50,35);
    Sleep(0.2);
    ctrl.straightUntilWall(25);

    //hit the control panel
    ctrl.controlPanel(color);

    //for blue!
    if(color==-1){
        ctrl.driveDistance(2.0,-25);
        ctrl.turn(90,25);
        ctrl.driveDistance(21.0,35);
    }
    if(color==1){
        ctrl.driveDistance(2.0,-25);
        ctrl.turn(90,25);
        ctrl.driveDistance(17.0,35);
    }

    ctrl.driveDistance(12.0,-35);
    ctrl.turn(88,25);
    ctrl.drive(1.5,35);

    LCD.WriteLine("End of ptest 2 code");
    Sleep(5.0);
    pi.homepage();










}

void Performance::test1(){
     Controls ctrl(680,0.95);

     //first go straight 8 inches
     ctrl.driveDistance(7.0,35);\

     //turn right 45
     ctrl.turn(45,35);

     //go straigh for 12 inches
     ctrl.driveDistance(12.0,50);

     //turn left 45
     ctrl.turn(-45,35);

     LCD.WriteLine("should go until wall is hit");
     //go until wall
     ctrl.straightUntilWall(35);

     //backup a bit
     ctrl.drive(3.0,-35);

     //turn to flick swit
     ctrl.turn(-33,35);

     //turn back
     ctrl.turn(35,35);

     //straight until wall
     ctrl.straightUntilWall(30);

     //back up like 14 inches
     ctrl.driveDistance(15.5,-35);

     //turnn left
     ctrl.turn(-84,35);

     //go straight until wall
     ctrl.straightUntilWall(35);

     //back up like 6 inches
     ctrl.driveDistance(4.5,-35);

     //turn right and hit the panell
     ctrl.turn(84,35);

     //hit panel
     ctrl.drive(1.0,35);

     //back up
     ctrl.driveDistance(1.0,-35);

     //turn around
     ctrl.turn(-84,35);

     //go to align with wall
     ctrl.straightUntilWall(35);

     //backup two inches
     ctrl.driveDistance(0.5,-35);

     //turn left
     ctrl.turn(-84,35);

     //go up ramp
     ctrl.driveDistance(25.0,50);

}
void Performance::finalTest(){
    ProteusInterface pi;
    Controls ctrl(680,0.95);

    RPS.InitializeTouchMenu();

    LCD.Clear(PINK);

    LCD.WriteLine("Wish me luck!");


    //waits to read a color
    ctrl.waitForCDS(1.0);

    ctrl.turn(5,25);

    Sleep(0.2);
    ctrl.driveDistance(15.0,25);


    ctrl.turn(89,25);

    ctrl.driveDistance(4.0,25);


    ctrl.turnCrank(2);

    ctrl.driveDistance(2.0,-25);

    ctrl.turn(-90,25);

    ctrl.straightUntilWall(25);

    while(true){
        LCD.WriteLine("X: ");
        LCD.Write(RPS.X());
        LCD.WriteLine("Y: ");
        LCD.Write(RPS.Y());
        LCD.WriteLine("head: ");
        LCD.Write(RPS.Heading());
        Sleep(1.0);
        LCD.Clear(BLACK);
    }




}
