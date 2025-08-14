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

    QVBoxLayout* m_mainQuestionLayout = nullptr;
    QVBoxLayout* m_toggleLayout = nullptr;

    QLabel* m_questionLabel = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QLabel* m_toggleLabel = nullptr;
    Switch* m_toggleSwitch = nullptr;

    const Flashcard* m_currentQuestion;
    bool m_showingQuestion = true;
};

#endif
