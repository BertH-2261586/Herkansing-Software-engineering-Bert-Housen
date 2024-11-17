#include <QApplication>
#include <QMainWindow>

#include "model/fileManager.h"
#include "model/Questions/MultipleChoiceQuestion.h"
#include "view/create questions/CreateQuestionView.h"
#include "model/questionset.h"
#include "Controller/questionmanagercontroller.h"
#include "model/questionmanager.h"
#include "view/leerhulpmiddelmainwindow.h"


int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QuestionManager qManager;

    LeerhulpmiddelMainWindow mainWindow(qManager);
    mainWindow.show();

    return app.exec();
}

