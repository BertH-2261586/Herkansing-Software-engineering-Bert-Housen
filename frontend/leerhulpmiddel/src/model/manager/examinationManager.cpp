#include "examinationManager.h"
#include "../fileManager.h"

void ExaminationManager::examinationStarted(const QString questionSetPath) {
	FileManager fileManager;
	QVector<shared_ptr<Question>> questions = fileManager.getAllQuestionsFromQuestionSet(questionSetPath);
	m_examination.setQuestions(questions);

	emit firstQuestion(questions[0], questions.size());
}

void ExaminationManager::nextQuestion() {
	m_examination.nextQuestion();
	emit questionLoadedModel(m_examination.getCurrentQuestion());
}