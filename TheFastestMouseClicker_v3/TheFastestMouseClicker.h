#pragma once

#include "ui_MainWindow.h"
#include "MyCustomEvent.h"
#include "HookThread.h"
#include "PostThread.h"

#include <QtWidgets/QMainWindow>
#include <QMouseEvent>
#include <QMessageBox>


class TheFastestMouseClicker : public QMainWindow
{
    Q_OBJECT
public:
    TheFastestMouseClicker();
    ~TheFastestMouseClicker();
    //void postMyCustomEvent(const int customData1, const int customData2);
    //void postMyWarningEvent(const QString& txt);

protected:
    void customEvent(QEvent* event);
    void closeEvent(QCloseEvent* event);
    void changeEvent(QEvent* event);

protected slots:
    void onTriggerChanged(int index);
    void onEventChanged(int index);
    void onConditionChanged(int index);

    void onMeanIntervalChangedText();
    void onRelDevIntervalChangedText();
    void onRadiusChangedText();

private:
    void handleMyMouseCoordsEvent(const MyMouseCoordsEvent* event);
    void handleMyClickingModeOnOffEvent(const MyClickingModeOnOffEvent* event);
    void onTimerTimeout();

private:
    void showMinimizedTip();
    void hideMinimizedTip();
    void updateTipInfo();

public:
    Ui_MainWindow ui;

private:
    HookThread m_hookThread;
    PostThread m_postThread;

    QMessageBox* m_msgBox;
};
