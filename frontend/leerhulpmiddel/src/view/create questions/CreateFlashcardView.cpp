#include "CreateFlashcardView.h"

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
