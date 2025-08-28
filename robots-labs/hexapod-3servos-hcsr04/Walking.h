/******************************************************************************
Walking.h
 *****************************************************************************/

#ifndef WALKING
#define WALKING

struct WalkingCommand
{
	int fwdBack; //Fowards is > 0, reverse is < 0,
	int leftRight; //Left is -1 to -10, right is 1 to 10
	int moveSpeed; //Slowest is 1, fastest is 100
};

extern struct WalkingCommand currentWalkCommand;

//####################################################//
//Servo Objects
//####################################################//

enum
{
	CENTER_SERVO = 0, RIGHT_SERVO, LEFT_SERVO
};

//####################################################//
//Center Servo Values & Ranges
//####################################################//

// Min/Max pulse values in microseconds to send to the servo
#define CENTER_MIN     1050
#define CENTER_MAX     1950
#define CENTER_CENTER  1500

// Legacy defines, for reference.
#define CENTER_RIGHTDOWN 1100  //Center vertical leg right side downward motion value. Lower value moves right side down further.
#define CENTER_LEFTDOWN  1900  //Center vertical leg left side downward motion value. Higher value moves the left side down further.


//####################################################//
//Right Servo Values & Ranges
//####################################################//

// Min/Max pulse values in microseconds to send to the servo
#define RIGHT_MIN     1300
#define RIGHT_MAX     1700
#define RIGHT_CENTER  1500

// Legacy defines, for reference.
#define RIGHT_FRONTSWEEP 1700  //Right horizontal legs forward sweep value. Higher value moves right legs forward further.
#define RIGHT_BACKSWEEP  1300  //Right horizontal legs backward sweep value. Lower value moves right legs backward further.


//####################################################//
//Left Servo Values & Ranges
//####################################################//

// Min/Max pulse values in microseconds to send to the servo
#define LEFT_MIN     1300
#define LEFT_MAX     1700
#define LEFT_CENTER  1500

// Legacy defines, for reference.
#define LEFT_FRONTSWEEP  1300  //Left horizontal legs forward sweep value. Lower value moves left legs forward further.
#define LEFT_BACKSWEEP   1700  //Left horizontal legs backward sweep value. Higher value moves left legs backward further.

// Placeholder for interpolation implementation.
extern int centerServoValue;   //current positional value being sent to the center servo.
extern int rightServoValue;  //current positional value being sent to the right servo.
extern int leftServoValue;  //current positional value being sent to the left servo.

// Delay values & speed for walking sequences.
extern int delayValue;
extern int delayValue2;
extern int time;

#define MIN_SPEED 1000
#define MAX_SPEED 200

//####################################################//
//Center Servo Values & Ranges
//####################################################//

extern int centerShiftRange;  //Range of this value should be 300-400. Controls the amount of vertical lift provided by the center servo.
extern int centerRightDown;
extern int centerLeftDown;


//####################################################//
//Right Servo Values & Ranges
//####################################################//

extern int rightSweepRange;  //Range of this value should be 100-200. Controls the distance of horizontal sweep motion.
extern int rightSweepRangeOffset;
extern int rightFrontSweep;
extern int rightBackSweep;


//####################################################//
//Left Servo Values & Ranges
//####################################################//

extern int leftSweepRange;  //Range of this value should be 100-200. Controls the distance of horizontal sweep motion.
extern int leftSweepRangeOffset;
extern int leftFrontSweep;
extern int leftBackSweep;

//####################################################//
//FORWARD WALKING POSES
//####################################################//

void RightUpForward(unsigned int leftOffset, unsigned int rightOffset);

void LeftUpForward(unsigned int leftOffset, unsigned int rightOffset);

void RightCenterForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);

void LeftCenterForward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);


//####################################################//
//BACKWARD WALKING POSES
//####################################################//

void RightUpBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);

void LeftUpBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);

void RightCenterBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);

void LeftCenterBackward(unsigned int leftOffset, unsigned int rightOffset, unsigned int deltaTime);


//####################################################//
// RIGHT TURN WALKING POSES
//####################################################//

void RightUpTurnRight(unsigned int deltaTime);

void LeftUpTurnRight(unsigned int deltaTime);

void RightCenterTurnRight(unsigned int deltaTime);

void LeftCenterTurnRight(unsigned int deltaTime);


//####################################################//
//LEFT TURN WALKING POSES
//####################################################//

void RightUpTurnLeft(unsigned int deltaTime);

void LeftUpTurnLeft(unsigned int deltaTime);

void RightCenterTurnLeft(unsigned int deltaTime);

void LeftCenterTurnLeft(unsigned int deltaTime);


//####################################################//
//WALK CYCLE FUNCTIONS
//####################################################//

void TurnRight(unsigned int cycle, unsigned int speed);

void TurnLeft(unsigned int cycle, unsigned int speed);

void WalkForward(unsigned int cycle, unsigned int speed);

void WalkBackward(unsigned int cycle, unsigned int speed);

void DriveForward(int turn, unsigned int cycle, unsigned int speed);

void DriveBackward(int turn, unsigned int cycle, unsigned int speed);

//####################################################//
//SERVO INTERPOLATION FUNCTIONS
//####################################################//

void SetServo(unsigned int deltaTime);

void SetServoCenter(unsigned int deltaTime);

void initializeServos();

//####################################################//
//EXTRA FUNCTIONS
//####################################################//

void Wiggle( unsigned int cycle, unsigned int speed );
void Shiver( unsigned int cycle, unsigned int speed );
void Giggle( unsigned int cycle, unsigned int speed );
void Wave( unsigned int cycle, unsigned int speed );

#endif
