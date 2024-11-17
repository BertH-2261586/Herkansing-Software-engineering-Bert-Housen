#include <QApplication>
#include <QMainWindow>

#include "Controller/questionmanagercontroller.h"
#include "model/manager/questionmanager.h"
#include "view/leerhulpmiddelmainwindow.h"


int main(int argc, char* argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QuestionManager qManager;

    LeerhulpmiddelMainWindow mainWindow(qManager);
    mainWindow.show();

    return app.exec();
}

