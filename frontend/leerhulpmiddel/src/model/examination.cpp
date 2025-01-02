#include "examination.h"
#include <random>

void Examination::setQuestions(QVector<shared_ptr<Question>> questions) {
    randomizeQuestions(questions);
    m_questions = questions;
    m_totalSize = questions.size();
    m_amountOfQuestions = questions.size();
}

void Examination::randomizeQuestions(QVector<shared_ptr<Question>>& questions) {
    // Create a random number generator and shuffle the questions
    std::random_device rd;  // Random device to get a seed
    std::mt19937 gen(rd());  // Initialize Mersenne Twister generator with the random seed

    std::shuffle(questions.begin(), questions.end(), gen);
}

bool Examination::nextQuestion(bool showWrong) {
    ++m_currentQuestionIndex;
    m_showWrong = showWrong;
    // All questions have been answered, show the wrong questions now
    if (m_currentQuestionIndex + 1 > m_questions.size() && showWrong) {         // Index starts at 0 and size at 1 so compensate the difference with +1
        m_currentQuestionIndex = 0;
        randomizeQuestions(m_wrongQuestions);
        m_questions = m_wrongQuestions;
        m_totalSize += m_wrongQuestions.size();
        ++m_mostRetries;
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

void Examination::addWrongCurrentQuestion(bool timeout) { 
	m_wrongQuestions.append(m_questions[m_currentQuestionIndex]); 
	++m_totalWrongAnswers;
    if (timeout) {
        ++m_totalTimeouts;
    }
}

bool Examination::finishedExamination() const {
    if (m_currentQuestionIndex + 1 == m_questions.size() && (m_wrongQuestions.isEmpty() || !m_showWrong)) {        // Index starts at 0 and size at 1 so compensate the difference with +1
        return true;
    }
    return false;
}

QMap<QString, QString> Examination::getExaminationData() const {
    QMap<QString, QString> data;
    data.insert("total_answers", QString::number(m_totalSize));
    data.insert("right_answers", QString::number(m_amountOfQuestions));
    data.insert("total_wrong_answers", QString::number(m_totalWrongAnswers));
    data.insert("total_timeouts", QString::number(m_totalTimeouts));
    data.insert("most_retries", QString::number(m_mostRetries));

    int percentage = (static_cast<double>(m_amountOfQuestions) / static_cast<double>(m_totalSize)) * 100;       // Bereken de percentage (in double anders geen kommagetal)
    data.insert("percentage", QString::number(percentage));

    return data;
}
