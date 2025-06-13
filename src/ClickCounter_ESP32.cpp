#include <ClickCounter_Esp32.h>

ClickCounter::ClickCounter()
{    
}

ClickCounter::ClickCounter(SevenSegDisplays* cntrDsplyPtr, bool rgthAlgn, bool zeroPad)
:_cntrDsplyPtr{cntrDsplyPtr}, _countRgthAlgn{rgthAlgn}, _countZeroPad{zeroPad}
{
}

ClickCounter::~ClickCounter()
{
}

bool ClickCounter::begin(const int32_t &startVal){
   bool result{false};

   if(!_begun){
      if(_cntrDsplyPtr != nullptr){
         _countMin = _cntrDsplyPtr->getDspValMin();
         _countMax = _cntrDsplyPtr->getDspValMax();
         if ((startVal >= _countMin) && (startVal <= _countMax)){
            result = countRestart(startVal);
            if (result){
               _beginStartVal = startVal;
               _begun = true;
            }
         }
      }
   }

   return result;
}

bool ClickCounter::blink(){
    
   return _cntrDsplyPtr->blink();
}

bool ClickCounter::blink(const unsigned long &onRate, const unsigned long &offRate){

   return _cntrDsplyPtr->blink(onRate, offRate);
}

void ClickCounter::clear(){
   _cntrDsplyPtr->clear();

   return;
}

bool ClickCounter::countDown(const int32_t &qty){
   int32_t locQty = abs(qty);
   bool result {false};

   if((_count - locQty) >= _countMin){
      _count -= locQty;
      result = updDisplay();
   }

   return result;
}

bool ClickCounter::countReset(){

   return countRestart(_beginStartVal);
}
    
bool ClickCounter::countRestart(int32_t restartValue){
   bool result{false};

   if ((restartValue >= _countMin) && (restartValue <= _countMax)){
      _count = restartValue;
      result = updDisplay();
   }

   return result;
}

bool ClickCounter::countToZero(const int32_t &qty){
   int32_t locQty = abs(qty);
   bool result {false};

   if (_count > 0){
      if((_count - locQty) >= 0){
         _count -= locQty;
         result = true;
      }
   }   
   else if (_count < 0){
      if((_count + locQty) <= 0){
         _count += locQty;
         result = true;
      }
   }
   if(result)
      result = updDisplay();

   return result;
}

bool ClickCounter::countUp(const int32_t &qty){
    bool result {false};
    int32_t locQty = abs(qty);

    if((_count + locQty) <= _countMax){
        _count += locQty;
        result = updDisplay();
    }

    return result;
}

bool ClickCounter::end(){
   bool result{false};
   
   if(_begun){
      _begun = false;
      result = true;
   }

   return result;
}

int32_t ClickCounter::getCount(){

    return _count;
}

int32_t ClickCounter::getMaxCountVal(){

   return _countMax;
}

int32_t ClickCounter::getMinCountVal(){

   return _countMin;
}

int ClickCounter::getStartVal(){

    return _beginStartVal;
}

bool ClickCounter::noBlink(){

    return _cntrDsplyPtr->noBlink();
}

bool ClickCounter::setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate){

    return _cntrDsplyPtr->setBlinkRate(newOnRate, newOffRate);
}

bool ClickCounter::updDisplay(){

    return _cntrDsplyPtr->print(_count, _countRgthAlgn, _countZeroPad);
}
