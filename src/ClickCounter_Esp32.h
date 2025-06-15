/**
 ******************************************************************************
 * @file ClickCounter_Esp32.h
 * 
 * @brief Header file for the ClickCounter_Esp32 library 
 * 
 * @details This is a flexible mechanical, electromechanical and electronic counter replacement library. Traditional counters and tally counters are devices composed by a 'counter display' (rotating wheel counter, electronic display, etc.), an 'increment count' signal provider (in the form of mechanical pushbutton, an actionable arm or electronic input signal pin), and a reset signal provider (in the form of mechanical pushbutton, rotating dial or electronic input signal pin). This libray's ClickCounter class models counters and tally counters in all it's functionality, and adds an extensive set of services and options to manage more complex and demanding counting applications. The displaying services are optionally provided through the use of a SevenSegDisplays library instantiated object.
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
  * @attention	This library was originally developed as part of the refactoring
  * process for an industrial machines security enforcement and productivity control
  * (hardware & firmware update). As such every class included complies **AT LEAST**
  * with the provision of the attributes and methods to make the hardware & firmware
  * replacement transparent to the controlled machines. Generic use attributes and
  * methods were added to extend the usability to other projects and application
  * environments, but no fitness nor completeness of those are given but for the
  * intended refactoring project, and for the author's projects requirements.  
  * 
  * @warning **Use of this library is under your own responsibility**
  * 
  * @warning The use of this library falls in the category described by The Alan 
  * Parsons Project (c) 1980 Games People play disclaimer:   
  * Games people play, you take it or you leave it  
  * Things that they say aren't alright  
  * If I promised you the moon and the stars, would you believe it?  
 *******************************************************************************
 */
#ifndef _CLICKCOUNTER_H_
#define _CLICKCOUNTER_H_

#include<Arduino.h>
#include <stdint.h>
#include <sevenSegDisplays.h>

// Definition workaround to let a function/method return value to be a function pointer to a function that receives no arguments and returns no values: void (funcName*)()
typedef void (*fncPtrType)();
typedef fncPtrType (*ptrToTrnFnc)();

// Definition workaround to let a function/method return value to be a function pointer to a function that receives a void* arguments and returns no values: void (funcName*)(void*)
typedef void (*fncVdPtrPrmPtrType)(void*);
typedef fncVdPtrPrmPtrType (*ptrToTrnFncVdPtr)(void*);

class ClickCounter{
private:
   SevenSegDisplays* _cntrDsplyPtr{nullptr};
   bool _countRgthAlgn{true};
   bool _countZeroPad{false};

   int32_t _countMin{0};
   int32_t _countMax{0};

   int32_t _beginStartVal{0};
   bool _begun{false};
   int32_t _count{0};
   bool _noDisplay{true};

   fncVdPtrPrmPtrType _fnWhnCntValZero{nullptr};
	void* _fnWhnCntValZeroArg {nullptr};

