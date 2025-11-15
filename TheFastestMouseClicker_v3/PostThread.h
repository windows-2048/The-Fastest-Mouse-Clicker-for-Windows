#pragma once

#include <QtCore/QThread>


class PostThread : public QThread
{
private:
    void run();
};
