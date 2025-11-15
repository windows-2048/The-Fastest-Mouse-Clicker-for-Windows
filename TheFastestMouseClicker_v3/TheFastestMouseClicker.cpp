#include "TheFastestMouseClicker.h"
#include "global_atomic.h"
#include "uiohook.h"

#include <QGuiApplication>
#include <QScreen>
#include <QTimer>

#include <QDoubleValidator>
#include <QLocale>
#include <QSettings>


struct HTrigger
{
    QString text;
    int code;
};


const HTrigger g_all_triggers[] = { 
    {"Mouse Left", MOUSE_BUTTON1}
    , {"Mouse Right", MOUSE_BUTTON2}
    , {"Mouse Wheel (Press)", MOUSE_BUTTON3}
    , {"Wheel Rotating Up", MOUSE_WHEEL_UP}
    , {"Wheel Rotating Down", MOUSE_WHEEL_DOWN}
    , {"Mouse Button4", MOUSE_BUTTON4}
    , {"Mouse Button5", MOUSE_BUTTON5}
    , {"Key Space", VC_SPACE}
    , {"Key Enter", VC_ENTER}
    , {"Key W", VC_W}
    , {"Key A", VC_A}
    , {"Key S", VC_S}
    , {"Key D", VC_D}
    ,  {"Key Up", VC_UP}
    ,  {"Key Left", VC_LEFT}
    ,  {"Key Right", VC_RIGHT}
    ,  {"Key Down", VC_DOWN}
};


const HTrigger g_mode_on_triggers[] = {
    {"Shift+Enter", MASK_SHIFT | VC_ENTER}
};


const HTrigger g_mode_off_triggers[] = {
    {"Shift+Esc", MASK_SHIFT | VC_ESCAPE}
};


struct HClick
{
    QString text;
    int code;
};

/*
#define MOUSE_BUTTON1                            1    // Left Button
#define MOUSE_BUTTON2                            2    // Right Button
#define MOUSE_BUTTON3                            3    // Middle Button
#define MOUSE_BUTTON4                            4    // Extra Mouse Button
#define MOUSE_BUTTON5                            5    // Extra Mouse Button
*/

const HClick g_all_clicks[] = {
    {"Mouse Left", MOUSE_BUTTON1}
    , {"Mouse Right", MOUSE_BUTTON2}
    , {"Mouse Wheel (Click)", MOUSE_BUTTON3}
    , {"Wheel Rotating Up", MOUSE_WHEEL_UP}
    , {"Wheel Rotating Down", MOUSE_WHEEL_DOWN}
    , {"Mouse Button4", MOUSE_BUTTON4}
    , {"Mouse Button5", MOUSE_BUTTON5}
    , {"Key Space", VC_SPACE}
    , {"Key Enter", VC_ENTER}
    , {"Key W", VC_W}
    , {"Key A", VC_A}
    , {"Key S", VC_S}
    , {"Key D", VC_D}
    ,  {"Key Up", VC_UP}
    ,  {"Key Left", VC_LEFT}
    ,  {"Key Right", VC_RIGHT}
    ,  {"Key Down", VC_DOWN}
    ,  {"Key Insert", VC_INSERT}
    ,  {"Key Delete", VC_DELETE}
    ,  {"Key Home", VC_HOME}
    ,  {"Key End", VC_END}
    ,  {"Key PageUp", VC_PAGE_UP}
    ,  {"Key PageDown", VC_PAGE_DOWN}
    ,  {"Key PrintScreen", VC_PRINTSCREEN}
    ,  {"Key ScrollLock", VC_SCROLL_LOCK}
    ,  {"Key PauseBreak", VC_PAUSE}
    ,  {"Key BackSpace", VC_BACKSPACE}
    ,  {"Key F1", VC_F1}
    ,  {"Key F2", VC_F2}
    ,  {"Key F3", VC_F3}
    ,  {"Key F4", VC_F4}
    ,  {"Key F5", VC_F5}
    ,  {"Key F6", VC_F6}
    ,  {"Key F7", VC_F7}
    ,  {"Key F8", VC_F8}
    ,  {"Key F9", VC_F9}
    ,  {"Key F10", VC_F10}
    ,  {"Key F11", VC_F11}
    ,  {"Key F12", VC_F12}
};