 protected:
   bool _updDisplay();

public:
   /**
    * @brief Default constructor
    * 
    */
   ClickCounter();
   /**
    * @brief Class constructor
    * 
    * Instantiates a ClickCounter object with an associated display that will autonomously exhibit the updated count value. For details about SevenSegDisplays objects see the SevenSegDisplays_ESP32 library documentation. 
    * Having a display associated with the counter implies some of the counter object characteristics will be set according to the display characteristics:
    * - The counter minimum and maximum values will be set to the minimum and maximum displayable numbers (related to the quantity of the display's digits/port). 
    * - Every counter modification will automatically update the display, overwriting any other value sent to the display by other sources. 
    * - The boolean value returned by the counter modification methods will be false if the counter modification failed (overflow, underflow) OR if the counter printing to the display fails. 
    * - Methods related to display behavior management (blink(), clear(), noblink(), setBlinkRate(), _updDisplay) will only be enabled and return true boolean values for objects instantiated with associated displays and after successful execution. In any other cases will return **false**.
    * 
    * @param cntrDsplyPntr Pointer to an instantiated SevenSegDisplays class object. That object models the display used to exhibit the counter state. The SevenSegDisplays subclasses model seven segment displays objects.  
    * @param rgthAlgn (Optional) Indicates if the represented value must be displayed right aligned (true), or left aligned (false). When set true, the missing heading characters will be completed with spaces or zeros, depending in the zeroPad optional parameter. If the parameter is not specified the default value, true, will be assumed.  
    * @param zeroPad (Optional) Indicates if the heading free spaces of the integer right aligned displayed must be filled with zeros (true) or spaces (false). If not specified the default value, false, will be assumed.  
    */
   ClickCounter(SevenSegDisplays* cntrDsplyPntr, bool rgthAlgn = true, bool zeroPad = false);
   /**
    * @brief Class constructor
    * 
    * Instantiates a ClickCounter object **without an associated display**. Not having a SevenSegDisplays object associated has, as main consequence for the instantiation, the lack of parameters to calculate the counter valid values range, so the counter minimum and maximum values must be provided at instantiation time. The begin() method will ensure the minimum requirements are met by the provided parameters, most important: the minimum < maximum condition.  
    * 
    * @param countMin Left side limit (minimum) for the counter values valid range segment. The valid count range minimum value is included as a valid counting value. 
    * @param countMax Right side limit (maximum) for the counter values valid range segment. The valid count range maximum value is included as a valid counting value. 
    */
   ClickCounter(int32_t countMin, int32_t countMax);
   /**
    * @brief Class destructor.  
    */
   ~ClickCounter();
   /**
    * @brief Sets the basic required parameters for the object to start working. 
    * 
    * @param startVal Initial value for the counter. The parameter must be within the valid range, wich is calculated from the display digits quantity or provided as constructor parameters (depending on the constructor used to instantiate the ClickCounter object). 
    * 
    * @return The success in setting the needed attribute values for the object to start working.  
    * @retval true The object was not previously begun, the display pointer (if provided) is not a nullptr and the startVal parameter is in the valid range. The object is started.
    * @retval false One of the previously described conditions failed, the object is not ready to be used. 
    */
   bool begin(const int32_t &startVal);
    /**
    * @brief Makes the display blink the contents it is showing if the object was instantiated with an associated display. 
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
    * @note The method will not produce any change in the count state, that will be preserved. To revert the clear() method effects an updateDisplay() must be executed, or wait for the next change of the count value that will refresh the display contents automatically.  
    * 
    * Use example:  
    * @code {.cpp}
    * myClickCounter.clear();
    * @endcode
    */
   void clear();
   /**
    * @brief Decrements the value of the current count. 
    * 
    * The counter value is successfuly decremented by the absolute value of the passed parameter. After the counter is decremented the display (if there's one associated) is refreshed to keep it updated. The counter is decremented independently of the sign of the current count, as long as the new value resulting is in the displayable range.
    * 
    * @param qty Optional integer value, its **absolute** value will be decremented from the current count value. If no parameter is passed a value of one will be used. If qty = 0 the method will return false.  
    * 
    * @return The success in decrementing the counter by the parameter's absolute value. 
    * @retval true The count could be decremented by the corresponding value without setting count out of range. The counter value is decremented and the resulting count is displayed.  
    * @retval false The qty parameter was equal to 0, or the count couldn't be decremented by the parameter value without getting out of range. The counter will keep its current value. 
    * 
    * @warning The returned value must be kept controled to ensure no underflow error occurs and the counter is no longer holding a valid count. 
    */
   bool countDown(const int32_t &qty = 1);
   /**
    * @brief Returns a boolean indicating if the current count value is equal to 0 (zero) or not. 
    * 
    * @retval true The counter current value is 0 (zero). 
    * @retval false The counter current value is NOT 0 (zero). 
    */
   bool countIsZero();   
   /**
    * @brief Resets the counter. 
    * 
    * Restarts the count to the original value provided when the counter was started with the begin(const int32_t &startVal) method, i.e. to **startVal**. The display is updated to reflect this change in its new value.  
    * 
    * @return true
    * 
    * @note The startVal value might not necesarily be 0. If the counter needs to be restarted from a determined value see bool countRestart(int32_t) 
    */
   bool countReset();
   /**
    * @brief Restarts the count from the value provided as parameter. The display is updated to reflect this change in its new value.  
    * 
    * @param restartValue Optional integer value, a value of 0 is set if no parameter is provided. The parameter must be in the range getMinCountVal() <= restartValue <= getMaxCountVal().  
    * @return true The parameter value was within valid range, count restart succeeded.
    * @return false The parameter value was NOT within valid range, count restart failed.
    */
   bool countRestart(const int32_t &restartValue = 0);
   /**
    * @brief Increments or decrements the counter value to approach a final 0 value. 
    * 
    * The counter **absolute** value will be decremented, independently of the sign of the current count, and the sign of the qty parameter: If the current count value was negative, the value will be incremented, if it was positive, will be decremented, with the concrete purpouse of approaching the count value to 0 (zero).
    * The function will decrement the absolute value of the count as long as abs(count) - abs(qyt) >= 0. 
    * 
    * @param qty Optional integer value, its **absolute value** will be decremente from the current **absolute count value**, the sign of the resulting count will be preserved. If no parameter is passed a default value of 1 (one) will be used.
    * @retval true The absolute value of the count minus the absolute value of the parameter resulted in a value greater or equal to 0, i.e. abs(count) - abs(qyt) >= 0. The count is modified. 
    * @return false The qty parameter passed value is 0, or the absolute value of the count minus the absolute value of the parameter resulted in a value smaller than 0, i.e. a negative value. The count will NOT be modified.
    * 
    * @note If the count has reached a zero value no further countToZero(const int32_t &) methods would modify the counter, returning always false.
    */
   bool countToZero(const int32_t &qty = 1);
   /**
    * @brief Increments the value of the current count. 
    * 
    * The counter value is incremented by the absolute value of the passed parameter. After the counter is successfuly incremented the display (if there's one associated) is refreshed to keep it updated. The counter is incremented independently of the sign of the current count, as long as the new value resulting is in the displayable range.
    * 
    * @param qty Optional integer value, its **absolute** value will be incremented in the current count value. If no parameter is passed a value of one will be used. If qty = 0 the method will return false.  
    * 
    * @return The success in incrementing the counter by the parameter's absolute value. 
    * @retval true The count could be incremented by the corresponding value without setting count out of range. The counter value is incremented and the resulting count is displayed (if a display is set).  
    * @retval false The qty parameter was equal to 0, or the count couldn't be incremented by the parameter value without getting out of range. The counter will keep its current value. 
    * 
    * @warning The returned value must be kept controled to ensure no overflow error occurs and the counter is no longer holding a valid count. 
    */
   bool countUp(const int32_t &qty = 1);
   /**
    * @brief Disables the counter for further activities.  
    * 
    * To RESTART the counter for further use a new bool begin(const int32_t &) method must be invoked
    * 
    * @return The success in ending the counter active status.  
    * @retval true The counter was active, and it was disabled for further actions. 
    * @retval false The counter was NOT active, no change of it's status was done. 
    */
   bool end();
   /**
    * @brief Get the object's count value. 
    * 
    * @return int32_t The current count value of the clickCounter object. 
    */
   int32_t getCount();
   /**
    * @brief Returns the maximum rate the display can be configured to blink at. 
    * 
    * The maximum rate the display can be configured to blink at helps keeping the blinkRate setters inside the accepted range. At least two aspects of the blinking process are involved in the determination if this value.  
    * - The technical aspect is related to the hardware possibility to manage the display contents change to show the blinking effect. 
    * - The perception aspect is related to keep the blinking effect range in a rhythm at which the display turns on and off as part of the information that it provides, and doesn't make it look like a display failure or any other kind of bug.  
    * 
    * @return The maximum time, in milliseconds, the display can be set to blink. This value is the maximum to set as the turned-on or the turned-off stage of the blinking process started by a **`blink()`** or a **`blink(const unsigned long, const unsigned long &)`** method.  
    * 
    * @attention Opposite to the concept of Hertz, that designates how many times an action happens in a fixed period of time (a second), the value used in the `blink()` and all related methods is **the time set to elapse before the next action happens**.  
    */
   int32_t getMaxBlinkRate();
   /**
    * @brief Returns the Maximum Counter Value.
    * 
    * The maximum valid counter value is the right side limit for the counter values valid range segment. The valid count range maximum value is **included** as a valid counting value. 
    * 
    * @return The Maximum Counter Value
    */
   int32_t getMaxCountVal();
   /**
    * @brief Returns the minimum rate the display can be configured to blink at. 
    * 
    * The minimum rate the display can be configured to blink at helps keeping the blinkRate setters inside the accepted range. At least two aspects of the blinking process are involved in the determination if this value.  
    * - The technical aspect is related to the hardware possibility to manage the display contents change to show the blinking effect. 
    * - The perception aspect is related to keep the blinking effect range in a rhythm at which the display turns on and off as part of the information that it provides, and doesn't make it look like a display failure or any other kind of bug.  
    * 
    * @return The minimum time, in milliseconds, the display can be set to blink. This value is the minimum to set as the turned-on or the turned-off stage of the blinking process started by a **`.blink()`** or a **`blink(const unsigned long &, const unsigned long &)`** method.  
    * 
    * @attention Opposite to the concept of Hertz, that designates how many times an action happens in a fixed period of time (a second), the value used in the `blink()` and all related methods is **the time set to elapse before the next action happens**.  
    */
   int32_t getMinBlinkRate();
   /**
    * @brief Returns the Minimum Counter Value.
    * 
    * The minimum valid counter value is the left side limit for the counter values valid range segment. The valid count range minimum value is **included** as a valid counting value. 
    * 
    * @return The Minimum Counter Value
    */
   int32_t getMinCountVal();
   /**
    * @brief Returns the value that was used to begin() the ClickCounter object. 
    * 
    * The original value provided when the counter was started with the begin(const int32_t &startVal) method, i.e. to **startVal** is the value to wich the counter will return when a bool countReset() method is invoked. 
    * 
    * @return startVal attribute value.  
    * 
    * @note The startVal value might not necesarily be 0. If the counter needs to be restarted from a determined value see bool countRestart(int32_t) 
    * 
    * @note No specific mechanism is provided to change the startVal attibute, as changing just that attribute might colude with the data integrity mechanisms. The only way provided to do such change is trough the execution of an `end()` method followed by a `begin(newStartValue)`, where the newStartValue is the intented value for startVal to be set to. 
    */
   int32_t getStartVal();
   /**
    * @brief Stops the display blinking, if it was doing so, leaving the display turned on.
    * 
    * @retval true The display was set to not blinking, either because the display was set to blink and was stopped, either the display was not set to blink.  
    * @retval false The display was set to blink, and the blink stopping failed.  
    * 
    * Code example:  
    * 
    * @code {.cpp}
    * myClickCounter.noBlink();
    * @endcode
    */
   bool noBlink();
   /**
    * @brief Changes the time parameters to use for the display blinking of the contents it shows.  
    * 
    * The parameters change will take immediate effect, either if the display is already blinking or not, in the latter case the parameters will be the ones used when a **`blink()`** method is called without parameters. The blinking will be **symmetrical** if only one parameter is passed, **asymmetrical** if two different parameters are passed, meaning that the time the display shows the contents and the time the display is blank will be equal (symmetrical) or not equal (asymmetrical), depending of those two parameters. The blink rate set will be kept after a **`noBlink()`** or new **`blink()`** without parameters call is done, until it is modified with a new **`setBlinkRate()`** call, or it is restarted by a **`blink()`** with parameters. Note that to restart the blinking with a **`blink()`** the service must first be stopped, as the method makes no changes if the blinking service was already running.  
    * 
    * @param newOnRate unsigned long integer containing the time (in milliseconds) the display must stay on, the value must be in the range _minBlinkRate <= onRate <= _maxBlinkRate. Those built-in values can be known by the use of the **`getMinBlinkRate()`** and the **`getMaxBlinkRate()`** methods.  
    * @param newOffRate optional unsigned long integer containing the time (in milliseconds) the display must stay off, the value must be in the range _minBlinkRate <= offRate <= _maxBlinkRate. Those built-in values can be known by the use of the **`getMinBlinkRate()`** and the **`getMaxBlinkRate()`** methods. If no offRate value is provided the method will assume it's a symmetric blink call and use a value of offRate equal to the value passed by onRate.  
    * 
    * @return true The parameter or parameters passed are within the valid range, The change will take effect immediately.  
    * @return false One or more of the parameters passed were out of range. The rate change would not be made for none of the parameters.  
    * 
    * Code example
    * 
    * @code {.cpp}
    * myClickCounter.setBlinkRate(400);  //Returns true and sets the blinking rate to 400 millisecs on, 400 millisecs off (symmetrical blink).  
    * myClickCounter.setBlinkRate(800, 200);   //Returns true and sets the blinking rate to 800 millisecs on, 200 millisecs off (asymmetrical blink)  
    * myClickCounter.setBlinkRate(3000); //Returns false and the display blinking rate stays without change.  
    * myClickCounter.setBlinkRate(600, 3500);  //Returns false and the display blinking rate stays without change.  
    * @endcode
    */
   bool setBlinkRate(const unsigned long &newOnRate, const unsigned long &newOffRate = 0);
   /**
    * @brief Updates the SevenSegDisplays object associated to the ClickCounter with the current object's count value.
    * 
    * @return The success in displaying the ClickCounter count value.
    * @retval true The SevenSegDisplays object associated to the ClickCounter updated it's display to reflect the current count value, or there's no display associated to the ClickCounter (so the displaying action couldn't have failed). 
    * @retval false The SevenSegDisplays object associated to the ClickCounter failed updating it's display. 
    */
   bool updDisplay();
};
   
#endif

