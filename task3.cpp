#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (pSecret[i] != pKey[i]){
            correct = false;
            break;
        }
    }

    return correct;
}

void task3(){
     enum class TaskStates
    {
        INIT,
        WAIT_CONFIG,
        SLOW_MODE,
        MID_MODE,
        RAPID_MODE
    };

    static TaskStates taskState = TaskStates::INIT;
    const uint8_t led = 14;
    static uint8_t keyCounter;
    static bool ledstate = true;
    static bool ledBlink = true;
    TaskStates currentState;
    TaskStates lastState = TaskStates::SLOW_MODE;
    
    static uint32_t lastTime;
    static constexpr uint32_t SLOW = 1000U;
    static constexpr uint32_t MID = 500U;
    static constexpr uint32_t FAST = 250U;

    static BUTTONS secret[5] = {BUTTONS::BTN_ONE, BUTTONS::BTN_ONE,
                                BUTTONS::BTN_TWO, BUTTONS::BTN_TWO,
                                BUTTONS::BTN_ONE
                                };

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};

    switch (taskState)
    {
    case TaskStates::INIT:
    {
        pinMode(led, OUTPUT);
        digitalWrite(led, HIGH);
        ledstate=true;
        keyCounter=0;
        taskState = TaskStates::WAIT_CONFIG;
        lastTime = millis();
        break;
    }
    case TaskStates::WAIT_CONFIG:           
    {
        uint32_t currentTime = millis();
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE && lastState == TaskStates::SLOW_MODE)
            {
                digitalWrite(led, LOW);
                ledstate=false;
                if (buttonEvt.whichButton == BUTTONS::BTN_ONE && ledstate==false)
                {
                    taskState = TaskStates::SLOW_MODE;
                    currentState = TaskStates::SLOW_MODE;
                }
                if (buttonEvt.whichButton == BUTTONS::BTN_TWO && ledstate==false)
                {
                    taskState=TaskStates::RAPID_MODE;
                    keyCounter = 0;
                }
            }
            if (buttonEvt.whichButton == BUTTONS::BTN_TWO && lastState == TaskStates::SLOW_MODE)
            {
                taskState=TaskStates::MID_MODE;
                currentState = TaskStates::MID_MODE;
            }
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE && lastState == TaskStates::MID_MODE)
            {
                digitalWrite(led, HIGH);
                ledstate=true;
                if (buttonEvt.whichButton == BUTTONS::BTN_ONE && ledstate==true)
                {
                    taskState = TaskStates::MID_MODE;
                    currentState = TaskStates::MID_MODE;
                }
                if (buttonEvt.whichButton == BUTTONS::BTN_TWO && ledstate==true)
                {
                    taskState=TaskStates::RAPID_MODE;
                    keyCounter = 0;
                }
            }
            if (buttonEvt.whichButton == BUTTONS::BTN_TWO && lastState == TaskStates::MID_MODE)
            {
                taskState=TaskStates::SLOW_MODE;
                currentState = TaskStates::SLOW_MODE;
            }
        }
        break;
    }
    case TaskStates::SLOW_MODE:
    {
        if( (currentTime - lasTime) >= SLOW ){
                lasTime = currentTime;
                digitalWrite(led, ledBlink);
                ledBlink = !ledBlink;
            }
        lastState=TaskStates::SLOW_MODE;
        taskState=TaskStates::WAIT_CONFIG;
        break;
    }
    case TaskStates::MID_MODE:
    {
         if( (currentTime - lasTime) >= MID ){
                lasTime = currentTime;
                digitalWrite(led, ledBlink);
                ledBlink = !ledBlink;
            }
        lastState=TaskStates::MID_MODE;
        taskState=TaskStates::WAIT_CONFIG;
        break;
    }
    case TaskStates::RAPID_MODE:
    {
        lastState=TaskStates::RAPID_MODE;
         if( (currentTime - lasTime) >= FAST ){
                lasTime = currentTime;
                digitalWrite(led, ledBlink);
                ledBlink = !ledBlink;
            }

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            disarmKey[keyCounter] = buttonEvt.whichButton;
            keyCounter++;
            if (keyCounter == 5)
            {
                keyCounter = 0;
                if (compareKeys(secret, disarmKey) == true)
                {
                    taskState = currentState;
                }
            }
        }
        break;
    }
    default:
    {}
    }



}