/*
typedef enum _HConditionType
{
    while_pressed
    , press_begin_press_end
} HConditionType;
*/

struct HCondition
{
    QString text;
    int end_code;
};


const HCondition g_all_conditions[] = {
    {"Keeps Pressed", CONDITION_PRESS_SELF}
    , {"Toggles On Each Hit", CONDITION_TOGGLE_SELF}
};


const HCondition g_all_conditions_wheelrot[] = {
    {"Is Rotating", CONDITION_PRESS_SELF}
    , {"Toggles On Each Rotate", CONDITION_TOGGLE_SELF}
};


void AppSettings_setValue(const QString& key, const QVariant& value)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
        "TheFastestMouseClickerv3", "TheFastestMouseClickerv3");
    settings.setValue(key, value);
    settings.sync();
}

QVariant AppSettings_getValue(const QString& key, const QVariant& defaultValue)
{
    QSettings settings(QSettings::IniFormat, QSettings::UserScope,
        "TheFastestMouseClickerv3", "TheFastestMouseClickerv3");
    return settings.value(key, defaultValue);
}


class NonNegativeDoubleValidator : public QDoubleValidator
{
public:
    explicit NonNegativeDoubleValidator(QObject* parent = nullptr, int d = 1, double maxv = std::numeric_limits<double>::max())
        : QDoubleValidator(0.0, maxv, d, parent)
        , m_d(d)
        , m_maxv(maxv)
    {
        // Set Notation to accept both standard and scientific notation
        setNotation(QDoubleValidator::ScientificNotation);
    }

    QValidator::State validate(QString& input, int& pos) const override
    {
        // Trim whitespace from input
        QString trimmed = input.trimmed();

        // Allow empty input during editing (user might be in the middle of typing)
        if (trimmed.isEmpty()) {
            return QValidator::Intermediate;
        }

        // Check for explicit negative sign - immediate rejection
        if (trimmed.startsWith('-')) {
            return QValidator::Invalid;
        }

        // First check with parent class validation for basic double format
        QValidator::State state = QDoubleValidator::validate(trimmed, pos);

        // For intermediate state (incomplete numbers like "1." or "0e"), allow continuation
        if (state == QValidator::Intermediate) {
            // Now perform the non-negative validation on acceptable input
            bool ok;
            double value = QLocale::c().toDouble(trimmed, &ok);

            if (ok && value > m_maxv) {
                input = QLocale::c().toString(m_maxv, 'f', m_d);
                pos = 0;
                return QValidator::Invalid;
            }

            return state;
        }

        // Additional check: reject negative zero strings specifically
        if (trimmed.compare("-0") == 0 || trimmed.compare("-0.") == 0 ||
            trimmed.startsWith("-0.0") || trimmed.startsWith("-0e") || trimmed.startsWith("-0E")) {
            return QValidator::Invalid;
        }

        // Now perform the non-negative validation on acceptable input
        bool ok;
        double value = QLocale::c().toDouble(trimmed, &ok);

        // If conversion fails or value is negative, reject
        if (!ok || value < 0.0) {
            return QValidator::Invalid;
        }

        if (value > m_maxv) {
            input = QLocale::c().toString(m_maxv, 'f', m_d);
            pos = 0;
            return QValidator::Invalid;
        }

        if (trimmed.contains("+") || trimmed.contains("e") || trimmed.contains("E"))
        {
            input = QLocale::c().toString(value, 'f', m_d);
            pos = 0;
        }

        return QValidator::Acceptable;
    }

private:
    int m_d;
    double m_maxv;
};


