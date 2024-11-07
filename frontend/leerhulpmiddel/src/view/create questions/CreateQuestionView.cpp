#include "CreateQuestionView.h"

#include <QVBoxLayout>
#include <QPushButton>
#include <QMessageBox>

CreateQuestionView::CreateQuestionView(QWidget* parent) : QWidget(parent) {
	setWindowFlags(Qt::Popup);
	setAttribute(Qt::WA_DeleteOnClose);

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

	mainLayout->addWidget(m_cboQuestionType);
	mainLayout->addWidget(m_questionSelection);
	mainLayout->addWidget(btnAddQuestion);

	setLayout(mainLayout);
}

void CreateQuestionView::confirm() {
	QMessageBox msgBox;

	switch (m_questionSelection->currentIndex()) {
	case 0:
		msgBox.setText("Created Flashcard");
		break;
	case 1:
		msgBox.setText("Created Multiple Choice Question");
		break;
	case 2:
		msgBox.setText("Created Fill In Question");
		break;
	default:
		msgBox.setText("Something went wrong, please close and try again!");
		break;
	}

	msgBox.exec();
}
