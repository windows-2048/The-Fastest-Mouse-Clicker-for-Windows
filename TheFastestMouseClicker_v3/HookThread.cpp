#include "HookThread.h"
#include "global_atomic.h"
#include "MyCustomEvent.h"
#include "TheFastestMouseClicker.h"
#include "uiohook.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <climits>


extern TheFastestMouseClicker* g_tfmc;


bool logger_proc(unsigned int level, const char* format, ...)
{
    bool status = false;

    va_list args;
    switch (level) {
    case LOG_LEVEL_INFO:
        va_start(args, format);
        status = vfprintf(stderr, format, args) >= 0;
        va_end(args);
        break;

    case LOG_LEVEL_WARN:
    case LOG_LEVEL_ERROR:
        va_start(args, format);
        status = vfprintf(stderr, format, args) >= 0;
        va_end(args);
        break;
    }

    return status;
}


bool g_bShift = false;

uint64_t g_toggle_count = 0;


void process_trigger(const uiohook_event* event, event_type pressed, event_type released, uint16_t button)
{
    if (event->type == pressed)
    {
        uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode);

        if (cmode == uint64_t(true))
        {
            uint64_t trigger = atomic_reader_thread(g_shared_data_trigger);

            if (button == trigger)
            {
                uint64_t condition = atomic_reader_thread(g_shared_data_condition);

                if (condition == CONDITION_PRESS_SELF)
                {
                    fprintf(stderr, "EVENT_*_PRESSED %d trigger %llu CONDITION_PRESS_SELF .wakeAll()\n", pressed, trigger);
                    atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                    atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                    g_postWaitCondition.wakeAll();
                }
            }
        }
    }
    else if (event->type == released)
    {
        uint64_t trigger = atomic_reader_thread(g_shared_data_trigger);
        uint64_t condition = atomic_reader_thread(g_shared_data_condition);

        if (button == trigger)
        {
            if (condition == CONDITION_PRESS_SELF) // Don't check cmode - immediate clicking stop
            {
                fprintf(stderr, "EVENT_*_RELEASED %d trigger %llu CONDITION_PRESS_SELF\n", released, trigger);
                atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
            }
            else if (condition == CONDITION_TOGGLE_SELF)
            {
                uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode);

                if (cmode == uint64_t(true))
                {
                    ++g_toggle_count;

                    if ((g_toggle_count % 2) == 1)
                    {
                        fprintf(stderr, "EVENT_*_RELEASED %d trigger %llu CONDITION_TOGGLE_SELF .wakeAll()\n", released, trigger);
                        atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                        atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                        g_postWaitCondition.wakeAll();
                    }
                    else
                    {
                        fprintf(stderr, "EVENT_*_RELEASED %d trigger %llu CONDITION_TOGGLE_SELF\n", released, trigger);
                        atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                        atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
                    }
                }
            }
            else if (condition < (CONDITION_TOGGLE_SELF / 2))
            {
                uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode);

                if (cmode == uint64_t(true))
                {
                    fprintf(stderr, "EVENT_*_RELEASED %d trigger %llu condition %llu .wakeAll()\n", released, trigger, condition);
                    atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                    atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                    g_postWaitCondition.wakeAll();
                }
            }
        }
        else if (button == condition) // Don't check cmode - immediate clicking stop
        {
            fprintf(stderr, "EVENT_*_RELEASED %d trigger %llu condition %llu\n", released, trigger, condition);
            atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
            atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
        }
    }
    else if (event->type == EVENT_MOUSE_WHEEL)
    {
        uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode);

        if (cmode == uint64_t(true))
        {
            uint64_t condition = atomic_reader_thread(g_shared_data_condition);

            if ((MOUSE_WHEEL_UP == condition) || (MOUSE_WHEEL_DOWN == condition))
            {
                int16_t rotation = event->data.wheel.rotation;
                int16_t requested_rotation = (MOUSE_WHEEL_DOWN == condition) ? 1 : -1;

                if (rotation == requested_rotation)
                {
                    fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d condition %llu\n", rotation, condition);
                    atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                    atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
                }
            }
        }
    }
}


