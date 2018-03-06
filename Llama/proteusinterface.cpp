#include "proteusinterface.h"
#include <FEHLCD.h>
#include <FEHIO.h>
#include <FEHUtility.h>
#include "controls.h">
#include "performance.h"


extern Controls ctrl;

ProteusInterface::ProteusInterface()
{
  LCD.Clear(BLUE);
  int super = 680;
  double distance = 0.95;
  ctrl = Controls(super,distance);
}

/***********************
 *
 * homepage to do shit
 *
 *
 * *********************/
void ProteusInterface::homepage(){
    Performance perf;

    LCD.Clear(ORANGE);
    LCD.DrawRectangle(0,0,180,40);
    LCD.DrawRectangle(0,41,180,40);
    LCD.DrawRectangle(0,82,180,40);
    LCD.DrawRectangle(0,123,180,40);
    LCD.DrawRectangle(0,164,180,40);

    LCD.WriteAt("Turn Config",2,2);
    LCD.WriteAt("Drive Config",2,42);
    LCD.WriteAt("clockwise",2,82);
    LCD.WriteAt("counterclock",2,123);
    LCD.WriteAt("FinalComp",2,164);

    //dummys
    float x,y;

    while(true){
        while(LCD.Touch(&x,&y)){
            if((x>0&&x<80)&&(y>0&&y<40)){
                configureTurn();
            }
            if((x>0&&x<80)&&(y>40&&y<80)){
                configureDrive();
            }
            if((x>0&&x<80)&&(y>80&&y<120)){
                ctrl.turnCrank(1);
            }
            if((x>0&&x<80)&&(y>123&&y<164)){
                ctrl.turnCrank(2);
            }
            if((x>0&&x<80)&&(y>164&&y<204)){
                perf.finalTest();
            }

        }
    }
}

/*****************
 * Configuration for the turning
 * ************/
int ProteusInterface::configureTurn(){
    LCD.Clear(BLUE);

    float x,y;

    //draws the box to exit the configuration screen
    LCD.DrawRectangle(260,200,60,40);
    LCD.WriteAt("Done",262,202);

    //draws the box to increase the constant
    LCD.DrawRectangle(80,60,100,50);
    LCD.WriteAt("UP",82,62);

    //draws the box to decrease the constant
    LCD.DrawRectangle(80,115,100,50);
    LCD.WriteAt("DOWN",82,117);

    //draws the test box
    LCD.DrawRectangle(200,60,80,100);
    LCD.WriteAt("Right",202,62);
    LCD.WriteAt("Left",202,112);

    while(true){
        LCD.WriteAt(ctrl.getSuperConstant(),0,0);
        while(LCD.Touch(&x,&y)){
            //done button
            if(x>280&&y>200){
                homepage();
            }
            //increment constant
            if((x>80&&x<180)&&(y>60&&y<110)){
                ctrl.setSuperConstant(ctrl.getSuperConstant()+5);
                Sleep(0.25);
            }
            //decrement constant
            if((x>80&&x<180)&&(y>115&&y<165)){
                ctrl.setSuperConstant(ctrl.getSuperConstant()-5);
                Sleep(0.25);
            }
            if((x>200&&x<280)&&(y>60&&y<110)){
                Sleep(0.25);
                ctrl.turn(90,35);
            }
            if((x>200&&x<280)&&(y>110&&y<160)){
                Sleep(1.0);
                ctrl.turn(-90,35);
            }
        }

    }

}
/******************************
 * Configure the drive distance
 * ****************************/
int ProteusInterface::configureDrive(){
    LCD.Clear(BLUE);

    float x,y;

    //draws the box to exit the configuration screen
    LCD.DrawRectangle(260,200,60,40);
    LCD.WriteAt("Done",262,202);

    //draws the box to increase the constant
    LCD.DrawRectangle(80,60,100,50);
    LCD.WriteAt("UP",82,62);

    //draws the box to decrease the constant
    LCD.DrawRectangle(80,115,100,50);
    LCD.WriteAt("DOWN",82,117);

    //draws the test box
    LCD.DrawRectangle(200,60,80,100);
    LCD.WriteAt("Drive",202,62);


    while(true){
        LCD.WriteAt(ctrl.getDistanceConstant(),0,0);
        while(LCD.Touch(&x,&y)){
            //done button
            if(x>280&&y>200){
                homepage();
            }
            //increment constant
            if((x>80&&x<180)&&(y>60&&y<110)){
                ctrl.setDistanceConstant(ctrl.getDistanceConstant()+0.01);
                Sleep(0.25);
            }
            //decrement constant
            if((x>80&&x<180)&&(y>115&&y<165)){
                ctrl.setDistanceConstant(ctrl.getDistanceConstant()-0.01);
                Sleep(0.25);
            }
            if((x>200&&x<280)&&(y>60&&y<160)){
                Sleep(0.5);
                ctrl.driveDistance(10.0,35);
            }

        }

    }
}
