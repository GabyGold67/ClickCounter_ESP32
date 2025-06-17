# **Included Methods for ClickCounter class**  

## [Complete library documentation HERE!](https://gabygold67.github.io/ClickCounter_ESP32/)

|Method | Parameters|
|---|---|
|**_ClickCounter_** |SevenSegDisplays* **cntrDsplyPtr**(, bool **rgthAlgn**(, bool **zeroPad**))|
||int32_t **countMin**, int32_t **countMax**|
|**_~ClickCounter_** |None|
|**begin()**|(int32_t **startVal**)|
|**blink()**|None|
||unsigned long **onRate** (,unsigned long **offRate**)|
|**clear()**|None|
|**countDown()**|(int32_t **qty**)|
|**countIsZero()**|None|
|**countReset()**|None|
|**countRestart()**|(int32_t **restartValue**)|
|**countToZero()**|(int32_t **qty**)|
|**countUp()**|(int32_t **qty**)|
|**end()**|None|
|**getCount()**|None|
|**getMaxBlinkRate()**|None|
|**getMaxCountVal()**|None|
|**getMinBlinkRate()**|None|
|**getMinCountVal()**|None|
|**getStartVal()**|None|
|**noBlink()**|None|
|**setBlinkRate()**|unsigned long **newOnRate**, (unsigned long **newOffRate**)|
|**updDisplay()**|None|