void process_wheel(const uiohook_event* event, int16_t rotation)
{
    if (event->type == EVENT_MOUSE_WHEEL)
    {
        uint64_t cmode = atomic_reader_thread(g_shared_data_clicking_mode);

        if (cmode == uint64_t(true))
        {
            uint64_t trigger = atomic_reader_thread(g_shared_data_trigger);

            if ((MOUSE_WHEEL_UP == trigger) || (MOUSE_WHEEL_DOWN == trigger))
            {
                int16_t requested_rotation = (MOUSE_WHEEL_DOWN == trigger) ? 1 : -1;

                if (rotation == requested_rotation)
                {
                    uint64_t condition = atomic_reader_thread(g_shared_data_condition);

                    if (condition == CONDITION_PRESS_SELF)
                    {
                        fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d trigger %llu CONDITION_PRESS_SELF .wakeAll()\n", rotation, trigger);
                        atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                        atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                        g_postWaitCondition.wakeAll();

                        fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d trigger %llu CONDITION_PRESS_SELF\n", rotation, trigger);
                        atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                        atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
                    }
                    else if (condition == CONDITION_TOGGLE_SELF)
                    {
                        ++g_toggle_count;

                        if ((g_toggle_count % 2) == 1)
                        {
                            fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d trigger %llu CONDITION_TOGGLE_SELF .wakeAll()\n", rotation, trigger);
                            atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                            atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                            g_postWaitCondition.wakeAll();
                        }
                        else
                        {
                            fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d trigger %llu CONDITION_TOGGLE_SELF\n", rotation, trigger);
                            atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                            atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(false));
                        }
                    }
                    else if (condition < (CONDITION_TOGGLE_SELF / 2))
                    {
                        fprintf(stderr, "EVENT_MOUSE_WHEEL rotation %d trigger %llu condition %llu .wakeAll()\n", rotation, trigger, condition);
                        atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                        atomic_writer_thread(g_shared_data_clicking_emit, uint64_t(true));
                        g_postWaitCondition.wakeAll();
                    }
                }
            }
        }
    }
}


// NOTE: The following callback executes on the same thread that hook_run() is called 
// from.  This is important because hook_run() attaches to the operating systems
// event dispatcher and may delay event delivery to the target application.
// Furthermore, some operating systems may choose to disable your hook if it 
// takes too long to process.  If you need to do any extended processing, please 
// do so by copying the event to your own queued dispatch thread.
void dispatch_proc(uiohook_event* const event)
{
    /*char buffer[256] = {0};
    size_t length = snprintf(buffer, sizeof(buffer),
        "id=%i,when=%" PRIu64 ",mask=0x%X",
        event->type, event->time, event->mask);*/

    switch (event->type)
    {
    case EVENT_KEY_PRESSED:
        {
            if ((event->data.keyboard.keycode == VC_SHIFT_L) || (event->data.keyboard.keycode == VC_SHIFT_R))
            {
                g_bShift = true;
            }

            if (!g_bShift) // FIXME: key press/release event emits many times per second while key is being pressed
            {
                event_type pressed = EVENT_KEY_PRESSED;
                event_type released = EVENT_KEY_RELEASED;
                uint16_t button = event->data.keyboard.keycode;

                process_trigger(event, pressed, released, button);
            }
        }
        return;

    case EVENT_KEY_RELEASED:
        {
            if ((event->data.keyboard.keycode == VC_SHIFT_L) || (event->data.keyboard.keycode == VC_SHIFT_R))
            {
                g_bShift = false;
            }
            else if ((event->data.keyboard.keycode == VC_ENTER) && g_bShift)
            {
                fprintf(stderr, "EVENT_KEY_RELEASED VC_ENTER && g_bShift cmode true\n");
                QApplication::postEvent(g_tfmc, new MyClickingModeOnOffEvent(true));
                atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                atomic_writer_thread(g_shared_data_clicking_mode, uint64_t(true));
                atomic_writer_thread(g_shared_data_number_of_clicks, uint64_t(0));
                g_toggle_count = 0;
            }
            else if ((event->data.keyboard.keycode == VC_ESCAPE) && g_bShift)
            {
                fprintf(stderr, "EVENT_KEY_RELEASED VC_ESCAPE && g_bShift cmode false\n");
                QApplication::postEvent(g_tfmc, new MyClickingModeOnOffEvent(false));
                atomic_writer_thread(g_shared_data_clicking_current_timeout, double(ULONG_MAX));
                atomic_writer_thread(g_shared_data_clicking_mode, uint64_t(false));
                g_toggle_count = 0;
            }

            if (!g_bShift)
            {
                event_type pressed = EVENT_KEY_PRESSED;
                event_type released = EVENT_KEY_RELEASED;
                uint16_t button = event->data.keyboard.keycode;

                process_trigger(event, pressed, released, button);
            }
        }
        return;

    case EVENT_KEY_TYPED:
        {
           /*snprintf(buffer + length, sizeof(buffer) - length,
                ",keychar=%lc,rawcode=%u",
                (wint_t)event->data.keyboard.keychar,
                event->data.keyboard.rawcode);*/
        }
        return;

    case EVENT_MOUSE_PRESSED:
    case EVENT_MOUSE_RELEASED:
    case EVENT_MOUSE_CLICKED:
    case EVENT_MOUSE_MOVED:
    case EVENT_MOUSE_DRAGGED:
        {
            /*snprintf(buffer + length, sizeof(buffer) - length,
            ",x=%i,y=%i,button=%i,clicks=%i",
            event->data.mouse.x, event->data.mouse.y,
            event->data.mouse.button, event->data.mouse.clicks);*/
            atomic_writer_thread(g_shared_data_mouse_coords_x, uint64_t(event->data.mouse.x));
            atomic_writer_thread(g_shared_data_mouse_coords_y, uint64_t(event->data.mouse.y));
            QApplication::postEvent(g_tfmc, new MyMouseCoordsEvent(event->data.mouse.x, event->data.mouse.y));

            if ((event->type == EVENT_MOUSE_PRESSED) || (event->type == EVENT_MOUSE_RELEASED))
            {
                event_type pressed = EVENT_MOUSE_PRESSED;
                event_type released = EVENT_MOUSE_RELEASED;
                uint16_t button = event->data.mouse.button;

                process_trigger(event, pressed, released, button);
            }
        }
        return;

    case EVENT_MOUSE_WHEEL:
        {
            /*fprintf(stderr,
                "EVENT_MOUSE_WHEEL=%i,amount=%i,rotation=%i\n",
                event->data.wheel.type, event->data.wheel.amount,
                event->data.wheel.rotation);*/

            int16_t rotation = event->data.wheel.rotation;

            process_wheel(event, rotation);

            event_type pressed = EVENT_MOUSE_PRESSED;
            event_type released = EVENT_MOUSE_RELEASED;
            uint16_t button = event->data.mouse.button;

            process_trigger(event, pressed, released, button);
        }
        return;

    default:
        return;
    }

    //fprintf(stdout, "%s\n", buffer);
}

