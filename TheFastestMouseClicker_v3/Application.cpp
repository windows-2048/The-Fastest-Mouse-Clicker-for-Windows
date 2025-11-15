#include "Application.h"
//#include "TheFastestMouseClicker.h"
//#include "uiohook.h"

#include <inttypes.h>
#include <stdarg.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <uiohook.h>
#include <wchar.h>


//extern TheFastestMouseClicker* g_tfmc;


Application::Application(int argc, char** argv) : QApplication(argc, argv)
{

}

Application::~Application()
{

}

/*
void Application::onButtonStopPressed()
{
    for (int i = 0; i < 275; i++) {
        m_event->type = EVENT_MOUSE_MOVED;
        m_event->data.mouse.button = MOUSE_NOBUTTON;
        m_event->data.mouse.x = i;
        m_event->data.mouse.y = i;
        hook_post_event(m_event);
    }
}
*/

bool Application::notify(QObject* dest, QEvent* ev)
{
    /*
    if ((g_tfmc != nullptr) && (dest == g_tfmc->ui.pbStop) && (ev->type() == QEvent::MouseButtonRelease))
    {
        onButtonStopPressed();
        return QApplication::notify(dest, ev);
    }
    */
    return QApplication::notify(dest, ev);
}
