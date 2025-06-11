#ifndef _CLICKCOUNTER_H_
#define _CLICKCOUNTER_H_

#include<Arduino.h>
#include <stdint.h>
#include <sevenSegDisplays.h>
#include <ButtonToSwitch_ESP32.h>

class ClickCounter{
   private:
       SevenSegDisplays* _cntrDsply;
       SnglSrvcVdblMPBttn* _cntrMPBPntr;
       int _count{0};
       int _beginStartVal{0};
       bool _countRgthAlgn{true};
       bool _countZeroPad{false};

   public:
       ClickCounter();
       ClickCounter(SevenSegDisplays* cntrDsplyPntr, SnglSrvcVdblMPBttn* cntrMPBPntr);
       ~ClickCounter();
       bool begin();
       bool blink();
       bool blink(const unsigned long &onRate, const unsigned long &offRate = 0);
       void clear();
       bool clicked();
       bool countBegin(int32_t startVal = 0);  //To be analyzed it's current need
       bool countDown(int32_t qty = 1);
       bool countReset();
       bool countRestart(int32_t restartValue = 0);
       bool countStop();   //To be analyzed it's current need
       bool countToZero(int32_t qty = 1);
       bool countUp(int32_t qty = 1);
       int32_t getCount();
       int32_t getStartVal();
       bool noBlink();
       bool setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate = 0);
       bool updDisplay();  //To be analyzed it's current need
   };
   
   #endif
