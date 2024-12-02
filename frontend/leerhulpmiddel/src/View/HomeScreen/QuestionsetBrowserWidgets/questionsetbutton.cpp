
#include "questionsetbutton.h"
#include "../../create questions/CreateQuestionView.h"
#include <QMenu>

QuestionsetButton::QuestionsetButton(Questionset* questionset, int indentation, QuestionsetTreeWidget* parent) :
    QWidget(parent), m_questionset(questionset), m_controller(questionset), m_parent(parent)
{
    QHBoxLayout* container = new QHBoxLayout(this);
    container->setContentsMargins(0, 0, 0, 0);
    container->setSpacing(0);

    m_label = new EditableLabel(m_questionset->GetName());

    connect(m_questionset, &Questionset::changed, this, &QuestionsetButton::update);     //hier wordt update() gebruikt in case dat er gedacht werdt dat update() redundant was

    connect(m_label, &EditableLabel::textChanged, this, [=](QString text){
        m_controller.changeName(text);
    });

    m_label->setStyleSheet(QString(                            //de styling van een subdeel, werk met padding zodat de button background er goed uitziet
                               "   border: none;"
                               "   border-radius: 0px;"
                               "   padding: 5px 0px 5px 0px;"
                               "   text-align: left;"
                               ""
                               ));
    m_label->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Maximum);

    QWidget* indentationWidget = new QWidget();
    indentationWidget->setFixedWidth((m_indentationOfset * indentation) + m_baseOfset);
    m_label->setStyleSheet(QString(                            //de styling van een subdeel, werk met padding zodat de button background er goed uitziet
                               "   border: 0px;"
                               "   border-radius: 0px;"
                               "   padding: 5px 0px 5px 0px;"
                               "   text-align: left;"
                               ""
                               ));

    container->addWidget(indentationWidget);
    container->addWidget(m_label);
    container->addWidget(GenerateMenuButton());

}


void QuestionsetButton::update()
{
    m_label->setText(m_questionset->GetName());
}


//Gaat de knop voor het menu te tonen waar je een vraag of subset ergens aan kan toevoegen
QPushButton* QuestionsetButton::GenerateMenuButton()
{
    QPushButton* outputButton = new QPushButton();           //hier een plus icoon aan geven en styling doen
    outputButton->setIcon(outputButton->style()->standardIcon(QStyle::SP_FileDialogNewFolder));
    outputButton->setStyleSheet(
        "QPushButton { "
        "   color: #000000;"
        //"   background-color: #AAAAAA;"
        "   border: none; "
        "   border-radius: 0px;"
        "   padding: 0px 0px 0px 0px;"
        "   text-align: center;"
        "}"
        );
    outputButton->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Minimum);
    outputButton->setFixedWidth(20);

    QMenu* menu = new QMenu(this);
    QAction* addSubsetAction = menu->addAction("Voeg subfolder toe");      //TODO deze strings aanpassen naar wat gepast is
    QAction* addQuestionAction = menu->addAction("Voeg vraag toe");
    menu->setStyleSheet("background-color: #AFAFAF; color: #000000;");

    connect(outputButton, &QPushButton::clicked, menu, [=]{
        menu->popup(outputButton->mapToGlobal(QPoint(0, outputButton->height())));
    });

    connect(addSubsetAction, &QAction::triggered, m_parent, &QuestionsetTreeWidget::CreateNewQuestionset);
    connect(addQuestionAction, &QAction::triggered, this, [=] {
        m_parent->sendDisplayQuestionSignal(new CreateQuestionView(QuestionsetController(m_questionset), this));
    });

    return outputButton;
}
