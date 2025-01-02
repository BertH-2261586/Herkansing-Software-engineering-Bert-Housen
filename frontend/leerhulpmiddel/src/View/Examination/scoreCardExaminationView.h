#ifndef SCORECARDEXAMINARTIONVIEW_H
#define SCORECARDEXAMINARTIONVIEW_H

#include <QWidget>
#include <QVBoxLayout>
#include <QLabel>
#include <QPushButton>

class scoreCardExaminationView : public QWidget {
    Q_OBJECT

public:
    scoreCardExaminationView(QWidget* parent = nullptr, bool questionSelectOnly = true);
    ~scoreCardExaminationView() {}
    void showExaminationData(QMap<QString, QString> examinationData);

private:
    void initializeBorderWidget();
    void initializeLabels();
    QLabel* createLabel();
    void initializeCloseButton(QWidget* parent);
    void initializeLayouts();

    QVBoxLayout* m_mainLayout;
    QVBoxLayout* m_borderLayout;
    QHBoxLayout* m_answeredQuestionLayout;

    QWidget* m_borderWidget;
    QLabel* m_totalAnswers;
    QLabel* m_correctAnswers;
    QLabel* m_wrongAnswers;
    QLabel* m_percentage;
    QLabel* m_mostRetries;
    QLabel* m_totalTimeouts;
    QLabel* m_score;

    bool m_questionSelectOnly;

    QPushButton* m_closeButton;
};

#endif