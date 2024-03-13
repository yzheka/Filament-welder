#include <GyverOLED.h>

class LcdController{
private:
    GyverOLED<SSD1306_128x64,OLED_BUFFER> lcd;
public:
    void init(){
        lcd.init();
        Wire.setClock(800000L);
        clear();
    }

    void clear(){
        lcd.clear();
        lcd.update();
    }

    void write(int line,const String text,uint8_t scale=1, bool alignCenter=true){
        String prefix="";
        if(alignCenter){
            auto length=text.length();
            unsigned int count;
            switch (scale){
            case 1:
                count=21;
                break;

            case 2:
                count=10;
                break;

            case 3:
                count=8;
                break;

            case 4:
                count=5;
                break;

            default:
                count=-1;
                break;
            }
            if(length<count){
                int iter=(count-length)/2;
                for (int i = 0; i < iter; i++){
                    prefix+=" ";
                }

            }
        }
        lcd.setScale(scale);
        lcd.setCursor(0,line);
        lcd.print(prefix+text);
        lcd.print("                 ");
        lcd.update();
    }
};
