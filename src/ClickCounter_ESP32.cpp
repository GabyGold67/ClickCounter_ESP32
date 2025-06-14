#include <ClickCounter_Esp32.h>

ClickCounter::ClickCounter()
{    
}

ClickCounter::ClickCounter(SevenSegDisplays* cntrDsplyPtr, bool rgthAlgn, bool zeroPad)
:_cntrDsplyPtr{cntrDsplyPtr}, _countRgthAlgn{rgthAlgn}, _countZeroPad{zeroPad}
{
}

ClickCounter::ClickCounter(int32_t countMin, int32_t countMax)
:_countMin{countMin}, _countMax{countMax}
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
         _noDisplay = false;
      } 
      if(_countMin < _countMax){
         result = countRestart(startVal);
         if (result){
            _beginStartVal = startVal;
            _begun = true;
         }
      } 
   }

   return result;
}

bool ClickCounter::blink(){
   bool result{false};

   if(!_noDisplay)
      result = _cntrDsplyPtr->blink();
   else
      result = true;
    
   return result;
}

bool ClickCounter::blink(const unsigned long &onRate, const unsigned long &offRate){
   bool result = false;

   if(!_noDisplay)
      result = _cntrDsplyPtr->blink(onRate, offRate);
   else
      result = true;

      return result;
}

void ClickCounter::clear(){
   if(!_noDisplay)
      _cntrDsplyPtr->clear();

   return;
}

bool ClickCounter::countDown(const int32_t &qty){
   int32_t locQty = abs(qty);
   bool result {false};

   if((_count - locQty) >= _countMin){
      _count -= locQty;
      if(!_noDisplay)
         result = updDisplay();
      else
         result = true;
   }

   return result;
}

bool ClickCounter::countReset(){

   return countRestart(_beginStartVal);
}
    
bool ClickCounter::countRestart(const int32_t &restartValue){
   bool result{false};

   if ((restartValue >= _countMin) && (restartValue <= _countMax)){
      _count = restartValue;
      if(!_noDisplay)
         result = updDisplay();
      else
         result = true;
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
      if(!_noDisplay)
         result = updDisplay();

   return result;
}

bool ClickCounter::countUp(const int32_t &qty){
   bool result {false};
   int32_t locQty = abs(qty);

   if((_count + locQty) <= _countMax){
      _count += locQty;
      result = true;
      if(!_noDisplay)
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
   bool result{false};

   if(!_noDisplay)
      result = _cntrDsplyPtr->noBlink();
   else
      result = true;
    
   return result;
}

bool ClickCounter::setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate){
   bool result{false};

   if(!_noDisplay)
      result = _cntrDsplyPtr->setBlinkRate(newOnRate, newOffRate);
   else
      result = true;
    
   return result;
}

bool ClickCounter::updDisplay(){
   bool result{false};

   if(!_noDisplay)
      result = _cntrDsplyPtr->print(_count, _countRgthAlgn, _countZeroPad);
   else
      result = true;
    
   return result;
}
