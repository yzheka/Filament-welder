#include <EEPROM.h>
#include "GyverPID.h"
#include <thermistor.h>
#include "config.h"

class HeaterController{
private:
    thermistor therm;
    GyverPID regulator;
    float currentTemp;
    bool enabled;
    unsigned long tmr;
    int lastVal=LOW;
    bool fanEnabled;

    void analogWriteHeater(int value){
        if(value!=lastVal){
            lastVal=value;
            analogWrite(HEATER_PIN,value);
        }
    }

    float getCurrentTemp(){
        if (millis() - tmr > 100) {
            tmr = millis();
            return readTemp();
        }
        return currentTemp;
    }

public:
    HeaterController():
    therm(TEMP_PIN,1),
    regulator(CFG_PID_P, CFG_PID_I, CFG_PID_D){
        pinMode(HEATER_PIN,INPUT);
        pinMode(FAN_PIN,INPUT);
        regulator.setMode(ON_RATE);
    }

    float readTemp(){
        tmr = millis();
        currentTemp=therm.analog2temp();
        return currentTemp;
    }

    void enable(){
        enabled=true;
    }

    void disable(){
        enabled=false;
        analogWrite(HEATER_PIN,LOW);
    }

    void setFanEnabled(bool enabled){
        if(fanEnabled!=enabled){
            digitalWrite(FAN_PIN,enabled?HIGH:LOW); 
            fanEnabled=enabled;
        }
    }

    bool isEnabled(){
        return enabled;
    }

    void tick(float targetTemp){
        if(enabled){
            auto temp=getCurrentTemp();
            regulator.setpoint = targetTemp;
            regulator.input = temp;
            analogWriteHeater(regulator.getResultTimer());
        } else {
            analogWriteHeater(LOW);
        }
    }

    bool heated(float targetTemp){
        return abs(getCurrentTemp()-targetTemp)<=1;
    }
};