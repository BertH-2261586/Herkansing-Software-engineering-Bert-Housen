#include "examinationController.h"
#include "../view/Examination/examinationView.h"

// Connect all the signals and slots between the manager, controller and the view
ExaminationController::ExaminationController(ExaminationView* examinationView) : m_examinationView(examinationView), m_currentQuestionIndex(1) {
	// Notify the manager that the user started an examination
	connect(m_examinationView, &ExaminationView::examinationStarted, m_examinationManager, &ExaminationManager::examinationStarted);
	// Notify the controller that the first question has been loaded
	connect(m_examinationManager, &ExaminationManager::firstQuestion, this, &ExaminationController::firstQuestion);
	// Notify the view that the current question is loaded
	connect(this, &ExaminationController::questionLoadedView, m_examinationView, &ExaminationView::questionLoadedView);
	// Notify the controller that the current question is loaded in the model
	connect(m_examinationManager, &ExaminationManager::questionLoadedModel, this, &ExaminationController::questionLoadedModel);
	// Notify the manager that the user wants to go to the next question
	connect(m_examinationView, &ExaminationView::nextQuestion, m_examinationManager, &ExaminationManager::nextQuestion);
}

// The manager notified the controller that the first question has loaded
void ExaminationController::firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions) {
	// Initialize the variables
    m_currentQuestion = question;
    m_totalAmountOfQuestions = totalAmountOfQuestions;
	// Notify the view that the first question has been loaded
    emit questionLoadedView();
}

// The manager notified the controller that the next question has loaded
void ExaminationController::questionLoadedModel(shared_ptr<Question> question) {
	// Initialize the variables
    m_currentQuestion = question;
	++m_currentQuestionIndex;
	// Notify the view that the next question has been loaded
    emit questionLoadedView();
}