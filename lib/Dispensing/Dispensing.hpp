#ifndef __DISPENSING
#define __DISPENSING

#include <Arduino.h>
#include <AccelStepper.h>
#include <Mqtt.hpp>
#include <ArduinoJson.h>

// #define DRIVER_TYPE DRV8825
#define DRIVER_TYPE TMC2208

#if defined(DRIVER_TYPE) && DRIVER_TYPE == TMC2208
    #define MS1_STATE HIGH
    #define MS2_STATE HIGH
    #define DIRECTION_INVERT true
#elif defined(DRIVER_TYPE) && DRIVER_TYPE == DRV8825
    #define MS1_STATE LOW
    #define MS2_STATE LOW
    #define DIRECTION_INVERT false
#endif

#define MICROSTEPS 16 // 1/16 microsteps
#define STEPS_PER_REV 200 // 1.8 degree per step

#define X_OFF_SET 600 // 600 steps
#define Z_OFF_SET 3000 // 3000 steps
#define ZP_OFF_SET 3000 // 3000 steps

#define X_ACCELERATION 5000 // 5000 steps/s^2
#define Z_ACCELERATION 5000 // 5000 steps/s^2
#define ZP_ACCELERATION 5000 // 5000 steps/s^2

#define X_MAX_SPEED 7000 // 7000 steps/s
#define X_HOMING_SPEED 4000 // 4000 steps/s
#define X_SECOND_HOMING_SPEED 2000 // 2000 steps/s

#define Z_MAX_SPEED 7000 // 7000 steps/s
#define Z_HOMING_SPEED 4000 // 4000 steps/s
#define Z_SECOND_HOMING_SPEED 2000 // 2000 steps/s

#define ZP_MAX_SPEED 7000 // 7000 steps/s
#define ZP_HOMING_SPEED 4000 // 4000 steps/s
#define ZP_SECOND_HOMING_SPEED 2000 // 2000 steps/s

#define X_HOME_POS (STEPS_PER_REV * MICROSTEPS) / 4 // 800 steps 
#define Z_HOME_POS (STEPS_PER_REV * MICROSTEPS) * 2 // 6400 steps
#define ZP_HOME_POS (STEPS_PER_REV * MICROSTEPS) * 2 // 6400 steps

#define VIAL_X_POS 0 // stepper X on vial position
#define VIAL_Z_POS 0 // stepper Z on vial position

#define CAPSULE_1_X_POS 0 // stepper X on capsule 1 position
#define CAPSULE_2_X_POS 0 // stepper X on capsule 2 position
#define CAPSULE_3_X_POS 0 // stepper X on capsule 3 position
#define CAPSULE_4_X_POS 0 // stepper X on capsule 4 position
#define CAPSULE_5_X_POS 0 // stepper X on capsule 5 position

#define CAPSULE_Z_POS 0 // stepper Z on capsule position

#define SYRINGE_MIN_POS 38400 // syringe on empty position or 0 uL volume
#define SYRINGE_MAX_POS 6400 // syringe on full position or 50 uL volume

#define SYRINGE_MIN_VOLUME 0 // 0 uL
#define SYRINGE_MAX_VOLUME 50 // 50 uL

#define CAPSULE_MIN_QTY 0 // 0 capsule
#define CAPSULE_MAX_QTY 5 // 5 capsules

class Dispensing
{
private:
    AccelStepper stepperX;
    AccelStepper stepperZ;
    AccelStepper stepperZp;

    byte stepPinX;
    byte dirPinX;
    byte enablePinX;
    byte limitSwitchPinX;
    byte ledPinX;

    byte stepPinZ;
    byte dirPinZ;
    byte enablePinZ;
    byte limitSwitchPinZ;
    byte ledPinZ;

    byte stepPinZp;
    byte dirPinZp;
    byte enablePinZp;
    byte limitSwitchPinZp;
    byte ledPinZp;

    byte ms1Pin;
    byte ms2Pin;

    byte solenoidPin;

    int volume = 0;
    int capsuleQty = 0;
    bool status = true;

    long sryingeFillingPosition = map(volume, SYRINGE_MIN_VOLUME, SYRINGE_MAX_VOLUME, SYRINGE_MIN_POS, SYRINGE_MAX_POS);
    long capsulePositionX[5] = {CAPSULE_1_X_POS, CAPSULE_2_X_POS, CAPSULE_3_X_POS, CAPSULE_4_X_POS, CAPSULE_5_X_POS}; 
    
    bool fillSyringe();
    bool dispenseSyringe();
    bool emptySyringe();
    
    void homeX();
    void homeZ();
    void homeZp();

    bool runToVialX();
    bool runToVialZ();

    bool runToHomeX();
    bool runToHomeZ();
    bool runToHomeZp();

    bool runToCapsuleX(int &i);
    bool runToCapsuleZ(int &i);

    bool remainCapsule(int &i);

    void dispensing();
    void dummyDispensing();

public:
    Dispensing(const byte pinsX[], const byte pinsZ[], const byte pinsZp[], const byte msPins[], const byte solenoidPin);
    
    // proccess methods
    void init();
    void homing();
    void dummyHoming();
    bool check(Mqtt &mqtt);
    bool start();
    bool getDispensingStatus();

    // test methods
    void ledTest();
    void solenoidTest();
    void allLedAndSolenoidTest();

    // calibration methods
    void readLimitSwitch();
    void runAndFindPosX(long &pos);
    void runAndFindPosZ(long &pos);
    void runAndFindPosZp(long &pos);
    void serialCalibrationX();
    void serialCalibrationZ();
    void serialCalibrationZp();
};

#endif