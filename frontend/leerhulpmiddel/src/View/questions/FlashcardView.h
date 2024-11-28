#ifndef FLASHCARDWINDOW_H
#define FLASHCARDWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTextEdit>

#include "../../model/Questions/Flashcard.h"
#include "../toggleSwitch.h"

class FlashcardView : public QWidget {
    Q_OBJECT

signals:
    void flashcardHasBeenFlipped();

public:
    FlashcardView(QWidget* parent = nullptr) {}
    void setQuestion(const Flashcard* question);
    void clearPreviousQuestion();
    void handleQuestionClicked();
    bool getToggleStatus() { return m_toggleSwitch->isChecked(); }

private:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void setQuestionLabel();
    void setScrollArea();
    void setupToggleSwitch();
    
    void initializeLayouts();

    QVBoxLayout* m_mainQuestionLayout;
    QVBoxLayout* m_toggleLayout;

    QLabel* m_questionLabel;
    QScrollArea* m_scrollArea;
    QLabel* m_toggleLabel;
    Switch* m_toggleSwitch;

    const Flashcard* m_currentQuestion;
    bool m_showingQuestion = true;
};

#endif