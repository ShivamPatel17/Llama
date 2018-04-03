#include <fehproteusfirmware/Libraries/FEHIO.h>
#include <fehproteusfirmware/Libraries/FEHLCD.h>
#include <fehproteusfirmware/Libraries/FEHMotor.h>
#include <fehproteusfirmware/Libraries/FEHUtility.h>
#ifndef CONTROLS_H
#define CONTROLS_H

using namespace std;


class Controls
{
    public:
        Controls(int, double);
        int drive(double, int);
        int driveDistance(double, int);
        int driveDistanceUntilBump(double,int);
        int turn(float, int);
        int straightUntilWall(int);
        void setSuperConstant(int);
        int getSuperConstant();
        void setDistanceConstant(double);
        double getDistanceConstant();
        void waitForCDS(int);
        int cdsColor();
        int driveUntilLight(int);
        int controlPanel(int);
        int turnCrank(int);
        int initializeCrank(int);
        int startMotors(int);
        int stopMotors();
        void setWrenchDegree(int,int);
        int followLine(float);
        int followWrenchLine(float);
        float getCDS();
        bool sweep(float,int);
        void masterStatus();
        void retry();
        //rps functions below
        void checkYPlus(float);
        void checkYMinus(float);
        void checkXPlus(float);
        void checkXMinus(float);
        void checkHeading(float,float,float,int);
        void XYRPS(float,float, int);



    private:

};

#endif // CONTROLS_H
