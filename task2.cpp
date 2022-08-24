#include <Arduino.h>
#include "task2.h"
#include "events.h"

void task2()
{
    enum class TaskState
    {
        INIT,
        WAIT_PRESS,
        WAIT_RELEASE,
        RAPID_MODE,
        SLOW_MODE,
        MID_MODE
    };
    static TaskState taskState = TaskState::INIT;
    static uint8_t lastButtonPressed;
    static uint32_t initStableTime;

    const uint8_t 1_BTN_PIN = 13;
    const uint8_t 2_BTN_PIN = 32;

    switch (taskState)
    {
    case TaskState::INIT:
    {
        pinMode(1_BTN_PIN, INPUT_PULLUP);
        pinMode(2_BTN_PIN, INPUT_PULLUP);

        taskState = TaskState::SLOW_MODE; 
        break;
    }
    case TaskState::WAIT_PRESS:
    {
        if(digitalRead(1_BTN_PIN) == LOW){
            lastButtonPressed = 1_BTN_PIN;
            initStableTime = millis();
            taskState = TaskState::SLOW_MODE;
        }
        if(digitalRead(2_BTN_PIN) == LOW){
            lastButtonPressed = 2_BTN_PIN;
            initStableTime = millis();
            taskState = TaskState::WAIT_STABLE;
        }
        break;
    }
    case TaskState::WAIT_RELEASE:
    {
        if(digitalRead(lastButtonPressed) == HIGH){
            taskState = TaskState::WAIT_PRESS;
        }
        break;
    }
    case TaskState::SLOW_MODE:
    {
        initStableTime = millis();
        if((millis()-initStableTime)>1000){
            if(lastButtonPressed == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::WAIT_RELEASE;
        }
        if(digitalRead(1_BTN_PIN) == LOW){
            if(lastButtonPressed == 1_BTN_PI) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::LOW_MODE;
            if(digitalRead(2_BTN_PIN) == LOW){
            if(lastButtonPressd == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::RAPID_MODE;
        }
        if(digitalRead(2_BTN_PIN) == LOW){
            if(lastButtonPressd == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::MID_MODE;
    }
    case TaskState::MID_MODE:
    {
        initStableTime = millis();
        if((millis()-initStableTime)>500){
            if(lastButtonPressed == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::WAIT_RELEASE;
        }
        if(digitalRead(1_BTN_PIN) == LOW){
            if(lastButtonPressed == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::MID_MODE;
            if(digitalRead(2_BTN_PIN) == LOW){
            if(lastButtonPressd == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::RAPID_MODE;
        }
        if(digitalRead(2_BTN_PIN) == LOW){
            if(lastButtonPressd == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;e
            taskState = TaskState::LOW_MODE;
    }
    case TaskState::RAPID_MODE:
    {
        initStableTime = millis();
        if((millis()-initStableTime)>250){
            if(lastButtonPressed == 1_BTN_PIN) buttonEvt.whichButton = BUTTONS::O_BTN;
            else if(lastButtonPressed == 2_BTN_PIN) buttonEvt.whichButton = BUTTONS::F_BTN;
            buttonEvt.trigger = true;
            taskState = TaskState::WAIT_RELEASE;
        }
    }

    default:
    {
    }
    }
}