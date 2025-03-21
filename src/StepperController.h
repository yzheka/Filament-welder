#define GS_FAST_PROFILE 10
//#define GS_NO_ACCEL

#include "config.h"
#include "GyverStepper2.h"

class StepperController{
private:
    GStepper2<STEPPER2WIRE> stepperLeft;
    GStepper2<STEPPER2WIRE> stepperRight;

    void run(){
        bool s1,s2;
        do{
            s1=stepperLeft.tick();
            s2=stepperRight.tick();
        } while (s1||s2);
    }

public:
    StepperController():
    stepperLeft(200, MOTOR1_STEP_PIN, MOTOR1_DIR_PIN, MOTOR1_ENABLE_PIN),
    stepperRight(200, MOTOR2_STEP_PIN, MOTOR2_DIR_PIN, MOTOR2_ENABLE_PIN){
        stepperLeft.setAcceleration(MOTOR_ACCELERATION);
        stepperRight.setAcceleration(MOTOR_ACCELERATION);
        stop();
    }

    void load(double speed,int dist){
        stepperLeft.enable();
        stepperRight.enable();
        stepperLeft.setMaxSpeed(speed);
        stepperRight.setMaxSpeed(speed);
        stepperLeft.setTarget(dist,RELATIVE);
        stepperRight.setTarget(-dist,RELATIVE);
        run();
    }

    void unload(double speed,int32_t dist){
        stepperLeft.enable();
        stepperRight.enable();
        stepperLeft.setMaxSpeed(speed);
        stepperRight.setMaxSpeed(speed);
        stepperLeft.setTarget(dist,RELATIVE);
        stepperRight.setTarget(dist,RELATIVE);
        run();
    }

    void unload(double speed){
        stepperLeft.enable();
        stepperRight.enable();
        stepperLeft.setSpeed(speed);
        stepperRight.setSpeed(speed);
        stepperLeft.tick();
        stepperRight.tick();
    }

    void stop(){
        stepperLeft.brake();
        stepperRight.brake();
        stepperLeft.reset();
        stepperRight.reset();
        stepperLeft.disable();
        stepperRight.disable();
    }
};