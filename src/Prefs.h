#include "config.h"
#include <EEPROM.h>
#include "Arduino.h"

struct Prefs{
    private:
        float preheatTemp=MIN_TEMP;
        float weldDist=WELD_DIST_MIN/STEPS_PER_MM;
        int weldTime=MELT_TIME_MIN;

    public:
        void setPreheatTemp(float temp){
            preheatTemp=temp;
            if(preheatTemp>MAX_TEMP)preheatTemp=MAX_TEMP;
            else if(preheatTemp<MIN_TEMP)preheatTemp=MIN_TEMP;
        }

        float getPreheatTemp(){
            return preheatTemp;
        }

        void setWeldDist(float dist){
            float min=WELD_DIST_MIN/STEPS_PER_MM;
            float max=WELD_DIST_MAX/STEPS_PER_MM;
            weldDist=dist;
            if(weldDist>max)weldDist=max;
            else if(weldDist<min)weldDist=min;
        }

        float getWeldDist(){
            return weldDist;
        }

        void setWeldTime(int time){
            weldTime=time;
            if(weldTime>MELT_TIME_MAX)weldTime=MELT_TIME_MAX;
            else if(weldTime<MELT_TIME_MIN)weldTime=MELT_TIME_MIN;
        }

        int getWeldTime(){
            return weldTime;
        }

        static void save(Prefs prefs){
            EEPROM.put(0,prefs);
        }

        static Prefs get(){
            Prefs prefs;
            EEPROM.get(0,prefs);
            if(
                isnan(prefs.weldDist)||
                isnan(prefs.preheatTemp)||
                prefs.weldTime<0
            )return Prefs();
            else return prefs;
        }
};
