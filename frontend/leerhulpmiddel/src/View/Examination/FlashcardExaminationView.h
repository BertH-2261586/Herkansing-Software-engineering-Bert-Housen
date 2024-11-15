#ifndef FLASHCARDEXAMINATIONWINDOW_H
#define FLASHCARDEXAMINATIONWINDOW_H

#include <QWidget>
#include <QLabel>
#include <QScrollArea>
#include <QEvent>
#include <QMouseEvent>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QTextEdit>

#include "../../model/Questions/Flashcard.h"

class FlashcardExaminationView : public QWidget {
    Q_OBJECT

public:
    FlashcardExaminationView(QWidget* parent = nullptr) {}
    void setQuestion(Flashcard* question);
    void clearPreviousQuestion();

private:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void setQuestionLabel();
    void setScrollArea();

    QLabel* m_questionLabel = nullptr;
    QScrollArea* m_scrollArea = nullptr;
    QVBoxLayout* m_mainQuestionLayout = nullptr;

    Flashcard* m_currentQuestion = nullptr;
    bool m_showingQuestion = true;
};

#endif