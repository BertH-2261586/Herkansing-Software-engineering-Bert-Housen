#include "scoreCardExaminationView.h"
#include "examinationView.h"

scoreCardExaminationView::scoreCardExaminationView(QWidget* parent, bool questionSelectOnly) : QWidget(parent) {
    m_questionSelectOnly = questionSelectOnly;
    // Create a border 
    initializeBorderWidget();

    // Initialize the labels (QLabels)
    initializeLabels();

    // Initialize the button (QPushButton)
    initializeCloseButton(parent);

    // Initialize the layouts
    initializeLayouts();

    setLayout(m_mainLayout);    
}

void scoreCardExaminationView::initializeBorderWidget() {
    m_borderWidget = new QWidget(this);
    m_borderWidget->setObjectName("borderWidget");
    m_borderWidget->setStyleSheet(
        "#borderWidget {"
        "   border: 2px solid palette(mid);"
        "   border-radius: 10px;"
        "   padding: 10px;" // Padding affects layout but not children styles
        "   background-color: palette(base);"
        "}"
    );
}

void scoreCardExaminationView::initializeLabels() {
    m_totalAnswers = createLabel();
    m_correctAnswers = createLabel();
    m_wrongAnswers = createLabel();
    m_percentage = createLabel();
    m_mostRetries = createLabel();
    m_totalTimeouts = createLabel();
    m_score = createLabel();
}

QLabel* scoreCardExaminationView::createLabel() {
    QLabel* label = new QLabel;
    label->setStyleSheet("color: palette(windowText); font-size: 25px;");
    return label;
}

void scoreCardExaminationView::initializeCloseButton(QWidget* parent) {
    m_closeButton = new QPushButton("Close score card", this);
    m_closeButton->setStyleSheet(
        "QPushButton {"
        "   color: palette(ButtonText);"
        "   background-color: palette(button);"
        "   border: 2px solid palette(mid);"
        "   border-radius: 5px;"
        "   padding: 8px 16px;"
        "   font-size: 16px;"
        "   max-width: 175px;"
        "   margin-top: 15px;"
        "}"
        "QPushButton:hover {"
        "   background-color: palette(mid);"
        "}"
    );
    m_closeButton->setCursor(Qt::PointingHandCursor);
    connect(m_closeButton, &QPushButton::clicked, static_cast<ExaminationView*>(parent), &ExaminationView::closeWindow);
}

void scoreCardExaminationView::initializeLayouts() {
    // Initialize main layout
    m_mainLayout = new QVBoxLayout;

    // Initialize the border layout
    m_borderLayout = new QVBoxLayout(m_borderWidget);
    m_borderLayout->setContentsMargins(35, 35, 35, 35);



    if (m_questionSelectOnly)
    {
        // Initialize answered question layout (horizontal)
        m_answeredQuestionLayout = new QHBoxLayout;
        m_answeredQuestionLayout->setSpacing(50);
        m_answeredQuestionLayout->addWidget(m_correctAnswers);
        m_answeredQuestionLayout->addWidget(m_wrongAnswers);
        m_answeredQuestionLayout->addWidget(m_percentage);

        // Add widgets and layouts to the border layout
        m_borderLayout->addWidget(m_totalAnswers);
        m_borderLayout->addLayout(m_answeredQuestionLayout);
        m_borderLayout->addWidget(m_mostRetries);
        m_borderLayout->addWidget(m_totalTimeouts);
    }
    else
    {
        m_borderLayout->addWidget(m_score);
    }

    m_borderLayout->addWidget(m_closeButton, 0, Qt::AlignHCenter);

    // Set the main layout
    m_mainLayout->addWidget(m_borderWidget);
}

void scoreCardExaminationView::showExaminationData(QMap<QString, QString> examinationData) {
    m_totalAnswers->setText("Total questions answered: " + examinationData.value("total_answers"));
    m_correctAnswers->setText("Correct answers: " + examinationData.value("right_answers"));
    m_wrongAnswers->setText("Wrong answers: " + examinationData.value("total_wrong_answers"));
    m_percentage->setText("Accuracy: " + examinationData.value("percentage") + "%");
    m_mostRetries->setText("Highest repeated question amount: " + examinationData.value("most_retries"));
    m_totalTimeouts->setText("Amount of questions timed out: " + examinationData.value("total_timeouts"));
    m_score->setText("Score: " + QString::number(examinationData.value("right_answers").toInt() - examinationData.value("total_wrong_answers").toInt()) + "/" + examinationData.value("total_answers"));
}