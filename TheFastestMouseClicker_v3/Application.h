#pragma once

#include <QApplication>
//#include <uiohook.h>


class Application : public QApplication
{
public:
    explicit Application(int argc, char** argv);
    ~Application();

protected:
    bool notify(QObject* dest, QEvent* ev);

private:
    //void onButtonStopPressed();
};
