#include "scoreCardExaminationView.h"

scoreCardExaminationView::scoreCardExaminationView(int correctAnswers, int wrongAnswers, QWidget* parent) : QWidget(parent) {
	m_mainLayout = new QVBoxLayout;

	m_scoreCardBoxLayout = new QVBoxLayout;
	m_correctAnswers = new QLabel(QString::number(correctAnswers));
	m_wrongAnswers = new QLabel(QString::number(wrongAnswers));
    //m_endExaminationButton = new QPushButton("End the examination", this);
    //m_endExaminationButton->setStyleSheet(
    //    "QPushButton {"
    //    "   color: palette(ButtonText); "
    //    "   background-color: palette(button);"             // Background color (green)
    //    "   border: 2px solid palette(mid);"             // Border with a darker green
    //    "   border-radius: 5px;"                    // Rounded corners
    //    "   padding: 8px 16px;"                     // Padding for a better button shape
    //    "   margin-bottom: 25px;"
    //    "   font-size: 16px;"                       // Font size
    //    "   max-width: 175px;"                      // Limit the width of the button
    //    "}"
    //    "QPushButton:hover {"
    //    "   background-color: palette(mid);"             // Darker green on hover
    //    "}"
    //);
    //m_endExaminationButton->setCursor(Qt::PointingHandCursor);
    //connect(m_endExaminationButton, &QPushButton::clicked, this, [this]() {
    //    closeWindow();  
    //});

	m_scoreCardBoxLayout->addWidget(m_correctAnswers);
	m_scoreCardBoxLayout->addWidget(m_wrongAnswers);
    m_scoreCardBoxLayout->addWidget(m_closeButton);

	m_mainLayout->addLayout(m_scoreCardBoxLayout);
}