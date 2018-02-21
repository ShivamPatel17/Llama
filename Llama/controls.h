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
        Controls();
        int drive(double, int);
        int driveDistance(double, int);
        int turn(int, int);

    private:

};

#endif // CONTROLS_H
