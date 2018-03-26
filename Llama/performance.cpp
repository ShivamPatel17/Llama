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

    ctrl.setWrenchDegree(149,10);

    RPS.InitializeTouchMenu();

    LCD.Clear(PINK);

    LCD.WriteLine("Wish me luck!");


    //waits to read a color
    ctrl.waitForCDS(1.0);

    ctrl.turn(5,25);

    Sleep(0.2);
    ctrl.driveDistance(14.8,25);


    ctrl.turn(89,25);


    ctrl.setWrenchDegree(10,2);


    ctrl.checkXMinus(10.5);
    ctrl.checkHeading(180,1.5,0.4);

    ctrl.followLine(2.5);



    //PICK UP WRENCH
    ctrl.setWrenchDegree(105,5);

    ctrl.driveDistance(3,-25);

    ctrl.turn(-45,25);

    ctrl.driveDistance(7.0,25);

    ctrl.turn(-40,25);

    ctrl.straightUntilWall(30);

    //backup a bit
    ctrl.driveDistance(2.0,-35);

    //turn to flick swit
    ctrl.turn(-33,35);

    //turn back
    ctrl.turn(20,35);

    //straight until wall
    ctrl.straightUntilWall(25);

    //back up and turn
    ctrl.driveDistance(1.0,-25);
    Sleep(0.5);
    ctrl.turn(5,25);

    //back up like 14 inches
    ctrl.driveDistance(12.0,-35);

    Sleep(0.5);

    //turnn left
    ctrl.turn(-90,35);
    ctrl.checkHeading(0,2,1);
    ctrl.driveDistance(12.0,35);
    ctrl.XYRPS(25.7,20,0);

    ctrl.driveUntilLight(20);

    //determinelight color
    int color = ctrl.cdsColor();
    if(color==-1){
        LCD.Clear(BLUE);
    }
    else{
        LCD.Clear(RED);
    }
    ctrl.XYRPS(RPS.X()+1,RPS.Y(),5);
    ctrl.checkHeading(0,2,1);
    ctrl.straightUntilWall(25);
    ctrl.driveDistance(5.0,-25);
    ctrl.turn(-40,25);
    ctrl.driveDistance(5.0,25);
    ctrl.turn(35,25);
    ctrl.straightUntilWall(25);

    ctrl.controlPanel(color);

    ctrl.driveDistance(1.0,-25);
    ctrl.turn(-90,25);
    ctrl.straightUntilWall(25);
    ctrl.driveDistance(0.5,-25);
    ctrl.turn(-83,25);
    ctrl.checkHeading(80.0,0.8,0.5);

    ctrl.driveDistance(19.0,40);
    Sleep(1.0);
    ctrl.checkYPlus(44.0);

    ctrl.turn(-40,25);
    ctrl.XYRPS(22.2,52,0);
    ctrl.checkHeading(135,1.4,0.9);
    ctrl.XYRPS(15.7,56.8,0);
    ctrl.checkHeading(135,0.8,0.8);

    ctrl.driveDistance(5.0,25);
    ctrl.setWrenchDegree(45,2);


    ctrl.drive(1.5,35);
    ctrl.setWrenchDegree(10,5);
    ctrl.driveDistance(10.0,-35);
    ctrl.turn(-85,25);
    ctrl.XYRPS(20.2,58.9,3);
    SD.OpenLog();
    SD.Printf("X: %f Y: %f", RPS.X(),RPS.Y());
    ctrl.XYRPS(24,62.0,3);
     SD.Printf("X: %f Y: %f", RPS.X(),RPS.Y());
    SD.CloseLog();
    ctrl.initializeCrank(RPS.FuelType());
    ctrl.setWrenchDegree(150,0);

    //x:25.4 y:63.8
    ctrl.checkHeading(225,1,0.5);
    ctrl.startMotors(-30);
    Sleep(0.2);


    ctrl.turnCrank(RPS.FuelType());
    Sleep(1.7);
    ctrl.stopMotors();
    ctrl.driveDistance(0.22,15);
    Sleep(0.2);
    ctrl.driveDistance(14.0,40);
    ctrl.turn(-90,35);
    ctrl.checkHeading(315,1,0.7);
    ctrl.driveDistance(15.2,35);
    ctrl.turn(45,25);
    ctrl.checkHeading(273,3,0.7);
    ctrl.driveDistance(22.0,40);
    ctrl.turn(95,35);
    ctrl.checkHeading(180,3,1.2);
    ctrl.driveDistance(9.0,35);
    ctrl.turn(87,35);
    ctrl.driveDistance(10.0,25);




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