TheFastestMouseClicker::TheFastestMouseClicker() : QMainWindow(), m_msgBox(nullptr)
{
    ui.setupUi(this);

    ui.chbPermanentShow->setChecked(AppSettings_getValue(QString("chbPermanentShow"), QVariant(false)).toBool());

    ui.cbMeanInterval->addItem("1000");
    ui.cbMeanInterval->addItem("500");
    ui.cbMeanInterval->addItem("100");
    ui.cbMeanInterval->addItem("50");
    ui.cbMeanInterval->addItem("10");
    ui.cbMeanInterval->addItem("5");
    ui.cbMeanInterval->addItem("1");

    ui.cbMeanInterval->setCurrentText(AppSettings_getValue(QString("cbMeanInterval"), QVariant(QString("1000"))).toString());

    ui.cbMeanInterval->setValidator(new NonNegativeDoubleValidator(ui.cbMeanInterval, 3));

    ui.cbRelativeIntervalDeviation->addItem("0");
    ui.cbRelativeIntervalDeviation->addItem("1");
    ui.cbRelativeIntervalDeviation->addItem("2");
    ui.cbRelativeIntervalDeviation->addItem("3");
    ui.cbRelativeIntervalDeviation->addItem("4");
    ui.cbRelativeIntervalDeviation->addItem("5");
    ui.cbRelativeIntervalDeviation->addItem("10");
    ui.cbRelativeIntervalDeviation->addItem("20");
    ui.cbRelativeIntervalDeviation->addItem("30");
    ui.cbRelativeIntervalDeviation->addItem("40");
    ui.cbRelativeIntervalDeviation->addItem("50");

    ui.cbRelativeIntervalDeviation->setCurrentText(AppSettings_getValue(QString("cbRelativeIntervalDeviation"), QVariant(QString("0"))).toString());

    ui.cbRelativeIntervalDeviation->setValidator(new NonNegativeDoubleValidator(ui.cbRelativeIntervalDeviation, 1, 100));

    ui.cbRadius->addItem("0");
    ui.cbRadius->addItem("1");
    ui.cbRadius->addItem("2");
    ui.cbRadius->addItem("3");
    ui.cbRadius->addItem("4");
    ui.cbRadius->addItem("5");
    ui.cbRadius->addItem("10");
    ui.cbRadius->addItem("20");
    ui.cbRadius->addItem("30");
    ui.cbRadius->addItem("40");
    ui.cbRadius->addItem("50");

    ui.cbRadius->setCurrentText(AppSettings_getValue(QString("cbRadius"), QVariant(QString("0"))).toString());

    ui.cbRadius->setValidator(new NonNegativeDoubleValidator(ui.cbRadius, 1, 60000));

    onMeanIntervalChangedText();
    onRelDevIntervalChangedText();
    onRadiusChangedText();

    for (const auto& trigger : g_all_triggers)
    {
        ui.cbTrigger->addItem(trigger.text, trigger.code);
    }

    int idxTrigger = AppSettings_getValue(QString("cbTrigger"), QVariant(0)).toInt();
    ui.cbTrigger->setCurrentIndex(idxTrigger);

    onTriggerChanged(idxTrigger);

    int idxEvent = AppSettings_getValue(QString("cbEvent"), QVariant(0)).toInt();
    ui.cbEvent->setCurrentIndex(idxEvent);

    onEventChanged(idxEvent);

    int idxCondition = AppSettings_getValue(QString("cbCondition"), QVariant(0)).toInt();
    ui.cbCondition->setCurrentIndex(idxCondition);

    onConditionChanged(idxCondition);

    for (const auto& mode_on_trigger : g_mode_on_triggers)
    {
        ui.cbClickingModeOn->addItem(mode_on_trigger.text, mode_on_trigger.code);
    }

    for (const auto& mode_off_trigger : g_mode_off_triggers)
    {
        ui.cbClickingModeOff->addItem(mode_off_trigger.text, mode_off_trigger.code);
    }

    ui.leClickingMode->setText("Off");

    QObject::connect(ui.cbTrigger, SIGNAL(activated(int)), this, SLOT(onTriggerChanged(int)));
    QObject::connect(ui.cbEvent, SIGNAL(activated(int)), this, SLOT(onEventChanged(int)));
    QObject::connect(ui.cbCondition, SIGNAL(activated(int)), this, SLOT(onConditionChanged(int)));

    QObject::connect(ui.cbMeanInterval, SIGNAL(editTextChanged(QString)), this, SLOT(onMeanIntervalChangedText()));
    QObject::connect(ui.cbRelativeIntervalDeviation, SIGNAL(editTextChanged(QString)), this, SLOT(onRelDevIntervalChangedText()));
    QObject::connect(ui.cbRadius, SIGNAL(editTextChanged(QString)), this, SLOT(onRadiusChangedText()));

    m_hookThread.start();
    m_postThread.start();

    // Create a timer that fires every 100ms (10 times per second)
    QTimer* timer = new QTimer(this);
    QObject::connect(timer, &QTimer::timeout, this, &TheFastestMouseClicker::onTimerTimeout);
    timer->start(1000); // milliseconds
}


