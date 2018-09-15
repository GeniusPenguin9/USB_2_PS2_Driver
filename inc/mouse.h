#ifndef _MOUSE_H
#define _MOUSE_H

#include "input.h"

typedef struct _PS2_MOUSE_GENERIC_PACK_STRUCT
{
    char status;
    char movement_x;
    char movement_y;
} PS2_MOUSE_GENERIC_PACK_STRUCT;

typedef struct _PS2_MOUSE_EXTENSION_PACK_STRUCT
{
    char status;
    char movement_x;
    char movement_y;
    char movement_z;
} PS2_MOUSE_EXTENSION_PACK_STRUCT;

typedef enum 
{
    GENERIC_MOUSE = 0x00,
    EXTENDED_MOUSE = 0x03,
    EXTENDED_MOUSE_WITH_BUTTONS = 0x04
} PS2_MOUSE_MODE;

typedef enum
{
    RESET = 0xFF,
    RESEND = 0xFE,
    SET_DEFAULTS = 0xF6,
    DISABLE_DATA_REPORTING = 0xF5,
    ENABLE_DATA_REPORTING = 0xF4,
    SET_SAMPLE_RATE = 0xF3,
    GET_DEVICE_ID = 0xF2,
    SET_REMOTE_MODE = 0xF0,
    SET_WRAP_MODE = 0xEE,
    RESET_WRAP_MODE = 0xEC,
    READ_DATA = 0xEB,
    SET_STREAM_MODE = 0xEA,
    STAUTS_REPORT = 0xE9,
    SET_RESOLUTION = 0xE8,
    SET_SCALING_21 = 0xE7,
    SET_SCALING_11 = 0xE6
} PS2_MOUSE_HOST_COMMANDS;

typedef enum 
{
    ACK = 0xFA,
    BAT_SUCCESS = 0xAA,
    BAT_ERROR = 0xFC
} PS_MOUSE_RESPONSE;

void host_command_received(PS2_MOUSE_HOST_COMMANDS command);
void mouse_event_received(INPUT_EVENT_STRUCT event);

#endif