#pragma once

#include "Question.h"

#include <QString>


class FillInQuestion : public Question {
public:
    FillInQuestion(QString name, QString question, Answer answer) : Question(name, QuestionType::FillIn), m_question(question), m_answer(answer) {}

    // Getters
    QString getQuestion() const override { return m_question; }
    Answer getAnswer() const override { return m_answer; }

private:
    QString m_question;
    Answer m_answer;
};