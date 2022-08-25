#include <Arduino.h>
#include "task1.h"
#include "events.h"

void task1()
{
    enum class TaskState
    {
        INIT,
        WAIT_COMMANDS
    };
    static TaskState taskState = TaskState::INIT;

    switch (taskState)
    {
    case TaskState::INIT:
    {
        Serial.begin(115200);
        taskState = TaskState::WAIT_COMMANDS;
        break;
    }
    case TaskState::WAIT_COMMANDS:
    {
        if (Serial.available() > 0)
        {
            int dataRx = Serial.read();
            if (dataRx == '1')
            {
                Serial.print("BTN 1\n");
                buttonEvt.trigger = true;
                buttonEvt.whichButton = BUTTONS::BTN_ONE;
            }
            if (dataRx == '2')
            {
                Serial.print("BTN 2\n");
                buttonEvt.trigger = true;
                buttonEvt.whichButton = BUTTONS::BTN_TWO;
            }
        }
        break;
    }
    default:
    {
    }
    }
}