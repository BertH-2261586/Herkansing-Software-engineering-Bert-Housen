#include "CreateMultipleChoiceQuestionView.h"
#include "../ToastMessage.h"

#include <QPushButton>
#include <QLabel>
#include <QVBoxLayout>
#include <QIcon>
#include <QScrollArea>
#include <QMessageBox>
#include <QStringList>

CreateMultipleChoiceQuestionView::CreateMultipleChoiceQuestionView(QWidget* parent) : QWidget(parent), m_txtQuestion{ new QTextEdit(this)} {


	QLabel* lblQuestion = new QLabel(this);
	lblQuestion->setText("Question: ");

	QPushButton* btnAddAnswer = new QPushButton("Add an Answer",this);

	connect(btnAddAnswer, &QPushButton::clicked, this, &CreateMultipleChoiceQuestionView::addAnswer);


	m_layout = new QVBoxLayout(this);
	


	QWidget* contentWidget = new QWidget(this);
	contentWidget->setLayout(m_layout);


	QScrollArea* scrollArea = new QScrollArea(this);
	scrollArea->setWidget(contentWidget);  // Set the scrollable content widget
	scrollArea->setWidgetResizable(true);
	scrollArea->setFrameShape(QFrame::NoFrame);  // No frame around the scroll area
	scrollArea->setFrameShadow(QFrame::Plain);

	QVBoxLayout* outerLayout = new QVBoxLayout(this);
	outerLayout->addWidget(lblQuestion);
	outerLayout->addWidget(m_txtQuestion);
	outerLayout->addWidget(btnAddAnswer);
	outerLayout->addWidget(scrollArea);
	setLayout(outerLayout);
}

QString CreateMultipleChoiceQuestionView::getQuestion() const {
	return m_txtQuestion->toPlainText();
}

Answer CreateMultipleChoiceQuestionView::getAnswer() const {
	return Answer({});
}



void CreateMultipleChoiceQuestionView::addAnswer() {

	if (m_layout->count() > 15)
	{
		ToastMessage* toast = new ToastMessage("You can only add 15 answers", this);
		toast->setFixedWidth(200);  // Adjust width as needed
		toast->move((width() - toast->width()) / 2, height() - 50);  // Position near the bottom center
		toast->show();
	}
	else
	{

		QHBoxLayout* answerLayout = new QHBoxLayout;

		QTextEdit* txtAnswer = new QTextEdit(this);
		txtAnswer->setPlaceholderText("Enter an answer");
		txtAnswer->setFixedHeight(50);


		QPushButton* removeButton = new QPushButton(this);

		removeButton->setIcon(QIcon::fromTheme(QIcon::ThemeIcon::ListRemove));


		answerLayout->addWidget(txtAnswer);
		answerLayout->addWidget(removeButton);


		m_layout->addLayout(answerLayout);

		connect(removeButton, &QPushButton::clicked, [this, answerLayout]() {
			m_layout->removeItem(answerLayout);
			QLayoutItem* child;
			while ((child = answerLayout->takeAt(0)) != nullptr) {
				delete child->widget(); // delete the widget
				delete child;   // delete the layout item
			}
			delete answerLayout;
			});

		update();
	}
}