void TheFastestMouseClicker::updateTipInfo()
{
    uint64_t noc = atomic_reader_thread(g_shared_data_number_of_clicks);
    ui.leNumberOfClicks->setText(QString("%1").arg(noc));

    if (m_msgBox)
    {
        m_msgBox->setText(QString("%1 triggers %2 if %3 every %4ms +- %5% inside %6px; Total clicks: %7; Mouse Coords: (%8,%9)")
            .arg(ui.cbTrigger->currentText())
            .arg(ui.cbEvent->currentText())
            .arg(ui.cbCondition->currentText())
            .arg(ui.cbMeanInterval->currentText())
            .arg(ui.cbRelativeIntervalDeviation->currentText())
            .arg(ui.cbRadius->currentText())
            .arg(ui.leNumberOfClicks->text())
            .arg(ui.leMousePosX->text())
            .arg(ui.leMousePosY->text())
        );
    }
}


void TheFastestMouseClicker::onTimerTimeout()
{
    updateTipInfo();
}


TheFastestMouseClicker::~TheFastestMouseClicker()
{
    AppSettings_setValue("chbPermanentShow", QVariant(ui.chbPermanentShow->isChecked()));

    AppSettings_setValue("cbMeanInterval", QVariant(QString(ui.cbMeanInterval->currentText())));
    AppSettings_setValue("cbRelativeIntervalDeviation", QVariant(QString(ui.cbRelativeIntervalDeviation->currentText())));
    AppSettings_setValue("cbRadius", QVariant(QString(ui.cbRadius->currentText())));

    AppSettings_setValue("cbTrigger", QVariant(ui.cbTrigger->currentIndex()));
    AppSettings_setValue("cbEvent", QVariant(ui.cbEvent->currentIndex()));
    AppSettings_setValue("cbCondition", QVariant(ui.cbCondition->currentIndex()));
}


void TheFastestMouseClicker::onTriggerChanged(int index)
{
    int idxEv = ui.cbEvent->currentIndex();
    int maxCntEv = ui.cbEvent->maxCount();

    ui.cbEvent->clear();

    for (const auto& click : g_all_clicks)
    {
        if (ui.cbTrigger->currentData().toInt() != click.code) {
            ui.cbEvent->addItem(click.text, click.code);
        }
    }

    ui.cbEvent->setCurrentIndex(((idxEv >= maxCntEv) || (idxEv < 0)) ? 0 : idxEv);

    int idxCo = ui.cbCondition->currentIndex();
    int maxCntCo = ui.cbCondition->maxCount();

    ui.cbCondition->clear();

    if ((ui.cbTrigger->currentData().toInt() != MOUSE_WHEEL_UP) && (ui.cbTrigger->currentData().toInt() != MOUSE_WHEEL_DOWN))
    {
        for (const auto& cond : g_all_conditions)
        {
            ui.cbCondition->addItem(QString("%1 %2").arg(ui.cbTrigger->currentText()).arg(cond.text), cond.end_code);
        }
    }
    else
    {
        for (const auto& cond : g_all_conditions_wheelrot)
        {
            ui.cbCondition->addItem(QString("%1 %2").arg(ui.cbTrigger->currentText()).arg(cond.text), cond.end_code);
        }
    }

    for (const auto& trigger : g_all_triggers)
    {
        if (
            (ui.cbTrigger->currentData().toInt() != trigger.code)
            && (ui.cbEvent->currentData().toInt() != trigger.code)
            )
        {
            ui.cbCondition->addItem(QString("Begin: %1; End: %2").arg(ui.cbTrigger->currentText()).arg(trigger.text), trigger.code);
        }
    }

    ui.cbCondition->setCurrentIndex(((idxCo >= maxCntCo) || (idxCo < 0)) ? 0 : idxCo);

    atomic_writer_thread(g_shared_data_trigger, uint64_t(ui.cbTrigger->currentData().toInt()));
    atomic_writer_thread(g_shared_data_event, uint64_t(ui.cbEvent->currentData().toInt()));
    atomic_writer_thread(g_shared_data_condition, uint64_t(ui.cbCondition->currentData().toInt()));
}


