/**
 ******************************************************************************
 * @file CC_ESP32_Example_01.ino
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
 *       Last update:   17/06/2025 15:00 GMT+0200 DST  
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
   const long testTime{1500};
   bool myBlinkMask[4] {true, true, true, true};

   const uint8_t dio {GPIO_NUM_17};  // Pin connected to DIO of TM1637
   const uint8_t clk {GPIO_NUM_16}; // Pin connected to CLK of TM1637
   
   static uint8_t myDispIOPins[2] {clk, dio}; // Pins set as an array as required by hw constructor

   uint8_t theNewOrder [4] {3, 2, 1, 0};

   SevenSegDispHw* myLedDispHwPtr {new SevenSegTM1637(myDispIOPins, 4, false)};
   myLedDispHwPtr -> setDigitsOrder(theNewOrder);
   SevenSegDisplays myLedDisp(myLedDispHwPtr);
   SevenSegDisplays* myLedDispPtr {&myLedDisp};

   ClickCounter myClickCounter(myLedDispPtr, true, false);

   myLedDisp.begin();
   Serial.println("Display Service Started");

   myClickCounter.begin(100);
   Serial.println("Counter Service Started");
   Serial.print("Minimum value for the counter: ");
   Serial.println(myClickCounter.getMinCountVal());
   Serial.print("Maximum value for the counter: ");
   Serial.println(myClickCounter.getMaxCountVal());
   vTaskDelay(3000);

   for(;;){

      {
         //Counter reset
         Serial.println("\n\nStart the demo, setting the counter to the original starting value");
         myClickCounter.countReset();
         vTaskDelay(2000);
      }

      {
         //Count up
         Serial.println("\nCount up 50 times one by one");
         for(int i{0}; i<50; i++){
            myClickCounter.countUp();
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Count up
         Serial.println("\nCount up 25 times two by two");
         for(int i{0}; i<25; i++){
            myClickCounter.countUp(2);
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Counter reset
         Serial.println("\nReset the counter to the original starting value");
         myClickCounter.countReset();
         vTaskDelay(2000);
      }

      {
         //Count down
         Serial.println("\nCount down 50 times one by one");
         for(int i{0}; i<50; i++){
            myClickCounter.countDown();
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Count down
         Serial.println("Count down 25 times five by five");
         for(int i{0}; i<25; i++){
            myClickCounter.countDown(5);
            if(myClickCounter.countIsZero()){
               myClickCounter.blink(250);
               vTaskDelay(2000);
               myClickCounter.noBlink();
            }
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Counter restart in 50 counts
         Serial.println("\nReset the counter to a starting value of 50");
         myClickCounter.countRestart(50);
         vTaskDelay(2000);
      }

      {
         //Count To Zero
         Serial.println("Count to Zero 10 times five by five");
         for(int i{0}; i<10; i++){
            testResult = myClickCounter.countToZero(5);
            if(!testResult)
               Serial.print("FAIL!!: ");            
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Counter restart in -50 counts
         Serial.println("\nReset the counter to a starting value of -50");
         myClickCounter.countRestart(-50);
         vTaskDelay(2000);
      }

      {
         //Count To Zero, show exceeding error
         Serial.println("Count to Zero 10 times six by six");
         Serial.println("The count will stand when the value exceeds 0!!");
         for(int i{0}; i<10; i++){
            testResult = myClickCounter.countToZero(6);
            if(!testResult)
               Serial.print("FAIL!!: ");            
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Counter restart in (getMaxCountVal() - 20) counts
         Serial.println("\nReset the counter to a starting value of getMaxCountVal() - 20");
         myClickCounter.countRestart(myClickCounter.getMaxCountVal() - 20);
         vTaskDelay(2000);
      }

      {
         //Count Up over the maximum 
         Serial.println("Count Up 8 times three by three");
         for(int i{0}; i<8; i++){
            testResult = myClickCounter.countUp(3);
            if(!testResult)
               Serial.print("FAIL!!: ");            
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }

      {
         //Counter restart in (getMinCountVal() + 15) counts
         Serial.println("\nReset the counter to a starting value of getMinCountVal() + 15");
         myClickCounter.countRestart(myClickCounter.getMinCountVal() + 15);
         vTaskDelay(2000);
      }

      {
         //Count Down below the minimum 
         Serial.println("Count Down 5 times four by four");
         for(int i{0}; i<5; i++){
            testResult = myClickCounter.countDown(4);
            if(!testResult)
               Serial.print("FAIL!!: ");            
            Serial.print(myClickCounter.getCount());
            Serial.print(", ");
            vTaskDelay(500);
         }
         Serial.println(" ");
      }
   
      {
         // vTaskDelay(testTime);
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
//=========================================>> User Functions Implementations END
