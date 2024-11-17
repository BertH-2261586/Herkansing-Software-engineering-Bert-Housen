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
	emit firstQuestion(questions[0], questions.size());
}

// The manager has been notified that the user wants to go the next question
void ExaminationManager::nextQuestion() {
	// Go to the next question and notify the view that the next question has been loaded
	m_examination.nextQuestion();
	emit questionLoadedModel(m_examination.getCurrentQuestion());
}