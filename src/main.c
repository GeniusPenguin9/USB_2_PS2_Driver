#include "input.h"
#include "mouse.h"

int g_init = 0;

int main(int argc, char *argv[])
{
    INPUT_EVENT_STRUCT *event_queue;
    int event_queue_length;


    while (1)
    {
		while (0) // 等待5v电源输入
		{
			if (g_init)
			{
				g_init = 0;
				stop_listening();
			}
		}

		if (!g_init)
		{
			g_init = 1;
			mouse_init();
			start_listening();
		}

        get_events(&event_queue, &event_queue_length);

		handle_mouse_messages(event_queue, event_queue_length);
    }
}