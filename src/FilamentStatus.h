#include "config.h"
#include "Arduino.h"

class FilamentStatus{

private:
    int status1,status2;

public:
    enum Side{LEFT,RIGHT};
    FilamentStatus(){
        pinMode(ENDSTOP_LEFT_PIN,INPUT_PULLUP);
        pinMode(ENDSTOP_RIGHT_PIN,INPUT_PULLUP);
    }

    bool isLoaded(Side side){
        switch (side){
            case Side::LEFT: return digitalRead(ENDSTOP_LEFT_PIN)==LOW;
            case Side::RIGHT: return digitalRead(ENDSTOP_RIGHT_PIN)==LOW;
        }
        return false;
    }

    bool isLoaded(){
        return isLoaded(Side::LEFT)&&isLoaded(Side::RIGHT);
    }
};