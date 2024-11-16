#include "examinationController.h"
#include "../view/Examination/examinationView.h"

ExaminationController::ExaminationController(ExaminationView* examinationView) : m_examinationView(examinationView), m_currentQuestionIndex(1) {
	connect(m_examinationView, &ExaminationView::examinationStarted, m_examinationManager, &ExaminationManager::examinationStarted);

	connect(m_examinationManager, &ExaminationManager::firstQuestion, this, &ExaminationController::firstQuestion);

	connect(this, &ExaminationController::questionLoadedView, m_examinationView, &ExaminationView::questionLoadedView);

	connect(m_examinationView, &ExaminationView::nextQuestion, m_examinationManager, &ExaminationManager::nextQuestion);

	connect(m_examinationManager, &ExaminationManager::questionLoadedModel, this, &ExaminationController::questionLoadedModel);
}

void ExaminationController::firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions) {
    m_currentQuestion = question;
    m_totalAmountOfQuestions = totalAmountOfQuestions;
    emit questionLoadedView();
}

void ExaminationController::questionLoadedModel(shared_ptr<Question> question) {
    m_currentQuestion = question;
	++m_currentQuestionIndex;
    emit questionLoadedView();
}