void HookThread::run()
{
    // Set the logger callback for library output.
    hook_set_logger_proc(&logger_proc);

    // Set the event callback for uiohook events.
    hook_set_dispatch_proc(&dispatch_proc);

    // Start the hook and block.
    // NOTE If EVENT_HOOK_ENABLED was delivered, the status will always succeed.
    int status = hook_run();

    switch (status) {
    case UIOHOOK_SUCCESS:
        // Everything is ok.
        break;

        // System level errors.
    case UIOHOOK_ERROR_OUT_OF_MEMORY:
        logger_proc(LOG_LEVEL_ERROR, "Failed to allocate memory. (%#X)", status);
        break;


        // X11 specific errors.
    case UIOHOOK_ERROR_X_OPEN_DISPLAY:
        logger_proc(LOG_LEVEL_ERROR, "Failed to open X11 display. (%#X)", status);
        break;

    case UIOHOOK_ERROR_X_RECORD_NOT_FOUND:
        logger_proc(LOG_LEVEL_ERROR, "Unable to locate XRecord extension. (%#X)", status);
        break;

    case UIOHOOK_ERROR_X_RECORD_ALLOC_RANGE:
        logger_proc(LOG_LEVEL_ERROR, "Unable to allocate XRecord range. (%#X)", status);
        break;

    case UIOHOOK_ERROR_X_RECORD_CREATE_CONTEXT:
        logger_proc(LOG_LEVEL_ERROR, "Unable to allocate XRecord context. (%#X)", status);
        break;

    case UIOHOOK_ERROR_X_RECORD_ENABLE_CONTEXT:
        logger_proc(LOG_LEVEL_ERROR, "Failed to enable XRecord context. (%#X)", status);
        break;


        // Windows specific errors.
    case UIOHOOK_ERROR_SET_WINDOWS_HOOK_EX:
        logger_proc(LOG_LEVEL_ERROR, "Failed to register low level windows hook. (%#X)", status);
        break;


        // Darwin specific errors.
    case UIOHOOK_ERROR_AXAPI_DISABLED:
        logger_proc(LOG_LEVEL_ERROR, "Failed to enable access for assistive devices. (%#X)", status);
        break;

    case UIOHOOK_ERROR_CREATE_EVENT_PORT:
        logger_proc(LOG_LEVEL_ERROR, "Failed to create apple event port. (%#X)", status);
        break;

    case UIOHOOK_ERROR_CREATE_RUN_LOOP_SOURCE:
        logger_proc(LOG_LEVEL_ERROR, "Failed to create apple run loop source. (%#X)", status);
        break;

    case UIOHOOK_ERROR_GET_RUNLOOP:
        logger_proc(LOG_LEVEL_ERROR, "Failed to acquire apple run loop. (%#X)", status);
        break;

    case UIOHOOK_ERROR_CREATE_OBSERVER:
        logger_proc(LOG_LEVEL_ERROR, "Failed to create apple run loop observer. (%#X)", status);
        break;

        // Default error.
    case UIOHOOK_FAILURE:
    default:
        logger_proc(LOG_LEVEL_ERROR, "An unknown hook error occurred. (%#X)", status);
        break;
    }
}
