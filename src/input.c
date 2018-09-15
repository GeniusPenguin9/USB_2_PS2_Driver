#include "input.h"
#include <string.h>
#include <pthread.h>

int xi_opcode;
int current_index;
volatile int stop_flag;
volatile int flush_flag;
volatile int event_count;
pthread_t thread;
INPUT_EVENT_STRUCT events_queue[100];

static void _handle_event(XIRawEvent *event)
{
    double *raw_val;
    int i = current_index;

    memset(&events_queue[i], 0, sizeof(INPUT_EVENT_STRUCT));
    events_queue[i].event_type = event->evtype;

    switch (event->evtype)
    {
    case XI_RawButtonPress:
    case XI_RawButtonRelease:
        events_queue[i].mouse_button = event->detail;
        break;
    case XI_RawMotion:
        raw_val = event->raw_values;
        events_queue[i].motion_X = raw_val[0];
        events_queue[i].motion_Y = raw_val[1];
        break;
    case XI_RawKeyPress:
    case XI_RawKeyRelease:
        events_queue[i].key_code = event->detail;
        break;
    }
    current_index++;
}

void *_start_listening(void *message)
{
    Display *display = XOpenDisplay(NULL);
    XIEventMask mask[2];
    XIEventMask *m;
    Window win;
    stop_flag = 0;

    int event, error;
    if (!XQueryExtension(display, "XInputExtension", &xi_opcode, &event, &error))
    {
        fprintf(stderr, "X Input extension not available.\n");
        exit(1);
    }

    setvbuf(stdout, NULL, _IOLBF, 0);

    win = DefaultRootWindow(display);

    m = &mask[0];
    m->deviceid = XIAllDevices;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));

    m = &mask[1];
    m->deviceid = XIAllMasterDevices;
    m->mask_len = XIMaskLen(XI_LASTEVENT);
    m->mask = calloc(m->mask_len, sizeof(char));
    XISetMask(m->mask, XI_RawKeyPress);
    XISetMask(m->mask, XI_RawKeyRelease);
    XISetMask(m->mask, XI_RawButtonPress);
    XISetMask(m->mask, XI_RawButtonRelease);
    XISetMask(m->mask, XI_RawMotion);

    XISelectEvents(display, win, &mask[0], 2);

    XSync(display, False);

    free(mask[0].mask);
    free(mask[1].mask);

    while (!stop_flag)
    {
        XEvent ev;
        XGenericEventCookie *cookie = (XGenericEventCookie *)&ev.xcookie;
        XNextEvent(display, (XEvent *)&ev);

        if (XGetEventData(display, cookie) &&
            cookie->type == GenericEvent &&
            cookie->extension == xi_opcode)
        {
            switch (cookie->evtype)
            {
            case XI_RawKeyPress:
            case XI_RawKeyRelease:
            case XI_RawButtonPress:
            case XI_RawButtonRelease:
            case XI_RawMotion:
                _handle_event(cookie->data);
                break;
            }
        }

        XFreeEventData(display, cookie);

        //XWarpPointer(display, win,  win, 0, 0, 0, 0, 100, 100);
        //XFlush(display);

        if (flush_flag)
        {
            event_count = current_index;
            current_index = 0;
            flush_flag = 0;
        }
    }

    XDestroyWindow(display, win);
}

void start_listening()
{
    int rtn;
    rtn = pthread_create(&thread, NULL, _start_listening, (void *)"input monitoring thread");
    if (rtn)
    {
        fprintf(stderr, "Create thread failed");
        exit(rtn);
    }
}

void stop_listening()
{
    stop_flag = 1;
    pthread_join(thread, NULL);
}

void get_events(OUT INPUT_EVENT_STRUCT **events, OUT int *count)
{
    flush_flag = 1;
    while (flush_flag)
    {
    }

    *events = events_queue;
    *count = event_count;
}
