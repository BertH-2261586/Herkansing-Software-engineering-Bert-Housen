
#include "saveexamdialog.h"
#include "../ToastMessage.h"
#include "../../model/onlineexamen.h"

SaveExamDialog::SaveExamDialog()
{
    setWindowFlags(Qt::Popup);

    m_chooseQuestionView = new ChooseQuestionView(false, true);
    setupTimerLabel();
    setupTimer();
    setupSwitchToggle();

    QPushButton* saveButton = new QPushButton("save");

    m_nameField = new QLineEdit();
    m_nameField->setPlaceholderText("naam van het examen");

    QVBoxLayout* layout = new QVBoxLayout(this);
    layout->addWidget(new QLabel("save examen"));
    layout->addWidget(m_chooseQuestionView);
    layout->addWidget(m_toggleLabel);
    layout->addWidget(m_toggle);
    layout->addWidget(m_timerLabel);
    layout->addWidget(m_timer);

    layout->addWidget(m_nameField);
    layout->addWidget(saveButton);

    m_timer->hide();
    m_timerLabel->hide();

    //Save knop
    connect(saveButton, &QPushButton::pressed, this, [=]{
        if (m_nameField->text().isEmpty())
        {
            ToastMessage* toast = new ToastMessage("You have to input a name", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }
        if (m_nameField->text().length() > 64)
        {
            ToastMessage* toast = new ToastMessage("Your name cant be longer then 64", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }
        if (m_chooseQuestionView->getQuestionSetPaths().isEmpty())
        {
            ToastMessage* toast = new ToastMessage("You have to  select a question", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }
        if (OnlineExamen::doesAlreadyExist(m_nameField->text()))
        {
            ToastMessage* toast = new ToastMessage("Name already exists", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }

        QList<QString> paths = m_chooseQuestionView->getQuestionSetPaths();
        if(m_toggle->isChecked())
        {
            OnlineExamen::saveExamen(paths, m_nameField->text(), m_timer->time());
        }
        else
        {
            OnlineExamen::saveExamen(paths, m_nameField->text(), QTime(0, 0));
        }
        this->close();
    });
}


void SaveExamDialog::setupSwitchToggle() {
    m_toggleLabel = new QLabel("Use a question timer");
    m_toggleLabel->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 15px; "
        );
    m_toggleLabel->adjustSize();
    m_toggleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_toggle = new Switch(this);
    connect(m_toggle, &Switch::toggled, this, &SaveExamDialog::switchToggle);
}

void SaveExamDialog::setupTimerLabel() {
    m_timerLabel = new QLabel("Set a timer for every question");
    m_timerLabel->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 15px; "
        );
    m_timerLabel->adjustSize();
    m_timerLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void SaveExamDialog::setupTimer() {
    m_timer = new QTimeEdit();
    m_timer->setFixedWidth(125);
    m_timer->setStyleSheet("font-size: 15px;");
    m_timer->setDisplayFormat("mm:ss");
    m_timer->setTime(QTime(0, 1, 00));          // Set default time for 1 minute
    m_timer->setMaximumTime(QTime(0, 15, 0));   // Set 15 minutes maximum time
    m_timer->setWrapping(true);

    connect(m_timer, &QTimeEdit::timeChanged, this, &SaveExamDialog::handleTimeChanged);
}

void SaveExamDialog::handleTimeChanged() {
    QTime currentTime = m_timer->time();

    int seconds = currentTime.second();
    int minutes = currentTime.minute();

    // Check if the current time is less than 10 seconds
    if (minutes == 0 && seconds < 10) {
        // If seconds is less than 10, reset to 10 seconds
        m_timer->setTime(QTime(0, 0, 10));
    }
}

void SaveExamDialog::switchToggle(){
    if (m_toggle->isChecked()) {
        m_timer->show();
        m_timerLabel->show();
    }
    else {
        m_timer->hide();
        m_timerLabel->hide();
    }
}
