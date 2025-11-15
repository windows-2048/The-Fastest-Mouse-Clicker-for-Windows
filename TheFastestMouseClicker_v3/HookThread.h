#pragma once

#include <QtCore/QThread>


class HookThread : public QThread
{
private:
    void run();
};
