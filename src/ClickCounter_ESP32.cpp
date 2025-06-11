#include <ClickCounter_Esp32.h>

ClickCounter::ClickCounter(){};

ClickCounter::ClickCounter(SevenSegDisplays* cntrDsplyPntr, SnglSrvcVdblMPBttn* cntrMPBPntr){}

ClickCounter::~ClickCounter(){

}

bool ClickCounter::begin(){

    return true;
}


bool ClickCounter::blink(){
    
    return _cntrDsply->blink();
}

bool ClickCounter::blink(const unsigned long &onRate, const unsigned long &offRate){

    return _cntrDsply->blink(onRate, offRate);
}

void ClickCounter::clear(){
    _cntrDsply->clear();

    return;
}

bool ClickCounter::countBegin(int32_t startVal){
   bool result{false};

    if ((startVal >= _cntrDsply->getDspValMin()) && (startVal <= _cntrDsply->getDspValMax())){
        // if (_display.begin() == true){
            result = countRestart(startVal);
            if (result)
                _beginStartVal = startVal;
        // }
    }
   return result;
}

bool ClickCounter::countDown(int32_t qty){
    bool result {false};
    qty = abs(qty);

    if((_count - qty) >= _cntrDsply->getDspValMin()){
        _count -= qty;
        result = updDisplay();
    }

    return result;
}

bool ClickCounter::countReset(){

    return countRestart(_beginStartVal);
}
    
bool ClickCounter::countRestart(int32_t restartValue){
   bool result{false};

   if ((restartValue >= _cntrDsply->getDspValMin()) && (restartValue <= _cntrDsply->getDspValMax())){
      _count = restartValue;
      result = updDisplay();
   }

   return result;
}

bool ClickCounter::countStop(){
    
    // return _display.stop();
    return true;
}

bool ClickCounter::countToZero(int32_t qty){
    bool result {false};

    if (_count > 0)
        result = countDown(qty);
    else if (_count < 0)
        result = countUp(qty);
    
    return result;
}

bool ClickCounter::countUp(int32_t qty){
    bool result {false};
    qty = abs(qty);

    if((_count + qty) <= _cntrDsply->getDspValMax()){
        _count += qty;
        result = updDisplay();
    }

    return result;
}

int ClickCounter::getCount(){

    return _count;
}

int ClickCounter::getStartVal(){

    return _beginStartVal;
}

bool ClickCounter::noBlink(){

    return _cntrDsply->noBlink();
}

bool ClickCounter::setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate){

    return _cntrDsply->setBlinkRate(newOnRate, newOffRate);
}

bool ClickCounter::updDisplay(){

    return _cntrDsply->print(_count, _countRgthAlgn, _countZeroPad);
}
