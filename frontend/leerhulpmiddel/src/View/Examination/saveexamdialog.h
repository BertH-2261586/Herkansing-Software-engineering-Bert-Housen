
#ifndef SAVEEXAMDIALOG_H
#define SAVEEXAMDIALOG_H


#include <QObject>
#include <QDialog>

#include "../toggleSwitch.h"
#include "../chooseQuestionView.h"
#include "../../model/manager/NetworkManager.h"

class SaveExamDialog : public QDialog
{
    Q_OBJECT
public:
    SaveExamDialog();


private:
    void setupTimerLabel();
    void setupTimer();
    void setupSwitchToggle();

    Switch* m_toggle;
    ChooseQuestionView* m_chooseQuestionView;
    QTimeEdit* m_timer;
    QLabel* m_timerLabel;
    QLabel* m_toggleLabel;

    QLineEdit* m_nameField;


private slots:
    void switchToggle();
    void handleTimeChanged();

};

#endif // SAVEEXAMDIALOG_H
