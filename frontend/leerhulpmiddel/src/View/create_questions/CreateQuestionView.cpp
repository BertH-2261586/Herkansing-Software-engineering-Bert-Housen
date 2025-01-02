#include "CreateQuestionView.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>
#include <QLabel>

#include "../../model/Questions/Flashcard.h"
#include "../../model/Questions/MultipleChoiceQuestion.h"
#include "../../model/Questions/FillInQuestion.h"
#include "../../Exceptions/EmptyFieldException.h"
#include "../ToastMessage.h"

CreateQuestionView::CreateQuestionView(QuestionsetController questionsetController, QWidget* parent)
    : m_questionsetController(questionsetController), QWidget(parent) {
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
	try {
		if (m_txtQuestionName->text().isEmpty())
		{
			throw EmptyFieldException("Please fill in the question name");
		}

		switch (m_questionSelection->currentIndex()) {
			case 0: {
				showToastMessage("Created Flashcard Question");
				//Create flashcard object
				Flashcard* question = new Flashcard(m_txtQuestionName->text(), m_createFlashcardView->getQuestion(), m_createFlashcardView->getAnswer());

				m_questionsetController.addQuestion(question);
				break;
			}
			case 1: {
				showToastMessage("Created Multiple Choice Question");
				//Create multipleChoiceQuestion object
				MultipleChoiceQuestion* question = new MultipleChoiceQuestion(m_txtQuestionName->text(), m_createMultipleChoiceQuestionView->getQuestion(), m_createMultipleChoiceQuestionView->getAnswer());

				m_questionsetController.addQuestion(question);
				break;
			}
			case 2: {
				showToastMessage("Created Fill In Question");
				//Create FillInQuestion object
				FillInQuestion* question = new FillInQuestion(m_txtQuestionName->text(), m_createFillInQuestionView->getQuestion(), m_createFillInQuestionView->getAnswer());

				m_questionsetController.addQuestion(question);
				break;
			}
			default: {
				showToastMessage("Something went wrong, please close and try again!");
				break;
			}
		}
	}
	catch (const EmptyFieldException &e) {
		showToastMessage(e.what());
	}
	catch (const std::exception &e) {
		showToastMessage(e.what());
	}
}

void CreateQuestionView::showToastMessage(const QString textToDisplay) {
	ToastMessage* toast = new ToastMessage(textToDisplay, this);
	toast->move((width() - toast->width()) / 2, height() - 70);
	toast->show();
}
