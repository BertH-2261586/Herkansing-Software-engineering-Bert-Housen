#include <QApplication>
#include <QMainWindow>
#include <QSettings>

#include "Controller/questionmanagercontroller.h"
#include "model/manager/questionmanager.h"
#include "view/leerhulpmiddelmainwindow.h"
#include "view/Login/LoginView.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QuestionManager qManager;
    QuestionManagerController* questionManagerController = new QuestionManagerController(qManager);

    LeerhulpmiddelMainWindow mainWindow(questionManagerController);
    
    mainWindow.show();
    
    return app.exec();
}

