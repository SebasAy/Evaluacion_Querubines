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
    bool ledstate=true;
    TaskStates currentState;
    TaskStates lastState = TaskStates::SLOW_MODE;

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
        break;
    }
    case TaskStates::WAIT_CONFIG:
    {
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
        digitalWrite(led, HIGH);
        delay(1000);
        digitalWrite(led, LOW);
        lastState=TaskStates::SLOW_MODE;
        taskState=TaskStates::WAIT_CONFIG;
        break;
    }
    case TaskStates::MID_MODE:
    {
        digitalWrite(led, HIGH);
        delay(500);
        digitalWrite(led, LOW);
        lastState=TaskStates::MID_MODE;
        taskState=TaskStates::WAIT_CONFIG;
        break;
    }
    case TaskStates::RAPID_MODE:
    {
        digitalWrite(led, HIGH);
        delay(250);
        
        lastState=TaskStates::RAPID_MODE;
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
                    taskState=TaskStates::WAIT_CONFIG;
                }
            }
        }
        break;
    }
    default:
    {}
    }



}