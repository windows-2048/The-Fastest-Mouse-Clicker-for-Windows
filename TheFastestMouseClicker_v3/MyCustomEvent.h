#pragma once

#include <QEvent>


// Define your custom event identifier
const QEvent::Type MY_MOUSECOORDS_EVENT = static_cast<QEvent::Type>(QEvent::User + 1);

// Define your custom event subclass
class MyMouseCoordsEvent : public QEvent
{
public:
    MyMouseCoordsEvent(const int customData1, const int customData2) :
        QEvent(MY_MOUSECOORDS_EVENT),
        m_mouseCoordsX(customData1),
        m_mouseCoordsY(customData2)
    {
    }

    int getMouseCoordsX() const
    {
        return m_mouseCoordsX;
    }

    int getMouseCoordsY() const
    {
        return m_mouseCoordsY;
    }

private:
    int m_mouseCoordsX;
    int m_mouseCoordsY;
};


// Define your custom event identifier
const QEvent::Type MY_CLICKINGMODEONOFF_EVENT = static_cast<QEvent::Type>(QEvent::User + 2);

// Define your custom event subclass
class MyClickingModeOnOffEvent : public QEvent
{
public:
    MyClickingModeOnOffEvent(bool bOn) :
        QEvent(MY_CLICKINGMODEONOFF_EVENT),
        m_On(bOn)
    {
    }

    bool getOn() const
    {
        return m_On;
    }

private:
    bool m_On;
};
