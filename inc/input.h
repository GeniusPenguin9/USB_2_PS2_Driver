#ifndef _INPUT_H
#define _INPUT_H

#include <X11/extensions/XInput2.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <stdlib.h>

#define IN
#define OUT

typedef enum _EVENT_TYPE_ENUM
{
    EVENT_KEY_PRESS = 13,
    EVENT_KEY_RELEASE,
    EVENT_MOUSE_BUTTON_PRESS,
    EVENT_MOUSE_BUTTON_RELEASE,
    EVENT_MOUSE_MOTION,
} EVENT_TYPE_ENUM;

typedef struct _INPUT_EVENT_STRUCT
{
    EVENT_TYPE_ENUM event_type;
    int key_code;
    int mouse_button;
    double motion_X;
    double motion_Y;
} INPUT_EVENT_STRUCT;

typedef enum _MOUSE_BUTTONS_ENUM
{
    BUTTON_LEFT = 1,
    BUTTON_MIDDLE = 2,
    BUTTON_RIGHT = 3,
    BUTTON_BACK = 4,
    BUTTON_FORWARD = 5,
} MOUSE_BUTTONS_ENUM;

void start_listening();
void stop_listening();
void get_events(OUT INPUT_EVENT_STRUCT **events, OUT int *count);

#endif