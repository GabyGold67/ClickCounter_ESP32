# **Included Methods for ClickCounter class**  

|Method | Parameters|
|---|---|
|**_ClickCounter_** |uint8_t **sclk**, uint8_t **rclk**, uint8_t **dio**(, bool **rgthAlgn**(, bool **zeroPad**(, bool **commAnode**(, uint8_t **dspDigits**))))|
|**blink()**|None|
||unsigned long **onRate** (,unsigned long **offRate**)|
|**countBegin()**|(int **startVal**)|
|**countDown()**|(int **qty**)|
|**countReset()**|None|
|**countRestart()**|(int **restartValue**)|
|**countStop()**|None|
|**countToZero()**|(int **qty**)|
|**countUp()**|(int **qty**)|
|**getCount()**|None|
|**getStartVal()**|None|
|**noBlink()**|None|
|**setBlinkRate()**|unsigned long **newOnRate**, (unsigned long **newOffRate**)|
|**updDisplay()**|None|

## **Methods definition and use description**

---
### **ClickCounter**(uint8_t **sclk**, uint8_t **rclk**, uint8_t **dio**, bool **rgthAlgn**, bool **zeroPad**)
### Description:  
Class constructor, creates an instance of the class for each display to use. There's no need to configure the pins before calling the method, as the constructor takes care of the task.  
### Parameters:  
**sclk:** uint8_t (unsigned char), passes the pin number that is connected to the sclk pin of the display (the **SH_CP** pin of the shift register if working in a custom display). The pin must be free to be used as a digital output.  
**rclk:** uint8_t (unsigned char), passes the pin number that is connected to the rclk pin of the display (the **ST_CP** pin of the shift register if working in a custom display). The pin must be free to be used as a digital output.  
**dio:** uint8_t (unsigned char), passes the pin number that is connected to the dio pin of the display (the **DS** pin of the shift register if working in a custom display). The pin must be free to be used as a digital output.  
**rgtAlgn:** Boolean, optional parameter (if not specified the default value, true, will be assumed), indicates if the represented value must be displayed right aligned, with the missing heading characters being completed with spaces or zeros, depending in the zeroPad optional parameter. When a negative value is displayed and it's less than (dspDigits - 1) digits long, a right aligned display will keep the '-' sign in the leftmost position, and the free space to the leftmost digit will be filled with spaces or zeros, depending in the zeroPad optional parameter.  
**zeroPad:** Boolean, optional parameter (if not specified the default value, false, will be assumed), indicates if the heading free spaces of the integer right aligned displayed must be filled with zeros (true) or spaces (false). In the case of a negative integer the spaces or zeros will fill the gap between the '-' sign kept in the leftmost position, and the first digit.  
**commAnode:** bool, indicates if the display uses common anode seven segment LED digits (**true** value, default to keep the implementation backwards compatible), or  common cathode kind (**false** value). The use of one kind or the other makes a difference in the fact that one is complementary of the other, meaning a translation must be done on the information sent to the display. Each display instantiated by the class might be independently set up as one kind or the other.  
**dspDigits:** uint8_t (unsigned char), passes the number of digits in the instantiated display, the default value is 4, to keep the implementation backwards compatible.  
### Return value:  
The object created.  
### Use example:  
**`ClickCounter myClickCounter(6, 7, 10, true, true);`**  
**`ClickCounter myClickCounter(6, 7, 10, true, true, true, 5);`**  // 5 digits common anode counter  

---
### **blink**();
### Description:
Refer to **SevenSeg74HC595::blink()** method.  
### Parameters:  
None
### Return value:  
true: If the display was not already set to blink (so now the blinking was started).  
false: The display was already set to blink.  
### Use example:  
**`myClickCounter.blink();`**

