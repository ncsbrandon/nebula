#!/usr/bin/env python

#---------------------------------------------------------------------------------------------------
# @file
#
# <HR>
#
# Copyright (C) Sierra Wireless Inc. Use of this work is subject to license.
#---------------------------------------------------------------------------------------------------

import jinja2
import sys

code_block_template = """
// GPIO expander {% if expander %}#{{expander}} {% endif %}GPIO {{gpio}}
le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetInput
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Polarity_t polarity
)
{
    return gpioExpander_SetInput(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}}, polarity);
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetPushPullOutput
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetPushPullOutput(
        &GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}}, polarity, value);
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetTriStateOutput
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Polarity_t polarity
)
{
    return gpioExpander_SetTriStateOutput(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}}, polarity);
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetOpenDrainOutput
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Polarity_t polarity,
    bool value
)
{
    return gpioExpander_SetOpenDrainOutput(
        &GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}}, polarity, value);
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_EnablePullUp
(
    void
)
{
    return gpioExpander_EnablePullUp(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_EnablePullDown
(
    void
)
{
    return gpioExpander_EnablePullDown(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_DisableResistors
(
    void
)
{
    return gpioExpander_DisableResistors(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_Activate
(
    void
)
{
    return gpioExpander_Activate(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_Deactivate
(
    void
)
{
    return gpioExpander_Deactivate(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetHighZ
(
    void
)
{
    return gpioExpander_SetHighZ(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

bool mangoh_gpioExp{{expander}}Pin{{gpio}}_Read
(
    void
)
{
    return gpioExpander_Read(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_SetEdgeSense
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Edge_t trigger
)
{
    return gpioExpander_SetEdgeSense(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}}, trigger);
}

mangoh_gpioExp{{expander}}Pin{{gpio}}_Edge_t mangoh_gpioExp{{expander}}Pin{{gpio}}_GetEdgeSense
(
    void
)
{
    return gpioExpander_GetEdgeSense(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

le_result_t mangoh_gpioExp{{expander}}Pin{{gpio}}_DisableEdgeSense
(
    void
)
{
    return gpioExpander_DisableEdgeSense(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

bool mangoh_gpioExp{{expander}}Pin{{gpio}}_IsOutput
(
    void
)
{
    return gpioExpander_IsOutput(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

bool mangoh_gpioExp{{expander}}Pin{{gpio}}_IsInput
(
    void
)
{
    return gpioExpander_IsInput(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

mangoh_gpioExp{{expander}}Pin{{gpio}}_Polarity_t mangoh_gpioExp{{expander}}Pin{{gpio}}_GetPolarity
(
    void
)
{
    return gpioExpander_GetPolarity(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

bool mangoh_gpioExp{{expander}}Pin{{gpio}}_IsActive
(
    void
)
{
    return gpioExpander_IsActive(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

mangoh_gpioExp{{expander}}Pin{{gpio}}_PullUpDown_t mangoh_gpioExp{{expander}}Pin{{gpio}}_GetPullUpDown
(
    void
)
{
    return gpioExpander_GetPullUpDown(&GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %}, {{gpio}});
}

mangoh_gpioExp{{expander}}Pin{{gpio}}_ChangeEventHandlerRef_t mangoh_gpioExp{{expander}}Pin{{gpio}}_AddChangeEventHandler
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_Edge_t trigger,
    mangoh_gpioExp{{expander}}Pin{{gpio}}_ChangeCallbackFunc_t handlerPtr,
    void *contextPtr,
    int32_t sampleMs
)
{
    return (mangoh_gpioExp{{expander}}Pin{{gpio}}_ChangeEventHandlerRef_t)gpioExpander_AddChangeEventHandler(
        &GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %},
        {{gpio}},
        &handlerRecords{% if expander %}[EXPANDER_{{expander}}_INDEX]{% endif %}[{{gpio}}],
        trigger,
        handlerPtr,
        contextPtr,
        sampleMs);
}

void mangoh_gpioExp{{expander}}Pin{{gpio}}_RemoveChangeEventHandler
(
    mangoh_gpioExp{{expander}}Pin{{gpio}}_ChangeEventHandlerRef_t ref
)
{
    gpioExpander_RemoveChangeEventHandler(
        &GpioExpander{% if expander %}s[EXPANDER_{{expander}}_INDEX]{% endif %},
        {{gpio}},
        &handlerRecords{% if expander %}[EXPANDER_{{expander}}_INDEX]{% endif %}[{{gpio}}],
        (gpioExpander_ChangeCallbackRef_t)ref);
}
"""

def generate(platform):
    expanders = range(1, 4) if platform == 'green' else ['']
    for expander in expanders:
        for gpio in range(16):
            t = jinja2.Template(code_block_template)
            print(t.render(expander=expander, gpio=gpio))

if __name__ == '__main__':
    if len(sys.argv) != 2 or (sys.argv[1] != 'green' and sys.argv[1] != 'red'):
        print("This program expects 'red' or 'green' as an argument")
        sys.exit(1)
    generate(sys.argv[1])

