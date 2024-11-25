#include "examinationController.h"
#include "../view/Examination/examinationView.h"

// Connect all the signals and slots between the manager, controller and the view
ExaminationController::ExaminationController(ExaminationView* examinationView) : m_examinationView(examinationView), m_currentQuestionIndex(1), m_examinationStatus(false) {
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
	// Notify the manager that the user has answered the question wrong 
	connect(this, &ExaminationController::answeredWrong, m_examinationManager, &ExaminationManager::answeredWrong);
	// Notify the controller that all questions have been answered and that wrongly answered questions get loaded
	connect(m_examinationManager, &ExaminationManager::wrongQuestionsLoadedModel, this, &ExaminationController::wrongQuestionsLoadedModel);
	// Notify the view that all questions have been answered and that wrongly answered questions get loaded. 
	// You can use questionLoadedView slot because it has the same execution
	connect(this, &ExaminationController::wrongQuestionsLoadedView, m_examinationView, &ExaminationView::questionLoadedView);
}

// The manager notified the controller that the first question has loaded
void ExaminationController::firstQuestion(shared_ptr<Question> question, int totalAmountOfQuestions) {
	// Initialize the variables
    m_currentQuestion = question;
    m_totalAmountOfQuestions = totalAmountOfQuestions;
	// Notify the view that the first question has been loaded
    emit questionLoadedView();		// TODO check if size of examination is one 
}

// The manager notified the controller that the next question has loaded
void ExaminationController::questionLoadedModel(shared_ptr<Question> question) {
	// Initialize the variables
    m_currentQuestion = question;
	++m_currentQuestionIndex;
	// Notify the view that the next question has been loaded
    emit questionLoadedView();
}

void ExaminationController::wrongQuestionsLoadedModel(shared_ptr<Question> question, int totalSize) {
	m_currentQuestion = question;
	m_totalAmountOfQuestions = totalSize;
	++m_currentQuestionIndex;
	emit wrongQuestionsLoadedView();		// TODO check if only one is wrong
}

// Check if the user answered the multiple choice correctly and return the correct one if not
int ExaminationController::checkMultipleChoiceAnswer(QString checkedAnswers) {
	// Get the correct answer for the question
	QList<QString> correctAnswers = m_currentQuestion->getAnswer().getCorrectAnswers();
	// The question was answered wrong				There was no answer selected
	if (!correctAnswers.contains(checkedAnswers) || checkedAnswers == "") {
		emit answeredWrong();
		QList<int> correctAnswersIndices = m_currentQuestion->getAnswer().getIndicesCorrectAnswers();
		return correctAnswersIndices[0];		// Only return the first one since multiple choice can only have one correct answer
	}

	// The user answered correctly 
	return -1;
}

// Check if the user answered the fill in correctly
QVector<int> ExaminationController::checkFillInAnswer(QVector<QString> answerText) {
	QVector<int> wrongAnswers;
	bool alreadySignalled = false;
	QList<QString> correctAnswers = m_currentQuestion->getAnswer().getAnswers();

	for (int i = 0; i < answerText.size(); ++i) {
		QString userAnswer = answerText[i].toLower(); // Get the user input
		QString correctAnswer = correctAnswers[i].trimmed().toLower();     // Get the corresponding correct answer

		if (userAnswer != correctAnswer) {
			// Dont send the same signal multiple times
			if (!alreadySignalled) {
				emit answeredWrong();
			}

			wrongAnswers.append(i);
			alreadySignalled = true;		 // Set the bool to true so that you dont sent multiple signals to the manager	
		}
	}

	return wrongAnswers;
}