/**
 ******************************************************************************
 * @file CC_ESP32_Example_02.ino
 * 
 * @brief Code example file to demonstrate ClickCounter_ESP32 library use
 * 
 * @details 
 *
 * Repository: https://github.com/GabyGold67/ClickCounter_ESP32  
 * 
 * Framework: Arduino
 * Platform: ESP32
 * 
 * @author	: Gabriel D. Goldman
 * mail <gdgoldman67@hotmail.com>
 * Github <https://github.com/GabyGold67>
 *
 * @date First release: 16/06/2025  
 *       Last update:   07/07/2025 14:30 GMT+0200 DST  
 ******************************************************************************
  * @warning **Use of this library is under your own responsibility**
  * 
  * @warning The use of this library falls in the category described by The Alan 
  * Parsons Project (c) 1980 "Games People play" disclaimer:
  * 
  * Games people play, you take it or you leave it
  * Things that they say aren't alright
  * If I promised you the moon and the stars, would you believe it?
  * 
  * Released into the public domain in accordance with "GPL-3.0-or-later" license terms.
 ******************************************************************************
*/
#include <Arduino.h>
#include <SevenSegDisplays.h>
#include <ClickCounter_Esp32.h>
#include <ButtonToSwitch_ESP32.h>

//==============================================>> General use definitions BEGIN
#define LoopDlyTtlTm 1500 // Time between task unblocking, time taken from the start of the task execution to the next execution 
#define MainCtrlTskPrrtyLvl 4 // Task priority level

static BaseType_t xReturned; /*!<Static variable to keep returning result value from Tasks and Timers executions*/
static BaseType_t errorFlag {pdFALSE};

BaseType_t ssdExecTskCore = xPortGetCoreID();
BaseType_t ssdExecTskPrrtyCnfg = MainCtrlTskPrrtyLvl;
//================================================>> General use definitions END
 
//======================================>> General use function prototypes BEGIN
void fnPrssdBttn1(void *pvParameters);
void fnPrssdBttn2(void *pvParameters);
void fnPrssdBttn5(void *pvParameters);
void fnPrssdBttnRst(void *pvParameters);

void Error_Handler();
//========================================>> General use function prototypes END
 
//====================================>> Task Callback function prototypes BEGIN
void mainCtrlTsk(void *pvParameters);
//======================================>> Task Callback function prototypes END
 
//===========================================>> Tasks Handles declarations BEGIN
TaskHandle_t mainCtrlTskHndl {NULL};
//=============================================>> Tasks Handles declarations END

void setup() { 
   delay(10);  //FTPO Part of the WOKWI simulator additions, for simulation startup needs

   Serial.begin(9600); //FTPO
   Serial.println("\n\n\nTest started"); //FTPO
   Serial.println("============"); //FTPO

   // Create the Main control task for setup and execution of the main code
   xReturned = xTaskCreatePinnedToCore(
      mainCtrlTsk,  // Callback function/task to be called
      "MainControlTask",  // Name of the task
      2048,   // Stack size (in bytes in ESP32, words in FreeRTOS), the minimum value is in the config file, for this is 768 bytes
      NULL,  // Pointer to the parameters for the function to work with
      ssdExecTskPrrtyCnfg, // Priority level given to the task
      &mainCtrlTskHndl, // Task handle
      ssdExecTskCore // Run in the App Core if it's a dual core mcu (ESP-FreeRTOS specific)
   );
   if(xReturned != pdPASS)
      Error_Handler();
}

void loop() {
   vTaskDelete(NULL); // Delete this task -the ESP-Arduino LoopTask()- and remove it from the execution list
}  

//===============================>> User Tasks Implementations BEGIN

