#ifndef CREATEEXAMINATIONVIEW_H
#define CREATEEXAMINATIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTimeEdit>
#include <QLabel>

#include "examinationView.h"
#include "../toggleSwitch.h"
#include "../chooseQuestionView.h"

class CreateExaminationView : public QWidget {
    Q_OBJECT

public:
    CreateExaminationView(QWidget* parent = nullptr);

private slots:
    void handleTimeChanged();
    void switchToggle();
    void goBack() { this->close(); }

private:
    void setupExaminationButton();
    void setupTimerLabel();
    void setupTimer();
    void setupSwitchToggle();
    void setupGoBack();

    void setupLayouts();

    void startExamination();

    void clearLayout(QLayout* layout);

    ExaminationView* m_examinationView;

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_timerLayout;
    QVBoxLayout* m_switchLayout;

    QPushButton* m_goBack;
    ChooseQuestionView* m_chooseQuestionView;
    QLabel* m_timerLabel;
    QTimeEdit* m_timer;
    Switch* m_toggle;
    QLabel* m_toggleLabel;
    QPushButton* m_startExaminationButton;
};

#endif