---
### **blink**(unsigned long **onRate**,unsigned long **offRate**);
### Description:
Refer to **SevenSeg74HC595::blink()** method.  
### Parameters:  
Refer to **SevenSeg74HC595::blink()** method.  
### Return value:  
true: If the display was not already set to blink (so now the blinking was started).  
false: The display was already set to blink, and/or one or more of the parameters passed were out of range.  
### Use example:  
**`myClickCounter.blink(400);`** //Returns true and sets the blinking rate to 400 millisecs on, 400 millisecs off (symmetrical blink).  
**`myClickCounter.blink(800, 200);`** //Returns true and sets the blinking rate to 800 millisecs on, 200 millisecs off (asymmetrical blink)  
**`myClickCounter.blink(getMaxBlinkRate() + 10);`** //Returns false and the display stays without change.  

---
### **countBegin**(int **startVal**);
### Description:
Attaches the display to a timer interrupt service, as described in the **SevenSeg74HC595::begin()** method. The display then is started with the **startVal** count value, or **0** if no parameter is provided, represented according to the selected options of alignement and padding.
### Parameters:  
startVal: Optional integer value at wich the counter starts which must be in the range (-1)*(pow(10, (dspDigits - 1)) - 1) <= startValue <= (pow(10, dspDigits) - 1).  
### Return value:  
true: If the display could be attached to the ISR, or if the display was already attached to it, and the startValue was within the valid representable values range according to dspDigits.  
false: If the display couldn't be attached to the ISR, due to lack of free slots, or the startValue was out of range.  
### Use example:  
**`myClickCounter.countBegin();`**  

---
## **countDown()**(int **qty**);  
### Description:
Decrements the value of the current count and refreshes the display to keep it updated. The counter is decremented independently of the sign of the current count, as long as the new value resulting is in the displayable range.
### Parameters:  
qty: Optional integer value, its **absolute** value will be decremented from the current count value. If no parameter is passed a value of one will be used.  
### Return value:  
true: If the count could be decremented by the corresponding value without setting count out of range.  The counter value is decremented.  
false: If the count couldn't be decremented by the parameter value without getting out of range.The counter will keep its current value. 
### Use example:  
**`myClickCounter.countDown();`**  //Decrements the current count by 1  
**`myClickCounter.countDown(2);`**  //Decrements the current count by 2  
**`myClickCounter.countDown(-2);`**  //Decrements the current count by 2  

---
### **countReset()**;  
### Description:
Restarts the count to the original value provided when the counter was started with the **countBegin**(int **startVal**) method (not necesarily 0!!). The display is updated to reflect this change in its new value.  
### Parameters:  
None
### Return value:  
true: After resetting the value and updating the display.  

### Use example:  
**`myClickCounter.countReset();`**  

---

## **countRestart()**(int **restartValue**)  
### Description:
Restarts the count from the value provided as parameter. The display is updated to reflect this change in its new value.  
### Parameters:  
restartValue: Optional integer value, a value of 0 is set if no parameter is provided. The parameter must be in the range (-1)*(pow(10, (dspDigits - 1)) - 1) <= restartValue <= (pow(10, dspDigits) - 1).  
### Return value:  
true: If the parameter value was within valid range. 
false: If the parameter value was outside valid range.  
### Use example:  
**`myClickCounter.countRestart();`**  //sets the counter to 0  
**`myClickCounter.countRestart(-100);`**  //Sets the counter to -100 if the display is more than 3 digits long, false otherwise  
**`myClickCounter.countRestart(10000);`**  //Returns false if the display is less than 5 digits long, the counter is not changed  

---
### **countStop**();
### Description:
Refer to **SevenSeg74HC595::stop()** method.    
### Parameters:  
**None**  
### Return value:  
Refer to **SevenSeg74HC595::stop()** method.   
### Use example:  
**`myClickCounter.countStop();`**  

