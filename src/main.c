#include "input.h"

int main(int argc, char* argv[])
{
    INPUT_EVENT_STRUCT* event_queue;
    int event_queue_length;

    start_listening();

    while (1)
    {
        get_events(&event_queue, &event_queue_length);
        for (int i = 0; i < event_queue_length; i++)
        {
            printf("%d,%d,%d,%f,%f\n", event_queue[i].event_type, event_queue[i].key_code, event_queue[i].mouse_button, event_queue[i].motion_X, event_queue[i].motion_Y);
        }
    }
}