#include "performance.h"
#include "controls.h"

Performance::Performance()
{

}

void Performance::test1(){
     Controls ctrl;

     //first go straight 8 inches
     ctrl.driveDistance(7.0,35);\

     //turn right 45
     ctrl.turn(40,35);

     //go straigh for 12 inches
     ctrl.driveDistance(12.0,50);

     //turn left 45
     ctrl.turn(-35,35);

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
