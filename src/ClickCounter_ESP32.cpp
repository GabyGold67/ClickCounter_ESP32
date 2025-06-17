/**
 ******************************************************************************
 * @file ClickCounter_Esp32.cpp
 * 
 * @brief Code file for the ClickCounter_Esp32 library 
 * 
 * @details This libray's ClickCounter class models counters and tally counters in all it's functionality, and adds an extensive set of services and options to manage more complex and demanding counting applications. The displaying services are optionally provided through the use of a SevenSegDisplays library instantiated object.
 * 
 * Repository: https://github.com/GabyGold67/ClickCounter_Esp32  
 * 
 * Framework: Arduino  
 * Platform: ESP32  
 * 
 * @author Gabriel D. Goldman  
 * mail <gdgoldman67@hotmail.com>  
 * Github <https://github.com/GabyGold67>  
 * 
 * @version 1.0.0
 * 
 * @date First release: 20/12/2023  
 *       Last update:   15/06/2025 18:20 (GMT+0200) DST  
 * 
 * @copyright Copyright (c) 2025  GPL-3.0 license
 *******************************************************************************
 */
#include <ClickCounter_Esp32.h>

ClickCounter::ClickCounter()
{    
}

ClickCounter::ClickCounter(SevenSegDisplays* cntrDsplyPtr, bool rgthAlgn, bool zeroPad)
:_cntrDsplyPtr{cntrDsplyPtr}, _countRgthAlgn{rgthAlgn}, _countZeroPad{zeroPad}
{
   _CCCountMutex = xSemaphoreCreateMutex();
}

ClickCounter::ClickCounter(int32_t countMin, int32_t countMax)
:_countMin{countMin}, _countMax{countMax}
{   
   _CCCountMutex = xSemaphoreCreateMutex();
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

   if(locQty > 0){
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
         if((_count - locQty) >= _countMin){
            _count -= locQty;
            if(!_noDisplay)
               result = _updDisplay();
            else
               result = true;
         }
         xSemaphoreGive(_CCCountMutex);
      }
   }

   return result;
}

bool ClickCounter::countIsZero(){
   bool result{false};

   if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
      result = (_count == 0);
      xSemaphoreGive(_CCCountMutex);
   }

   return result;
}

bool ClickCounter::countReset(){

   return countRestart(_beginStartVal);
}
    
bool ClickCounter::countRestart(const int32_t &restartValue){
   bool result{false};

   if ((restartValue >= _countMin) && (restartValue <= _countMax)){
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
         _count = restartValue;
         if(!_noDisplay)
            result = _updDisplay();
         else
            result = true;
         xSemaphoreGive(_CCCountMutex);
      }
   }

   return result;
}

bool ClickCounter::countToZero(const int32_t &qty){
   int32_t locQty = abs(qty);
   bool result {false};

   if(locQty > 0){
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
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
               result = _updDisplay();
         xSemaphoreGive(_CCCountMutex);
      }
   }

   return result;
}

bool ClickCounter::countUp(const int32_t &qty){
   bool result {false};
   int32_t locQty = abs(qty);

   if(locQty > 0){
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
         if((_count + locQty) <= _countMax){
            _count += locQty;
            result = true;
            if(!_noDisplay)
               result = _updDisplay();
         }
         xSemaphoreGive(_CCCountMutex);
      }
   }

   return result;
}

bool ClickCounter::end(){
   bool result{false};

   if(_begun){
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
         if(_cntrDsplyPtr != nullptr)
            clear();
         _countMin = 0;
         _countMax = 0;
         _count = 0;
         _begun = false;
         result = true;
         xSemaphoreGive(_CCCountMutex);
      }
   }

   return result;
}

int32_t ClickCounter::getCount(){
   int32_t result{0};

   if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
      result = _count;
      xSemaphoreGive(_CCCountMutex);
   }

   return result;
}

int32_t ClickCounter::getMaxBlinkRate(){
   int32_t result{0};

   if(!_noDisplay)
      result = _cntrDsplyPtr->getMaxBlinkRate();
    
   return result;
}

int32_t ClickCounter::getMaxCountVal(){

   return _countMax;
}

int32_t ClickCounter::getMinBlinkRate(){
   int32_t result{0};

   if(!_noDisplay)
      result = _cntrDsplyPtr->getMinBlinkRate();
    
   return result;
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

bool ClickCounter::_updDisplay(){
   bool result{false};

   if(!_noDisplay)
      result = _cntrDsplyPtr->print(_count, _countRgthAlgn, _countZeroPad);
   else
      result = true;
    
   return result;
}

bool ClickCounter::updDisplay(){
   bool result{false};
      if(xSemaphoreTake(_CCCountMutex, portMAX_DELAY) == pdTRUE){
         result = _updDisplay();
         xSemaphoreGive(_CCCountMutex);
      }

   return result;
}
