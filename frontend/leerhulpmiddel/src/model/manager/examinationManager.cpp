#include "examinationManager.h"
#include "../fileManager.h"

// The manager has been notified that the user has started an examination
void ExaminationManager::examinationStarted(const QString questionSetPath) {
	// Get all the questions from the path
	FileManager fileManager;
	QVector<shared_ptr<Question>> questions = fileManager.getAllQuestionsFromQuestionSet(questionSetPath);  

	// Initialize the questions inside the examination class
	m_examination.setQuestions(questions);

	// Notify the view that the questions have been loaded
	emit firstQuestion(m_examination.getCurrentQuestion(), m_examination.getTotalSize());
}

// The manager has been notified that the user has started an examination
void ExaminationManager::examinationStartedL(const QList<QString> questionSetPath) {
	// Get all the questions from the path
	FileManager fileManager;
	QVector<shared_ptr<Question>> questions = fileManager.getAllQuestionsFromQuestionSetMulti(questionSetPath);

	// Initialize the questions inside the examination class
	m_examination.setQuestions(questions);

	// Notify the view that the questions have been loaded
	emit firstQuestion(m_examination.getCurrentQuestion(), m_examination.getTotalSize());
}

void ExaminationManager::examinationStartedQ(const QVector<shared_ptr<Question>> questions) {

    // Initialize the questions inside the examination class
    m_examination.setQuestions(questions);

    // Notify the view that the questions have been loaded
    emit firstQuestion(m_examination.getCurrentQuestion(), m_examination.getTotalSize());
}

// The manager has been notified that the user wants to go the next question
void ExaminationManager::nextQuestion(bool showWrong) {
	// Go to the next question and notify the view that the next question has been loaded
	bool retryWrongAnswers = m_examination.nextQuestion(showWrong);
	if (!retryWrongAnswers) {
		emit questionLoadedModel(m_examination.getCurrentQuestion());
	}
	else {
		emit wrongQuestionsLoadedModel(m_examination.getCurrentQuestion(), m_examination.getTotalSize());
	}
}

void ExaminationManager::getExaminationData() {
	QMap<QString, QString> data = m_examination.getExaminationData();
	emit sendExaminationData(data);
}
