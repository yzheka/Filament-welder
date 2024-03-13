#include "config.h"
#include "EncButton.h"
#include <GyverTimers.h>
#include "StepperController.h"
#include "HeaterController.h"
#include "LcdController.h"
#include "FilamentStatus.h"
#include "Prefs.h"

Button btnUp(BTN_UP_PIN);
Button btnDown(BTN_DOWN_PIN);
Button btnOk(BTN_OK_PIN);

StepperController stepperController;
HeaterController heaterController;
LcdController lcdController;
FilamentStatus filamentStatus;
Prefs prefs=Prefs::get();
byte editVal=0;
bool editing=false;

void printCurrentTemp(float target){
  float temp=heaterController.readTemp();
  String prefix;
  if(temp>=100&&target>=100)prefix=" ";
  else if(temp<100&&target<100)prefix="  ";
  else prefix=" ";
  lcdController.write(5,prefix+String(temp,0)+"C>"+String(target,0)+"C",2);
}

void unloadFilament(){
  stepperController.stop();
  heaterController.disable();
  lcdController.clear();
  bool flag=false;
  while (heaterController.readTemp()>COOLDOWN_TEMP){
    heaterController.setFanEnabled(true);
    lcdController.write(3,"    Cooling...");
    printCurrentTemp(COOLDOWN_TEMP);
  }
  lcdController.clear();
  heaterController.setFanEnabled(false);
  while (filamentStatus.isLoaded()&&!btnUp.read()&&!btnDown.read()){
    lcdController.write(2,"Press any key");
    lcdController.write(3,"to unload");
  };
  while (filamentStatus.isLoaded()){
    if(!flag){
      lcdController.write(3,"Unloading...");
      flag=true;
    }
    stepperController.unload(MAX_SPEED);
  }
  if(flag){
    stepperController.unload(MAX_SPEED/2,LOAD_DIST);
    stepperController.stop();
    delay(1000);
  }
}

void editValue(byte index,bool increment){
  float value=increment?1:-1;
  switch (index){
  case 0:
    prefs.setPreheatTemp(prefs.getPreheatTemp()+value);
    break;

  case 1:
    prefs.setWeldDist(prefs.getWeldDist()+value/10);
    break;

  case 2:
    prefs.setWeldTime(prefs.getWeldTime()+value);
    break;
  
  default:
    break;
  }
}

void setEditVal(bool next){
  byte value=next?1:-1;
  editVal+=value;
  if(editVal>2)editVal=0;
}

String getIndicator(byte editV){
  if(editing&&editVal==editV)return "* ";
  else if(editVal==editV)return "> ";
  else return "  ";
}

void selectTemperature(){
  heaterController.disable();
  lcdController.clear();
  stepperController.stop();
  while(editing||!filamentStatus.isLoaded()){
    btnUp.tick();
    if(btnUp.press()||btnUp.holding()){
      if(editing)editValue(editVal,true);
      else setEditVal(false);
    }
    
    btnDown.tick();
    if(btnDown.press()||btnDown.holding()){
      if(editing)editValue(editVal,false);
      else setEditVal(true);
    }

    btnOk.tick();
    if(btnOk.press()){
      editing=!editing;
      if(!editing)Prefs::save(prefs);
    }

    if(editing)lcdController.write(2,"");
    else{
      String prefix=filamentStatus.isLoaded(FilamentStatus::Side::LEFT)?"   ":"<- ";
      String suffix=filamentStatus.isLoaded(FilamentStatus::Side::RIGHT)?"   ":" ->";
      lcdController.write(2,prefix+"Insert Filament"+suffix);
    }
    lcdController.write(4,getIndicator(0)+"Weld temp: "+String(prefs.getPreheatTemp(),0)+"C",1,false);
    lcdController.write(5,getIndicator(1)+"Weld dist: "+String(prefs.getWeldDist(),1)+"mm",1,false);
    lcdController.write(6,getIndicator(2)+"Weld time: "+String(prefs.getWeldTime())+"sec",1,false);
  }
  editing=false;
}

void loadFilament(){
  if(!filamentStatus.isLoaded())return;
  heaterController.disable();
  lcdController.clear();
  heaterController.setFanEnabled(false);
  for (byte i = 3; i > 0; i--){
    if(!filamentStatus.isLoaded())return;
    lcdController.write(3,"Loading in "+String(i)+" sec.");
    delay(1000);
  }
  
  lcdController.write(3,"Loading...");
  stepperController.load(MAX_SPEED/10,LOAD_DIST);
  stepperController.stop();
}

void weld(){
  if(!filamentStatus.isLoaded())return;
  lcdController.clear();
  heaterController.enable();
  while (!heaterController.heated(prefs.getPreheatTemp())){
    if(!filamentStatus.isLoaded())return;
    lcdController.write(3,"Heating...");
    printCurrentTemp(prefs.getPreheatTemp());
  }
  auto tmr=millis();
  bool loaded=false;
  const unsigned int meltTime=prefs.getWeldTime()*1000;
  while(millis()-tmr<meltTime){
    if(!filamentStatus.isLoaded())return;
    auto time=meltTime-(millis()-tmr);
    lcdController.write(3,"Melting "+String(time/1000)+" sec.");
    printCurrentTemp(prefs.getPreheatTemp());
    if(time<meltTime/2&&!loaded){
      stepperController.load(WELD_SPEED,prefs.getWeldDist()*STEPS_PER_MM);
      stepperController.stop();
      loaded=true;
    }
  }
  stepperController.stop();
}

void cooldown(){
  if(!filamentStatus.isLoaded())return;
  bool rev=false;
  bool pull=true;
  lcdController.clear();
  heaterController.disable();
  if(!filamentStatus.isLoaded())return;
  heaterController.setFanEnabled(true);
  lcdController.write(3,"Cooling...");
  while (heaterController.readTemp()>COOLDOWN_TEMP){
    if(!filamentStatus.isLoaded())return;
    printCurrentTemp(COOLDOWN_TEMP);
    auto temp=heaterController.readTemp();
    if(temp<prefs.getPreheatTemp()-50&&temp>100){
      auto dist=rev?SWING_DIST:-SWING_DIST;
      if(pull){
        stepperController.unload(SWING_SPEED,SWING_DIST/2);
        pull=false;
      }
      stepperController.unload(SWING_SPEED,dist);
      rev=!rev;
    }else stepperController.stop();
  }
  heaterController.setFanEnabled(false);
}

ISR(TIMER1_A) {
  heaterController.tick(prefs.getPreheatTemp());
}

void setup() {
  lcdController.init();
  Timer1.enableISR();
  unloadFilament();
}

bool direction=1;

void loop() {
  selectTemperature();
  loadFilament();
  weld();
  cooldown();
  unloadFilament();
}