---
## **countToZero()**(int **qty**);  
### Description:
Modifies the value of the current count and refreshes the display to keep it updated. The counter absolute value will be decremented, independently of the sign of the current count , as long as the new value resulting is in the displayable range. If the current count was negative, the value will be incremented, if it was positive, will be incremented, with the concrete porpouse of approaching the count value to 0.
### Parameters:  
qty: Optional integer value, its **absolute** value will be decremente from the current absolute count value, the sign of the resulting count will be preserved. If no parameter is passed a value of one will be used.  
### Return value:  
true: If the absolut count could be decremented by the corresponding value, i.e. the count was not zero already, and the value pased by parameter absolut value does not exceed the count absolute value.  The counter value will be updated.  
false: If the count was already in a 0 value, or the the value pased by parameter absolut value does not exceed the count absolute value. The counter will keep its current value.  
### Use example:  
**`myClickCounter.countToZero();`**  // If the count was positive decrements the current count by 1, if it was negative increments the count by 1.  
**`myClickCounter.countToZero(2);`**  //If the count was positive and greater than 2 decrements the current count by 2, if it was negative increments the count by 2.  
**`myClickCounter.countToZero(-2);`**  //If the count was positive and greater than 2 decrements the current count by 2, if it was negative increments the count by 2.  

---
## **countUp()**(int **qty**);  
### Description:
Increments the value of the current count and refreshes the display to keep it updated. The counter is incremented independently of the sign of the current count, as long as the new value resulting is in the displayable range.
### Parameters:  
qty: Optional integer value, its **absolute** value will be incremented in the current count value. If no parameter is passed a value of one will be used.  
### Return value:  
true: If the count could be incremented by the corresponding value without setting count out of range.  The counter value will be updated
false: If the count couldn't be incremented by the parameter value without getting out of range. The counter will keep its current value.  
### Use example:  
**`myClickCounter.countUp();`**  //Increments the current count by 1  
**`myClickCounter.countUp(2);`**  //Increments the current count by 2  
**`myClickCounter.countUp(-2);`**  //Increments the current count by 2  

---
## **getCount()**;  
### Description:
Gets the current value of the counter.  
### Parameters:  
None.  
### Return value:  
The current value held by the counter, due to the 4 digits limitations, it will be in the range (-1)*(pow(10, (dspDigits - 1)) - 1) <= counter <= (pow(10, dspDigits) - 1)  
### Use example:  
**`long finalCount = myClickCounter.getCount();`**  

---
## **getStartVal()**  
### Description:
Gets the value passed as parameter when the `**.countBegin()**` method was invoked, starting the counter. This is the value that will be used for the `**.countReset()**` method.  
### Parameters:  
None.  
### Return value:  
The value to wich the counter was originally started, it will be in the range (-1)*(pow(10, (dspDigits - 1)) - 1) <= counter <= (pow(10, dspDigits) - 1)  
### Use example:  
**`long countStarted = myClickCounter.getStartVal();`**  

---
### **noBlink**();
### Description:
Refer to **SevenSeg74HC595::noBlink()** method.  
### Parameters:  
None
### Return value:  
true: If the display was set to blink, and the blinking is stopped.   
false: If the display was not set to blink, no changes will be done.
### Use example:  
**`myClickCounter.noBlink();`**

---
### **setBlinkRate**();
### Description:
Refer to **SevenSeg74HC595::setBlinkRate()** method.  
### Parameters:  
None
### Return value:  
Refer to **SevenSeg74HC595::setBlinkRate()** method.  
### Use example:  
**`myClickCounter.setBlinkRate(400);`** //Returns true and sets the blinking rate to 400 millisecs on, 400 millisecs off (symmetrical blink).  
**`myClickCounter.setBlinkRate(800, 200);`** //Returns true and sets the blinking rate to 800 millisecs on, 200 millisecs off (asymmetrical blink)  
**`myClickCounter.setBlinkRate(600, getMaxBlinkRate() + 500);`** //Returns false and the display blinking rate stays without change.  

---
## **updDisplay()**  
### Description:
Updates the display with the current value of the counter. This method is not usually needed as each method that affect the counter value takes care of refreshing the display, but is provided in case of need if some external manipulation of the display is needed to execute, so that the current count value can be displayed again.  
### Parameters:  
None
### Return value:  
true: The value could be restored.  
false: Te value couldn't be restored, the count value was out of range.  
### Use example:  
**`myClickCounter.updDisplay();`** //Returns true and displays the current counter value.  