void TheFastestMouseClicker::onEventChanged(int index)
{
    int idxCo = ui.cbCondition->currentIndex();
    int maxCntCo = ui.cbCondition->maxCount();

    ui.cbCondition->clear();

    if ((ui.cbTrigger->currentData().toInt() != MOUSE_WHEEL_UP) && (ui.cbTrigger->currentData().toInt() != MOUSE_WHEEL_DOWN))
    {
        for (const auto& cond : g_all_conditions)
        {
            ui.cbCondition->addItem(QString("%1 %2").arg(ui.cbTrigger->currentText()).arg(cond.text), cond.end_code);
        }
    }
    else
    {
        for (const auto& cond : g_all_conditions_wheelrot)
        {
            ui.cbCondition->addItem(QString("%1 %2").arg(ui.cbTrigger->currentText()).arg(cond.text), cond.end_code);
        }
    }

    for (const auto& trigger : g_all_triggers)
    {
        if (
            (ui.cbTrigger->currentData().toInt() != trigger.code)
            && (ui.cbEvent->currentData().toInt() != trigger.code)
            )
        {
            ui.cbCondition->addItem(QString("Begin: %1; End: %2").arg(ui.cbTrigger->currentText()).arg(trigger.text), trigger.code);
        }
    }

    ui.cbCondition->setCurrentIndex(((idxCo >= maxCntCo) || (idxCo < 0)) ? 0 : idxCo);

    atomic_writer_thread(g_shared_data_event, uint64_t(ui.cbEvent->currentData().toInt()));
    atomic_writer_thread(g_shared_data_condition, uint64_t(ui.cbCondition->currentData().toInt()));
}


void TheFastestMouseClicker::onConditionChanged(int index)
{
    atomic_writer_thread(g_shared_data_condition, uint64_t(ui.cbCondition->currentData().toInt()));
}


void TheFastestMouseClicker::onMeanIntervalChangedText()
{
    fprintf(stderr, "TheFastestMouseClicker::onMeanIntervalChanged(): %s\n", ui.cbMeanInterval->currentText().toStdString().c_str());
    bool ok = false;
    double val = QLocale::c().toDouble(ui.cbMeanInterval->currentText(), &ok);
    if (ok)
        atomic_writer_thread(g_shared_data_clicking_mean_interval, val);
}


void TheFastestMouseClicker::onRelDevIntervalChangedText()
{
    fprintf(stderr, "TheFastestMouseClicker::onRelDevIntervalChangedText(): %s\n", ui.cbRelativeIntervalDeviation->currentText().toStdString().c_str());
    bool ok = false;
    double val = QLocale::c().toDouble(ui.cbRelativeIntervalDeviation->currentText(), &ok);
    if (ok)
        atomic_writer_thread(g_shared_data_clicking_reldev_interval,val);
}


void TheFastestMouseClicker::onRadiusChangedText()
{
    fprintf(stderr, "TheFastestMouseClicker::onRadiusChangedText(): %s\n", ui.cbRadius->currentText().toStdString().c_str());
    bool ok = false;
    double val = QLocale::c().toDouble(ui.cbRadius->currentText(), &ok);
    if (ok)
        atomic_writer_thread(g_shared_data_clicking_radius, val);
}

/*
void TheFastestMouseClicker::postMyCustomEvent(const int customData1, const int customData2)
{
    // This method (postMyCustomEvent) can be called from any thread

    QApplication::postEvent(this, new MyCustomEvent(customData1, customData2));
}

void TheFastestMouseClicker::postMyWarningEvent(const QString& txt)
{
    // This method (postMyWarningEvent) can be called from any thread

    QApplication::postEvent(this, new MyWarningEvent(txt));
}
*/


