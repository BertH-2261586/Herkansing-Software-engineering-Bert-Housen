#include "examination.h"
#include <random>

void Examination::setQuestions(QVector<shared_ptr<Question>> questions) {
    randomizeQuestions(questions);
    m_questions = questions;
    m_totalSize = questions.size();
}

void Examination::randomizeQuestions(QVector<shared_ptr<Question>>& questions) {
    // Create a random number generator and shuffle the questions
    std::random_device rd;  // Random device to get a seed
    std::mt19937 gen(rd());  // Initialize Mersenne Twister generator with the random seed

    std::shuffle(questions.begin(), questions.end(), gen);
}

bool Examination::nextQuestion() {
    ++m_currentQuestionIndex;

    // All questions have been answered, show the wrong questions now
    if (m_currentQuestionIndex + 1 > m_questions.size()) {        // Index starts at 0 and size at 1 so compensate the difference with +1
        m_currentQuestionIndex = 0;
        randomizeQuestions(m_wrongQuestions);
        m_questions = m_wrongQuestions;
        m_totalSize += m_wrongQuestions.size();
        // All questions have been answered and there are no wrongly answered questions left
        // Index starts at 0 and size at 1 so compensate the difference with +1
        if (m_wrongQuestions.isEmpty()) {
            m_examinationDone = true;
        }

        m_wrongQuestions.clear();

        return true;
    }

    return false;
}

bool Examination::finishedExamination() const {
    if (m_currentQuestionIndex + 1 == m_questions.size() && m_wrongQuestions.isEmpty()) {        // Index starts at 0 and size at 1 so compensate the difference with +1
        return true;
    }

    return false;
}

void Examination::addWrongCurrentQuestion() { 
	m_wrongQuestions.append(m_questions[m_currentQuestionIndex]); 
	++m_totalWrongAnswers;
}