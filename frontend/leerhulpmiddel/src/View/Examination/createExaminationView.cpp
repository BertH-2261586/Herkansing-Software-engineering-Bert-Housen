#include "createExaminationView.h"
#include "../ToastMessage.h"

CreateExaminationView::CreateExaminationView(QWidget* parent, bool questionSelectOnly) : QWidget(parent) {
    this->setAttribute(Qt::WA_DeleteOnClose, true);
    m_questionSelectOnly = questionSelectOnly;
    m_chooseQuestionView = new ChooseQuestionView(questionSelectOnly, !questionSelectOnly);
    setupGoBack();
    setupTimerLabel();
    setupTimer();
    setupSwitchToggle();
    setupExaminationButton();

    setupLayouts();

	setLayout(m_mainLayout);
}

void CreateExaminationView::setupExaminationButton() {
    m_startExaminationButton = new QPushButton("Start the examination", this);
    m_startExaminationButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText); "
        "   background-color: palette(button);"
        "   border: 2px solid palette(mid);"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   margin-bottom: 25px;"
        "   font-size: 16px;"
        "   max-width: 175px;"
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"
        "}"
    );
    m_startExaminationButton->setCursor(Qt::PointingHandCursor);
    connect(m_startExaminationButton, &QPushButton::clicked, this, &CreateExaminationView::startExamination);
}

void CreateExaminationView::setupTimerLabel() {
    m_timerLabel = new QLabel("Set a timer for every question");
    m_timerLabel->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 15px; "
    );
    m_timerLabel->adjustSize();
    m_timerLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
}

void CreateExaminationView::setupTimer() {
    m_timer = new QTimeEdit();
    m_timer->setFixedWidth(125);
    m_timer->setStyleSheet("font-size: 15px;");
    m_timer->setDisplayFormat("mm:ss");
    m_timer->setTime(QTime(0, 1, 00));          // Set default time for 1 minute
    m_timer->setMaximumTime(QTime(0, 15, 0));   // Set 15 minutes maximum time
    m_timer->setWrapping(true);

    connect(m_timer, &QTimeEdit::timeChanged, this, &CreateExaminationView::handleTimeChanged);
}

void CreateExaminationView::setupSwitchToggle() {
    m_toggleLabel = new QLabel("Use a question timer");
    m_toggleLabel->setStyleSheet(
        "color: palette(windowText); "
        "font-size: 15px; "
    );
    m_toggleLabel->adjustSize();
    m_toggleLabel->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);

    m_toggle = new Switch(this);
    connect(m_toggle, &Switch::toggled, this, &CreateExaminationView::switchToggle);
}

void CreateExaminationView::setupGoBack() {
    m_goBack = new QPushButton("Go back");
    m_goBack->setIcon(QIcon::fromTheme("go-home"));
    m_goBack->setStyleSheet(
        "border: 0px;"
        "font-size: 25px;"
    );
    m_goBack->setCursor(Qt::PointingHandCursor);
    connect(m_goBack, &QPushButton::clicked, this, &CreateExaminationView::goBack);
}

void CreateExaminationView::setupLayouts(){
    m_switchLayout = new QVBoxLayout;
    m_switchLayout->addWidget(m_toggleLabel, 0, Qt::AlignHCenter);
    m_switchLayout->addWidget(m_toggle, 0, Qt::AlignHCenter);

    m_timerLayout = new QVBoxLayout;
    m_timerLayout->addWidget(m_timerLabel, 0, Qt::AlignHCenter);
    m_timerLayout->addWidget(m_timer, 0, Qt::AlignHCenter);
    m_timer->hide();                    // The toggle starts unchecked so hide it   
    m_timerLabel->hide();

    m_mainLayout = new QVBoxLayout;
    m_mainLayout->addWidget(m_goBack, 0, Qt::AlignLeft);
    m_mainLayout->addWidget(m_chooseQuestionView, 0, Qt::AlignHCenter);
    m_mainLayout->addLayout(m_switchLayout);
    m_mainLayout->addLayout(m_timerLayout);
    m_mainLayout->addWidget(m_startExaminationButton, 0, Qt::AlignHCenter);
}

// Make sure the user cant go below the minimum time (You cant use setMinimumTime for 10 seconds. It doesnt accept a 0 minute)
void CreateExaminationView::handleTimeChanged() {
    QTime currentTime = m_timer->time();

    int seconds = currentTime.second();
    int minutes = currentTime.minute();

    // Check if the current time is less than 10 seconds
    if (minutes == 0 && seconds < 10) {
        // If seconds is less than 10, reset to 10 seconds
        m_timer->setTime(QTime(0, 0, 10)); 
    }
}

void CreateExaminationView::switchToggle(){
    if (m_toggle->isChecked()) {
        m_timer->show();
        m_timerLabel->show();
    }
    else {
        m_timer->hide();
        m_timerLabel->hide();
    }
}

void CreateExaminationView::startExamination() {
    if (m_questionSelectOnly) {
        QString questionSetPath = m_chooseQuestionView->getQuestionSetPath();
        // No question set was selected, so show a toast message to alert the user
        if (questionSetPath == "") {
            ToastMessage* toast = new ToastMessage("You have to select a question set", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }

        m_examinationView = new ExaminationView(nullptr, m_questionSelectOnly);
        if (m_toggle->isChecked()) {
            QTime currentTime = m_timer->time();
            m_examinationView->startExamination(questionSetPath, currentTime);
        }
        else {
            m_examinationView->startExamination(questionSetPath, QTime(-1, -1, -1));
        }


        clearLayout(m_mainLayout);
        m_mainLayout->addWidget(m_examinationView);
        // If m_examinationView is closed also close this 
        connect(m_examinationView, &QWidget::destroyed, this, [=] {
            // Delete the remaining widgets
            //delete m_examinationView;
            delete m_mainLayout;
            this->close();
            });
    }

    else
    {
        QList <QString> questionSetPath = m_chooseQuestionView->getQuestionSetPaths();
        // No question set was selected, so show a toast message to alert the user
        if (questionSetPath.size() == 0) {
            ToastMessage* toast = new ToastMessage("You have to select a question set", this);
            toast->move((width() - toast->width()) / 2, height() - 85);
            toast->show();
            return;
        }

        m_examinationView = new ExaminationView(nullptr, m_questionSelectOnly);
        if (m_toggle->isChecked()) {
            QTime currentTime = m_timer->time();
            m_examinationView->startExamination(questionSetPath, currentTime);
        }
        else {
            m_examinationView->startExamination(questionSetPath, QTime(-1, -1, -1));
        }

        clearLayout(m_mainLayout);
        m_mainLayout->addWidget(m_examinationView);
        // If m_examinationView is closed also close this 
        connect(m_examinationView, &QWidget::destroyed, this, [=] {
            // Delete the remaining widgets
            //delete m_examinationView;
            delete m_mainLayout;
            this->close();
            });
    }

}

void CreateExaminationView::clearLayout(QLayout* layout) {
    // Iterate through the layout and remove all widgets and sub-layouts
    QLayoutItem* item;
    while ((item = layout->takeAt(0)) != nullptr) {
        // If the item is a widget, delete it
        if (item->widget()) {
            delete item->widget();
        }
        // If the item is a layout, recursively clear its contents and delete it
        else if (item->layout()) {
            clearLayout(item->layout());  // Recursively clear nested layouts
            delete item->layout();        // Delete the layout after clearing its contents
        }
    }
}
