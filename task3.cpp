#include <Arduino.h>
#include "task3.h"
#include "events.h"

static bool compareKeys(BUTTONS *pSecret, BUTTONS *pKey)
{
    bool correct = true;
    for (uint8_t i = 0; i < 5; i++)
    {
        if (pSecret[i] != pKey[i])
        {
            correct = false;
            break;
        }
    }

    return correct;
}

void task3()
{
    enum class TaskStates
    {
        INIT,
        SLOW_MODE,
        WAIT_SLOW,
        PERM_OFF,
        MID_MODE,
        WAIT_MID,
        PERM_ON,
        RAPID_MODE
    };

    static TaskStates taskState = TaskStates::INIT;
    const uint8_t led = 14;
    static uint8_t keyCounter;
    static bool ledBlink;
    static uint8_t currentstate;

    static uint32_t lastTime;
    static constexpr uint32_t SLOW = 1000U;
    static constexpr uint32_t MID = 500U;
    static constexpr uint32_t FAST = 250U;

    static BUTTONS secret[5] = {BUTTONS::BTN_ONE, BUTTONS::BTN_ONE,
                                BUTTONS::BTN_TWO, BUTTONS::BTN_TWO,
                                BUTTONS::BTN_ONE};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};

    switch (taskState)
    {
    case TaskStates::INIT:
    {
        pinMode(led, OUTPUT);
        ledBlink = true;
        digitalWrite(led, ledBlink);

        keyCounter = 0;
        lastTime = millis();
        taskState = TaskStates::SLOW_MODE;

        break;
    }
    case TaskStates::SLOW_MODE:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > SLOW)
        {
            lastTime = currentTime;
            ledBlink = !ledBlink;
            digitalWrite(led, ledBlink);
        }

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE)
            {
                taskState = TaskStates::WAIT_SLOW;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_TWO)
            {
                ledBlink = true;
                digitalWrite(led, ledBlink);
                lastTime = currentTime;
                taskState = TaskStates::MID_MODE;
            }
        }

        break;
    }
    case TaskStates::WAIT_SLOW:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > 1000)
        {
            ledBlink = false;
            digitalWrite(led, ledBlink);
            currentstate = 1;
            taskState = TaskStates::PERM_OFF;
        }
        break;
    }
    case TaskStates::PERM_OFF:
    {

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE)
            {
                ledBlink = true;
                digitalWrite(led, ledBlink);
                lastTime = millis();
                taskState = TaskStates::SLOW_MODE;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_TWO)
            {
                keyCounter = 0;
                ledBlink = true;
                digitalWrite(led, ledBlink);
                currentstate = 1;
                lastTime = millis();
                taskState = TaskStates::RAPID_MODE;
            }
        }
        break;
    }
    case TaskStates::MID_MODE:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > MID)
        {
            lastTime = currentTime;
            digitalWrite(led, ledBlink);
            ledBlink = !ledBlink;
        }
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE)
            {
                currentstate = 2;
                lastTime = currentTime;
                ledBlink = true;
                digitalWrite(led, ledBlink);
                taskState = TaskStates::WAIT_MID;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_TWO)
            {
                ledBlink = true;
                lastTime = currentTime;
                digitalWrite(led, ledBlink);
                taskState = TaskStates::SLOW_MODE;
            }
        }
        break;
    }
    case TaskStates::WAIT_MID:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > SLOW)
        {
            lastTime = currentTime;
            ledBlink = true;
            digitalWrite(led, ledBlink);
            // LedBñlin sincronizacdor con el estado del led
            currentstate = 2;
            taskState = TaskStates::PERM_ON;
        }
        break;
    }
    case TaskStates::PERM_ON:
    {
        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::BTN_ONE)
            {
                ledBlink = true;
                digitalWrite(led, ledBlink);
                taskState = TaskStates::MID_MODE;
            }
            else if (buttonEvt.whichButton == BUTTONS::BTN_TWO)
            {
                ledBlink = true;
                digitalWrite(led, ledBlink);
                keyCounter = 0;
                currentstate = 2;
                taskState = TaskStates::RAPID_MODE;
            }
        }

        break;
    }
    case TaskStates::RAPID_MODE:
    {
        uint32_t currentTime = millis();
        if ((currentTime - lastTime) > FAST)
        {
            lastTime = currentTime;
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
                    if (currentstate == 1)
                    {
                        lastTime = currentTime;
                        ledBlink = true;
                        digitalWrite(led, ledBlink);
                        taskState = TaskStates::SLOW_MODE;
                    }
                    if (currentstate == 2)
                    {
                        lastTime = currentTime;
                        ledBlink = true;
                        digitalWrite(led, ledBlink);
                        taskState = TaskStates::MID_MODE;
                    }
                }
            }
        }
        break;
    }
    default:
    {

        break;
    }
    }
}
