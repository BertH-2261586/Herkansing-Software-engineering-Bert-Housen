#include "Flashcard.h"

Flashcard::Flashcard(string name, string question, string answer) : Question(name), m_question(question), m_answer(answer) {}

string Flashcard::getQuestion() const {
    return m_question;
}

string Flashcard::getAnswer() const {
    return m_answer;
}
