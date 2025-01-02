#include "CreateFlashcardView.h"
#include "../../Exceptions/EmptyFieldException.h"


#include <QLabel>
#include <QVBoxLayout>

CreateFlashcardView::CreateFlashcardView(QWidget* parent) : QWidget(parent) {
	QLabel* lblQuestion = new QLabel(this);
	lblQuestion->setText("Question: ");

	QLabel* lblAnswer = new QLabel(this);
	lblAnswer->setText("Answer: ");

	m_txtQuestion = new QTextEdit(this);
	m_txtAnswer = new QTextEdit(this);
	
	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	mainLayout->addWidget(lblQuestion);
	mainLayout->addWidget(m_txtQuestion);
	mainLayout->addWidget(lblAnswer);
	mainLayout->addWidget(m_txtAnswer);

	setLayout(mainLayout);
}

QString CreateFlashcardView::getQuestion() const {
	if (m_txtQuestion->toPlainText().isEmpty())
	{
		throw EmptyFieldException("Please fill in the question");
	}

	return m_txtQuestion->toPlainText();
}

 Answer CreateFlashcardView::getAnswer() const {
	 if (m_txtAnswer->toPlainText().isEmpty())
	 {
		 throw EmptyFieldException("Please fill in the answer");
	 }
	 Answer answer = Answer({ m_txtAnswer->toPlainText() });

	 return answer;
}
