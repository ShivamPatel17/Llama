#include "performance.h"
#include "controls.h"
#include "proteusinterface.h"
#include <FEHSD.h>
#include "fehproteusfirmware/Libraries/FEHRPS.h"


Performance::Performance()
{

}
void Performance::fuelCrank(){
    Controls ctrl(680,0.95);
}

//test 3 is basically the final run//test 4 also//final run
void Performance::test3(){

    ProteusInterface pi;
    Controls ctrl(680,0.95);

    float colorX = 25.7;
    float colorY=20.1;
    ctrl.setWrenchDegree(149,10);
//hello shivam this is your robot speaking to you
    //Thank you for coding me gently <3
    RPS.InitializeTouchMenu();

    LCD.Clear(PINK);

    LCD.WriteLine("Wish me luck! ;)");


    //waits to read a color
    ctrl.waitForCDS(1.0);

    ctrl.turn(5,25);

    Sleep(0.2);
    ctrl.driveDistance(14.8,25);


    ctrl.turn(90,25);


    ctrl.setWrenchDegree(5,2);


    ctrl.driveDistance(2.3,25);
    ctrl.checkHeading(178.5,1.5,0.4,30);

    ctrl.followLine(2.8);



    //PICK UP WRENCH
    ctrl.setWrenchDegree(105,5);

    ctrl.driveDistance(3,-25);

    //ctrl.retry();

    ctrl.turn(-45,25);

    ctrl.driveDistance(7.0,25);

    ctrl.turn(-40,25);

    ctrl.straightUntilWall(30);

    //backup a bit
    ctrl.driveDistance(2.0,-35);

    //turn to flick swit
    ctrl.turn(-31,35);

    //turn back
    ctrl.turn(20,35);

    //straight until wall
    ctrl.straightUntilWall(36);

    //back up and turn
    ctrl.driveDistance(1.0,-25);
    Sleep(0.1);
    ctrl.turn(5,25);

    //back up like 14 inches
    ctrl.driveDistance(12.0,-35);

    Sleep(0.5);
    LCD.Clear(WHITE);

    //turnn left
    ctrl.turn(-85,35);
    ctrl.checkHeading(0,2,0.5,30);
    ctrl.driveDistance(12.0,35);
    ctrl.XYRPS(colorX,colorY,0);

    //determinelight color
    int color = ctrl.cdsColor();
    if(color==-1){
        LCD.Clear(BLUE);
    }
    else{
        LCD.Clear(RED);
    }

    if(RPS.Heading()>300){
        ctrl.turn(-1*(360-RPS.Heading()),20);
    }
    else{
        ctrl.turn(RPS.Heading(),20);
    }
    ctrl.checkHeading(0,2,1,30);
    ctrl.straightUntilWall(35);
    ctrl.driveDistance(5.0,-25);
    ctrl.turn(-40,25);
    ctrl.driveDistance(5.0,25);
    ctrl.turn(35,25);
    ctrl.straightUntilWall(25);

    ctrl.controlPanel(color);

    ctrl.driveDistance(1.0,-25);
    ctrl.turn(-90,25);
    ctrl.straightUntilWall(25);
    ctrl.driveDistance(0.15,-25);
    ctrl.turn(-80,25);
    ctrl.checkHeading(77.0,0.8,0.5,30);

    ctrl.driveDistance(22.0,40);
    Sleep(1.0);

    ctrl.turn(-35,25);
    ctrl.initializeCrank(RPS.FuelType());
    ctrl.XYRPS(18.7,56.0,0);
    ctrl.checkHeading(133,2,1,30);
    ctrl.driveDistance(5.0,20);

    bool linefound = ctrl.sweep(3.0);
    ctrl.setWrenchDegree(45,2);
    if(!linefound){
        //do something if the line isn't found
    }
    ctrl.followWrenchLine(5.0);
    /*before line following
    ctrl.XYRPS(22.2,52,0);
    ctrl.checkHeading(135,1.4,0.9);
    ctrl.XYRPS(16.2,56.0,0);
    ctrl.checkHeading(135,0.8,0.8);
    */


    ctrl.drive(1.5,35);
    ctrl.setWrenchDegree(5,5);
    ctrl.driveDistance(10.0,-35);
    ctrl.turn(-93,25);
    ctrl.XYRPS(20.2,58.9,3); 
    ctrl.XYRPS(24,62.0,3);
    ctrl.setWrenchDegree(150,0);
    LCD.Clear(RED);
    ctrl.checkHeading(225.0,1.0,0.5,45);
    Sleep(0.1);
    //maybe crank it up
    ctrl.startMotors(-30);
    Sleep(1);

    ctrl.turnCrank(RPS.FuelType());
    Sleep(1.7);
    ctrl.stopMotors();
    ctrl.driveDistance(0.22,15);
    Sleep(0.2);
    ctrl.driveDistance(14.0,40);
    ctrl.turn(-90,35);
    ctrl.checkHeading(315,1,0.7,30);
    ctrl.driveDistanceUntilBump(20.8,35);
    ctrl.turn(50,25);
    ctrl.checkHeading(273,3,0.7,30);
    ctrl.checkYMinus(19.0);

    ctrl.turn(90,35);
    ctrl.checkHeading(181,2,0.7,30);
    ctrl.driveDistance(9.0,35);
    ctrl.turn(90,35);
    ctrl.checkHeading(90,2.0,1.0,30);
    ctrl.driveDistance(10.0,40);




    //ctrl.checkXMinus();
    LCD.WriteLine("End of ptest 2 code");
    Sleep(2.0);
    pi.homepage();



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
