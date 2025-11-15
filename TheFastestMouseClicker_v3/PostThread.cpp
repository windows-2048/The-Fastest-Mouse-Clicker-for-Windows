#include "PostThread.h"
#include "global_atomic.h"
#include "uiohook.h"


#include <QRandomGenerator>
#include <climits>
#include <cmath>


// Using Qt's built-in method (simpler)
static double getRandomDouble(double minv, double maxv) {
    return QRandomGenerator::global()->generateDouble() * (maxv - minv) + minv;
}


double random_number(double mean_interval, double reldev_interval)
{
    double reld = double(reldev_interval) / 100;
    if (reld > 1)
        reld = 1;
    double minv = mean_interval * (1 - reld);
    double maxv = mean_interval * (1 + reld);
    double rndv = getRandomDouble(minv, maxv);
    if (rndv < 0)
        rndv = 0;
    return rndv;
}


const double PI = 3.14159265358979323846;


static void random_circle(double clicking_radius, uint64_t mouse_coords_x_in, uint64_t mouse_coords_y_in, uint64_t& mouse_coords_x, uint64_t& mouse_coords_y)
{
    double rndangle = getRandomDouble(0, 2 * PI);
    double minr = 0;
    double maxr = clicking_radius;
    double rndr = getRandomDouble(minr, maxr);
    double dx = rndr * cos(rndangle);
    double dy = rndr * sin(rndangle);
    int64_t mousex = int64_t(mouse_coords_x_in) + int64_t(dx);
    int64_t mousey = int64_t(mouse_coords_y_in) + int64_t(dy);
    mouse_coords_x = (mousex >= 0) ? mousex : 0;
    mouse_coords_y = (mousey >= 0) ? mousey : 0;
}


void PostThread::run()
{
    double current_timeout = ULONG_MAX;

	while (true)
	{
		//QThread::msleep(1000);
        g_postMutex.lock();
        unsigned long timeWait = (current_timeout >= 1.0) ? ((unsigned long)current_timeout) : 1;
        g_postWaitCondition.wait(&g_postMutex, timeWait);

        uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode); // Additional guard, despite HookThread

        if (cmode == uint64_t(true))
        {
            uint64_t cemit = atomic_reader_thread(g_shared_data_clicking_emit);

            if (cemit == uint64_t(true))
            {
                uint64_t cevent = atomic_reader_thread(g_shared_data_event);

                uint64_t mouse_coords_x_in = atomic_reader_thread(g_shared_data_mouse_coords_x);
                uint64_t mouse_coords_y_in = atomic_reader_thread(g_shared_data_mouse_coords_y);

                uint64_t x_factor = 1;

                if ((current_timeout < 1.0) && (current_timeout > 0.0))
                {
                    x_factor = uint64_t(1.0 / current_timeout);
                }

                uint64_t mouse_coords_x = mouse_coords_x_in;
                uint64_t mouse_coords_y = mouse_coords_y_in;

                double clicking_radius = atomic_reader_thread(g_shared_data_clicking_radius);

                for (uint64_t x = 0; x < x_factor; ++x)
                {
                    uint64_t cmode_x = atomic_reader_thread(g_shared_data_clicking_mode); // Additional guard, despite HookThread

                    if (cmode_x == uint64_t(true))
                    {
                        uint64_t cemit_x = atomic_reader_thread(g_shared_data_clicking_emit);

                        if (cemit_x == uint64_t(true))
                        {
                            if (clicking_radius > 0)
                            {
                                random_circle(clicking_radius, mouse_coords_x_in, mouse_coords_y_in, mouse_coords_x, mouse_coords_y);
                            }

                            fprintf(stderr, "After .wait(... current_timeout %3.3f) cevent %llu mouse_coords_x %llu mouse_coords_y %llu\n", current_timeout, cevent, mouse_coords_x, mouse_coords_y);

                            if ((cevent >= MOUSE_BUTTON1) && (cevent <= MOUSE_BUTTON5))
                            {
                                uiohook_event m_event;

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_MOUSE_PRESSED;
                                m_event.data.mouse.button = cevent;
                                m_event.data.mouse.x = mouse_coords_x;
                                m_event.data.mouse.y = mouse_coords_y;
                                hook_post_event(&m_event);

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_MOUSE_RELEASED;
                                m_event.data.mouse.button = cevent;
                                m_event.data.mouse.x = mouse_coords_x;
                                m_event.data.mouse.y = mouse_coords_y;
                                hook_post_event(&m_event);

                                fprintf(stderr, "After .wait(...) Done MOUSE\n");
                            }
                            else if ((cevent > MOUSE_BUTTON5) && (cevent < (MOUSE_WHEEL_UP / 2)))
                            {
                                uiohook_event m_event;

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_MOUSE_MOVED;
                                m_event.data.mouse.button = MOUSE_NOBUTTON;
                                m_event.data.mouse.x = mouse_coords_x;
                                m_event.data.mouse.y = mouse_coords_y;
                                hook_post_event(&m_event);

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_KEY_PRESSED;
                                //m_event.data.keyboard.keychar = (cevent < 128) ? cevent : CHAR_UNDEFINED;
                                m_event.data.keyboard.keycode = cevent;
                                hook_post_event(&m_event);

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_KEY_RELEASED;
                                //m_event.data.keyboard.keychar = (cevent < 128) ? cevent : CHAR_UNDEFINED;
                                m_event.data.keyboard.keycode = cevent;
                                hook_post_event(&m_event);

                                fprintf(stderr, "After .wait(...) Done KEY\n");
                            }
                            else if ((cevent == MOUSE_WHEEL_UP) || (cevent == MOUSE_WHEEL_DOWN))
                            {
                                int16_t requested_rotation = (MOUSE_WHEEL_DOWN == cevent) ? 1 : -1;

                                uiohook_event m_event;

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_MOUSE_MOVED;
                                m_event.data.mouse.button = MOUSE_NOBUTTON;
                                m_event.data.mouse.x = mouse_coords_x;
                                m_event.data.mouse.y = mouse_coords_y;
                                hook_post_event(&m_event);

                                memset(&m_event, 0, sizeof(uiohook_event));
                                m_event.type = EVENT_MOUSE_WHEEL;
                                m_event.data.wheel.x = mouse_coords_x; // Maybe non-used by hardware
                                m_event.data.wheel.y = mouse_coords_y;
                                m_event.data.wheel.amount = 3;
                                m_event.data.wheel.rotation = requested_rotation;
                                hook_post_event(&m_event);

                                fprintf(stderr, "After .wait(...) Done WHEEL\n");
                            }

                            atomic_increment(g_shared_data_number_of_clicks);
                        }
                    }
                }

                double mean_interval = atomic_reader_thread(g_shared_data_clicking_mean_interval);
                double reldev_interval = atomic_reader_thread(g_shared_data_clicking_reldev_interval);
                double current_timeout = mean_interval;

                if (reldev_interval > 0)
                {
                    current_timeout = random_number(mean_interval, reldev_interval);
                }

                atomic_writer_thread(g_shared_data_clicking_current_timeout, current_timeout);
            }
        }

        g_postMutex.unlock();

        current_timeout = atomic_reader_thread(g_shared_data_clicking_current_timeout);
	}
}
