#ifndef _CLICKCOUNTER_H_
#define _CLICKCOUNTER_H_

#include<Arduino.h>
#include <stdint.h>
#include <sevenSegDisplays.h>

class ClickCounter{
private:
   SevenSegDisplays* _cntrDsplyPtr{nullptr};
   bool _countRgthAlgn{true};
   bool _countZeroPad{false};

   bool _begun{false};
   int32_t _countMax{0};
   int32_t _countMin{0};
   int32_t _count{0};
   int32_t _beginStartVal{0};

public:
   /**
    * @brief Default constructor
    * 
    */
   ClickCounter();
   /**
    * @brief Class constructor
    * 
    * Instantiates a ClickCounter object. For details about SevenSegDisplays objects see the SevenSegDisplays_ESP32 library documentation.   
    * 
    * @param cntrDsplyPntr Pointer to an instantiated SevenSegDisplays class object. That object models the display used to exhibit the counter state. The SevenSegDisplays subclasses model seven segment displays objects.  
    * @param rgthAlgn (Optional) Indicates if the represented value must be displayed right aligned (true), or left aligned (false). When set true, the missing heading characters will be completed with spaces or zeros, depending in the zeroPad optional parameter. If the parameter is not specified the default value, true, will be assumed.  
    * @param zeroPad (Optional) Indicates if the heading free spaces of the integer right aligned displayed must be filled with zeros (true) or spaces (false). If not specified the default value, false, will be assumed.  
    */
   ClickCounter(SevenSegDisplays* cntrDsplyPntr, bool rgthAlgn = true, bool zeroPad = false);
   /**
    * @brief Class destructor.  
    */
   ~ClickCounter();
   /**
    * @brief Sets the basic required parameters for the object to start working. 
    * 
    * @param startVal Initial value for the counter. The parameter must be within the valid range, wich is calculated from the display digits quantity. 
    * 
    * @return The success in setting the needed values for the object to start working
    * @retval true The object was not previously begun, the display pointer is valid, the startVal parameter is in the valid range. The object is started.
    * @retval false One of the previously described conditions failed, the object is not ready to be used. 
    */
   bool begin(const int32_t &startVal);
    /**
    * @brief Makes the display blink the contents it is showing.
    * 
    * The display will blink the contents it is showing until a **`noBlink()`** method is invoked. The display will continue blinking even if the contents are changed.  
    * When invoking the **`blink()`** method with no parameters the blinking pace (timings) previously set will be used. If no **`setBlinkRate(const unsigned long, const unsigned long)`** and no **`blink(const unsigned long, const unsigned long)`** with parameters was used before this call, the blinking will be symmetrical, meaning that the time the display shows the contents and the time the display is blank are equal. The on time and the off time of the blinking starts at a preset rate this first time the method is invoked.    
    * The blinking rate can be changed by using the **`setBlinkRate(const unsigned long, const unsigned long)`** method. After changing the blinking rate, the new blinking rate will be kept after a **`noBlink()`** call is done, until it is modified with a new **`setBlinkRate(const unsigned long, const unsigned long)`** call, or it is restarted by a **`blink(const unsigned long, const unsigned long)`** with parameters.  
    * @note To restart the blinking with a **`blink()`** or a **`blink(const unsigned long, const unsigned long)`** the blinking service must first be stopped, as the method call makes no changes if the blinking service was already running.  
    * 
    * @retval true: If the display blinking process started ok, or was already set to blink. 
    * @retval false: The display blinking failed to start.  
    * 
    * Use example:  
    * @code {.cpp}
    * myCounter.blink(); // Begin blinking at the already set rate
    * @endcode
    */
   bool blink();
   /**
    * @brief Makes the display blink the contents it is showing.
    * 
    * Makes the display blink the contents it shows until a **`noBlink()`** method is invoked. The blinking is **symmetrical** if only one parameter is passed, **asymmetrical** if two **different** parameters are passed, meaning that the time the display shows the contents and the time the display is blank will be equal (symmetrical) or not (asymmetrical), depending of those two parameters. The blinking starts at a passed rate when the method is invoked. The blinking rate can be changed by using the **`.setBlinkRate(const unsigned long, const unsigned long)`** method. The blink rate set will be kept after a **`.noBlink()`**, until it is modified with a new **`.setBlinkRate(const unsigned long, const unsigned long)`** call, or it is restarted by a **`.blink(const unsigned long, const unsigned long)`** with parameters.  
    *   
    * @note To restart the blinking with a **`blink()`** or a **`blink(const unsigned long, const unsigned long)`** the service must first be stopped, as the method call makes no changes if the blinking service was already running.  

   * @param onRate Value indicating the time (in milliseconds) the display must stay on, the value must be in the range _minBlinkRate <= onRate <= _maxBlinkRate. Those preset values can be known by the use of the **`getMinBlinkRate()`** and the **`getMaxBlinkRate()`** methods.
   * @param offRate Optional value indicating the time (in milliseconds) the display must stay off, the value must be in the range _minBlinkRate <= offRate <= _maxBlinkRate. Those preset values might be known by the use of the **`getMinBlinkRate()`** and the **`getMaxBlinkRate()`** methods. If no offRate value is provided the method will assume it's a symmetric blink call and use a value for offRate equal to the value passed for onRate. 
   * 
   * @retval true If the display was already set to blink, or was not blinking and the blinking started with the provided parameters. If the display was already blinking no change will be made to the blinking rate.  
   * @retval false One or more of the parameters passed were out of range.  
   * 
   * Use examples:  
   * @code {.cpp}
   * myCounter.blink(400); // Starts blinking setting the rate to 400 millisecs on, 400 millisecs off (symmetrical blink). And returns true.  
   * @endcode
   * 
   * @code {.cp}
   * myCounter.blink(800, 200); // Sets the blinking rate to 800 millisecs on, 200 millisecs off (asymmetrical blink), starts blinking, and returns true.  
   * @endcode
   * 
   * @code {.cpp}
   * unsigned long rateTooBig {myLedDisp.getMaxBlinkRate() + 10} // Saves in a variable a blinking rate out of accepted range
   * myCounter.blink(rateTooBig); //Returns false and the display stays without change.  
   * @endcode
   */
   bool blink(const unsigned long &onRate, const unsigned long &offRate = 0);
   /**
    * @brief Clears the display, turning off all the segments and dots.
    * 
    * @note The method will not produce any change in the count state, that will be preserved.  
    * 
    * Use example:  
    * @code {.cpp}
    * myCounter.clear();
    * @endcode
    */
   void clear();
   /**
    * @brief Decrements the value of the current count. 
    * 
    * After the counter is decremented the display is refreshed to keep it updated. The counter is decremented independently of the sign of the current count, as long as the new value resulting is in the displayable range.
    * 
    * @param qty Optional integer value, its **absolute** value will be decremented from the current count value. If no parameter is passed a value of one will be used. 
    * 
    * @return The success in decrementing the counter by the parameter absolute value. 
    * @retval true The count could be decremented by the corresponding value without setting count out of range. The counter value is decremented and the resulting count is displayed.  
    * @retval false The count couldn't be decremented by the parameter value without getting out of range. The counter will keep its current value. 
    * 
    * @warning The returned value must be kept controled to ensure no underflow error occurs and the counter is no longer holding a valid count. 
    */
   bool countDown(const int32_t &qty = 1);
   /**
    * @brief Resets the counter. 
    * 
    * Restarts the count to the original value provided when the counter was started with the begin(const int32_t &startVal) method, i.e. to startVal. The display is updated to reflect this change in its new value.  
    * 
    * @return true
    * 
    * @note The startVal value might not necesarily be 0. If the counter needs to be restarted from a determined value use bool countRestart(int32_t) 
    */
   bool countReset();
   /**
    * @brief Restarts the count from the value provided as parameter. The display is updated to reflect this change in its new value.  
    * 
    * @param restartValue Optional integer value, a value of 0 is set if no parameter is provided. The parameter must be in the range getMinCountVal() <= restartValue <= getMaxCountVal().  
    * @return true The parameter value was within valid range, count restart succeeded.
    * @return false The parameter value was NOT within valid range, count restart failed.
    */
   bool countRestart(int32_t restartValue = 0);
//FFDR go on checking from this point
   /**
    * @brief Modifies the counter value to approach a final 0 value. 
    * 
    * The counter **absolute** value will be decremented, independently of the sign of the current count, and the sign of the qty parameter: If the current count value was negative, the value will be incremented, if it was positive, will be decremented, with the concrete purpouse of approaching the count value to 0 (zero).
    * The function will decrement the absolute value of the count as long as abs(count) - abs(qyt) >= 0. 
    * 
    * @param qty Optional integer value, its **absolute** value will be decremente from the current absolute count value, the sign of the resulting count will be preserved. If no parameter is passed a default value of 1 (one) will be used.
    * @retval true The absolute value of the count minus the absolute value of the parameter resulted in a value greater or equal to 0, i.e. abs(count) - abs(qyt) >= 0. The count is modified. 
    * @return false The absolute value of the count minus the absolute value of the parameter resulted in a value smaller than 0, i.e. a negative value. The count will NOT be modified.
    * 
    * @note If the count has reached a zero value 
    */
   bool countToZero(const int32_t &qty = 1);
   bool countUp(const int32_t &qty = 1);
   bool end();
   int32_t getCount();
//FFDR   SevenSegDisplays* getCntrDsplyPtr();
   int32_t getMaxBlinkRate();
   int32_t getMaxCountVal();
   int32_t getMinBlinkRate();
   int32_t getMinCountVal();
   int32_t getStartVal();
   bool noBlink();
   bool setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate = 0);
//FFDR   bool setFnCountValZero();
//FFDR   bool setFnCountValMax();
//FFDR   bool setFnCountValMin();
//FFDR   bool setFnCountValGoal();
   bool updDisplay();  //To be analyzed it's current need
};
   
   #endif
