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

void task3()
{

    enum class TaskStates
    {
        INIT,
        WAIT_CONFIG,
        COUNTING
    };
    static TaskStates taskState = TaskStates::INIT;
    const uint8_t ledBombCounting = 26;
    const uint8_t ledBombBoom = 25;
    const uint32_t BOMBINTERVAL = 1000U;
    const uint32_t LEDCOUNTERINTERVAL = 500U;

    static uint8_t bombCounter;
    static BUTTONS secret[5] = {BUTTONS::1_BTN, BUTTONS::1_BTN,
                                BUTTONS::2_BTN, BUTTONS::2_BTN,
                                BUTTONS::1_BTN};

    static BUTTONS disarmKey[5] = {BUTTONS::NONE};

    static uint8_t ledBombCountingState;
    static uint32_t initBombTimer;
    static uint32_t initLedCounterTimer;
    static uint8_t keyCounter;

    switch (taskState)
    {
    case TaskStates::INIT:
    {

        pinMode(ledBombCounting, OUTPUT);
        pinMode(ledBombBoom, OUTPUT);
        digitalWrite(ledBombBoom, LOW);
        ledBombCountingState = HIGH;
        digitalWrite(ledBombCounting, ledBombCountingState);
        bombCounter = 20;
        keyCounter = 0;
        taskState = TaskStates::WAIT_CONFIG;
        break;
    }
    case TaskStates::WAIT_CONFIG:
    {

        if (buttonEvt.trigger == true)
        {
            buttonEvt.trigger = false;
            if (buttonEvt.whichButton == BUTTONS::1_BTN)
            {
                if (bombCounter < 60)
                    bombCounter++;
            }
            else if (buttonEvt.whichButton == BUTTONS::2_BTN)
            {
                if (bombCounter > 10)
                    bombCounter--;
            }
        }

        break;
    }
    case TaskStates::COUNTING:
    {

        uint32_t timeNow = millis();

        if ((timeNow - initBombTimer) > BOMBINTERVAL)
        {
            initBombTimer = timeNow;
            bombCounter--;
            Serial.print("Counter: ");
            Serial.print(bombCounter);
            Serial.print("\n");
            if (bombCounter == 0)
            {
                ledBombCountingState = HIGH;
                Serial.print("BOMB BOOM\n");
                digitalWrite(ledBombBoom, HIGH);
                delay(2000);
                digitalWrite(ledBombBoom, LOW);
                digitalWrite(ledBombCounting, ledBombCountingState);
                bombCounter = 20;
                taskState = TaskStates::WAIT_CONFIG;
            }
        }
        if ((timeNow - initLedCounterTimer) > LEDCOUNTERINTERVAL)
        {
            initLedCounterTimer = timeNow;
            ledBombCountingState = !ledBombCountingState;
            digitalWrite(ledBombCounting, ledBombCountingState);
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
                    ledBombCountingState = HIGH;
                    digitalWrite(ledBombCounting, ledBombCountingState);
                    Serial.print("BOMB DISARM\n");
                    bombCounter = 20;
                    taskState = TaskStates::WAIT_CONFIG;
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