void TheFastestMouseClicker::customEvent(QEvent* event)
{
    // When we get here, we've crossed the thread boundary and are now
    // executing in the Qt object's thread

    if (event->type() == MY_MOUSECOORDS_EVENT)
    {
        handleMyMouseCoordsEvent(static_cast<MyMouseCoordsEvent*>(event));
    }
    else if (event->type() == MY_CLICKINGMODEONOFF_EVENT)
    {
        handleMyClickingModeOnOffEvent(static_cast<MyClickingModeOnOffEvent*>(event));
    }

    // use more else ifs to handle other custom events
}


void TheFastestMouseClicker::closeEvent(QCloseEvent* event)
{
    m_postThread.terminate();
    m_postThread.wait();

    int status = hook_stop();
    if (status == 0)
    {
        m_hookThread.terminate();
        m_hookThread.wait();
        event->accept();
    }
    else
    {
        m_hookThread.terminate();
        abort();
    }

    if (m_msgBox)
        delete m_msgBox;
    m_msgBox = nullptr;
}


void TheFastestMouseClicker::changeEvent(QEvent* event)
{
    QMainWindow::changeEvent(event);

    if (event->type() == QEvent::WindowStateChange) {
        updateTipInfo();
    }
}


void TheFastestMouseClicker::showMinimizedTip()
{
    if (m_msgBox)
        delete m_msgBox;
    m_msgBox = nullptr;

    m_msgBox = new QMessageBox(nullptr);
    m_msgBox->setModal(false); // Ensure it's modal
    // Set as application modal
    //m_msgBox->setWindowModality(Qt::ApplicationModal);
    m_msgBox->setWindowTitle("Information");

    updateTipInfo();

    m_msgBox->setIcon(QMessageBox::Information);
    m_msgBox->setStandardButtons(QMessageBox::NoButton);

    // Make it non-modal and tool window
    m_msgBox->setWindowFlags(Qt::Tool | Qt::FramelessWindowHint | Qt::WindowStaysOnTopHint);

    // First show the message box to ensure it has proper size
    m_msgBox->show();
    m_msgBox->ensurePolished();
    m_msgBox->adjustSize();

    QScreen* primaryScreen = QGuiApplication::primaryScreen();
    QRect availableGeometry = primaryScreen->availableGeometry();

    // Calculate position using logical coordinates (Qt handles DPI automatically)
    int margin = 0;
    QPoint pos(
        availableGeometry.right() - m_msgBox->width() - margin,
        availableGeometry.bottom() - m_msgBox->height() - margin
    );

    m_msgBox->move(pos);

    // Auto-close after 3 seconds
    if (!ui.chbPermanentShow->isChecked())
        QTimer::singleShot(3000, m_msgBox, &QMessageBox::hide);
}


void TheFastestMouseClicker::hideMinimizedTip()
{
    if (m_msgBox)
        delete m_msgBox;
    m_msgBox = nullptr;
}


void TheFastestMouseClicker::handleMyMouseCoordsEvent(const MyMouseCoordsEvent* event)
{
    // Now you can safely do something with your Qt objects.
    // Access your custom data using event->getCustomData1() etc.
    ui.leMousePosX->setText(QString("%1").arg(event->getMouseCoordsX()));
    ui.leMousePosY->setText(QString("%1").arg(event->getMouseCoordsY()));
}


void TheFastestMouseClicker::handleMyClickingModeOnOffEvent(const MyClickingModeOnOffEvent* event)
{
    /*
    QMessageBox msgBox;
    msgBox.setStandardButtons(QMessageBox::NoButton);
    msgBox.setText(event->getTxt());
    QTimer::singleShot(2000, &msgBox, &QMessageBox::accept);
    msgBox.exec();
    */

    if (event->getOn())
    {
        setEnabled(false);
        showMinimized();
        showMinimizedTip();
    }
    else
    {
        hideMinimizedTip();
        showNormal();
        setEnabled(true);
    }

    ui.leClickingMode->setText(event->getOn() ? "On" : "Off");
}