void mainCtrlTsk(void *pvParameters){
   delay(10);  //FTPO Part of the WOKWI simulator additions, for simulation startup needs

   //Set of variables and constants needed for the tests
   bool testResult{};
   bool myBlinkMask[4] {true, true, true, true};

   const uint8_t dio {GPIO_NUM_17};  // Pin connected to DIO of TM1637
   const uint8_t clk {GPIO_NUM_16}; // Pin connected to CLK of TM1637

   static uint8_t myDispIOPins[2] {clk, dio}; // Pins set as an array as required by hw constructor

   uint8_t theNewOrder [4] {3, 2, 1, 0};

   // Seven segment display hardware and displays objects instantiation
   SevenSegDispHw* myLedDispHwPtr {new SevenSegTM1637(myDispIOPins, 4, false)};
   myLedDispHwPtr -> setDigitsOrder(theNewOrder);
   SevenSegDisplays myLedDisp(myLedDispHwPtr);
   SevenSegDisplays* myLedDispPtr {&myLedDisp};

   // ClickCounter object instantiation
   ClickCounter myClickCounter(myLedDispPtr, true, false);

   const int8_t ssvmAdd1Pin{GPIO_NUM_15};
   const int8_t ssvmAdd2Pin{GPIO_NUM_2};
   const int8_t ssvmSubs5Pin{GPIO_NUM_0};
   const int8_t ssvmCntRstPin{GPIO_NUM_4};
   
   // Single Service VdblMPBttns instantiation
   SnglSrvcVdblMPBttn ssvmAdd1(ssvmAdd1Pin, true, true, 20, 50);
   SnglSrvcVdblMPBttn ssvmAdd2(ssvmAdd2Pin, true, true, 20, 50);
   SnglSrvcVdblMPBttn ssvmSubs5(ssvmSubs5Pin, true, true, 20, 50);
   SnglSrvcVdblMPBttn ssvmCntRst(ssvmCntRstPin, true, true, 20, 50);

   ssvmAdd1.setFVPPWhnTrnOn(fnPrssdBttn1, &myClickCounter);
   ssvmAdd2.setFVPPWhnTrnOn(fnPrssdBttn2, &myClickCounter);
   ssvmSubs5.setFVPPWhnTrnOn(fnPrssdBttn5, &myClickCounter);
   ssvmCntRst.setFVPPWhnTrnOn(fnPrssdBttnRst, &myClickCounter);

   myLedDisp.begin();
   Serial.println("\nDisplay Service Started");
   Serial.println("=======================");

   myClickCounter.begin(100);
   Serial.println("\nCounter Service Started");
   Serial.println("=======================");
   Serial.print("Minimum value for the counter: ");
   Serial.println(myClickCounter.getMinCountVal());
   Serial.print("Maximum value for the counter: ");
   Serial.println(myClickCounter.getMaxCountVal());

   ssvmAdd1.begin();
   ssvmAdd2.begin();
   ssvmSubs5.begin();
   ssvmCntRst.begin();
   Serial.println("\nButtons Services Started");
   Serial.println("========================");

   vTaskDelay(1000);

   for(;;){

      {
         vTaskDelay(1000);
      }

   }
}

//================================================>> General use functions BEGIN
//==================================================>> General use functions END

//=======================================>> User Functions Implementations BEGIN
/**
 * @brief Error Handling function
 * 
 * Placeholder for a Error Handling function, in case of an error the execution
 * will be trapped in this endless loop
 */
void Error_Handler(){
   for(;;)
   {    
   }
   
   return;
}

void fnPrssdBttn1(void* pvParameters){
   ClickCounter* myClickCounterPtr = static_cast<ClickCounter*>(pvParameters);
   bool result{true};

   result = myClickCounterPtr->countUp();
   Serial.print("Button 1 pressed, count up by 1, new count: ");
   Serial.println(myClickCounterPtr->getCount());

   // return result;
   return;
}

void fnPrssdBttn2(void* pvParameters){
   ClickCounter* myClickCounterPtr = (ClickCounter*) pvParameters;
   bool result{true};

   result = myClickCounterPtr->countUp(2);
   Serial.print("Button 2 pressed, count up by 2, new count: ");
   Serial.println(myClickCounterPtr->getCount());

   // return result;
   return;
}

void fnPrssdBttn5(void* pvParameters){
   ClickCounter* myClickCounterPtr = (ClickCounter*) pvParameters;
   bool result{true};

   result = myClickCounterPtr->countDown(5);
   Serial.print("Button 5 pressed, count down by 5, new count: ");
   Serial.println(myClickCounterPtr->getCount());

   // return result;
   return;
}

void fnPrssdBttnRst(void* pvParameters){
   ClickCounter* myClickCounterPtr = static_cast<ClickCounter*>(pvParameters);
   bool result{true};

   result = myClickCounterPtr->countReset();
   Serial.print("Button RESET pressed, new count: ");
   Serial.println(myClickCounterPtr->getCount());

   // return result;
   return;
}

//=========================================>> User Functions Implementations END
