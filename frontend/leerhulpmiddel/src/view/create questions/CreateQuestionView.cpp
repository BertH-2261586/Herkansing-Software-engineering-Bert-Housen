#include "CreateQuestionView.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

#include "../../model/Questions/Flashcard.h"
#include "../../model/Questions/MultipleChoiceQuestion.h"
#include "../../model/Questions/FillInQuestion.h"

CreateQuestionView::CreateQuestionView(QuestionManagerController* questionController, QWidget* parent) 
	: m_questionController(questionController), QWidget(parent) {
	setWindowFlags(Qt::Popup);
	setAttribute(Qt::WA_DeleteOnClose);

	QLabel* lblQuestionName = new QLabel(this);
	lblQuestionName->setText("Question name: ");

	m_txtQuestionName = new QLineEdit(this);

	QLabel* lblQuestionType = new QLabel(this);
	lblQuestionType->setText("Question type: ");

	m_cboQuestionType = new QComboBox(this);
	m_cboQuestionType->addItems({"Flashcard", "Multiple Choice Question", "Fill In Question"});

	QPushButton* btnAddQuestion = new QPushButton(this);
	btnAddQuestion->setText("Add Question");

	m_createFlashcardView = new CreateFlashcardView(this);
	m_createMultipleChoiceQuestionView = new CreateMultipleChoiceQuestionView(this);
	m_createFillInQuestionView = new CreateFillInQuestionView(this);

	m_questionSelection = new QStackedWidget(this);
	m_questionSelection->addWidget(m_createFlashcardView);
	m_questionSelection->addWidget(m_createMultipleChoiceQuestionView);
	m_questionSelection->addWidget(m_createFillInQuestionView);

	connect(m_cboQuestionType, &QComboBox::activated,
		m_questionSelection, &QStackedWidget::setCurrentIndex);

	connect(btnAddQuestion, &QPushButton::pressed,
		this, &CreateQuestionView::confirm);

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(lblQuestionName);
	mainLayout->addWidget(m_txtQuestionName);
	mainLayout->addWidget(lblQuestionType);
	mainLayout->addWidget(m_cboQuestionType);
	mainLayout->addWidget(m_questionSelection);
	mainLayout->addWidget(btnAddQuestion);

	setLayout(mainLayout);
}

void CreateQuestionView::confirm() {
	QMessageBox msgBox;

	switch (m_questionSelection->currentIndex()) {
	case 0: {
		msgBox.setText("Created Flashcard");
		//Create flashcard object
		Flashcard* question = new Flashcard(m_txtQuestionName->text(), m_createFlashcardView->getQuestion(), m_createFlashcardView->getAnswer());

		m_questionController->addQuestion("questionset", "", question);
		break;
	}
	case 1: {
		msgBox.setText("Created Multiple Choice Question");
		//Create multipleChoiceQuestion object
		MultipleChoiceQuestion* question = new MultipleChoiceQuestion(m_txtQuestionName->text(), m_createMultipleChoiceQuestionView->getQuestion(), m_createMultipleChoiceQuestionView->getAnswer());

		m_questionController->addQuestion("questionset", "", question);
		break;
	}
	case 2: {
		msgBox.setText("Created Fill In Question");
		//Create FillInQuestion object
		FillInQuestion* question = new FillInQuestion(m_txtQuestionName->text(), m_createFillInQuestionView->getQuestion(), m_createFillInQuestionView->getAnswer());

		m_questionController->addQuestion("questionset", "", question);
		break;
	}
	default: {
		msgBox.setText("Something went wrong, please close and try again!");
		break;
	}
	}

	msgBox.exec();
}
