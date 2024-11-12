#include "View/leerhulpmiddelmainwindow.h"
#include "model/questionmanager.h"
#include <QApplication>
#include <QMainWindow>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window

    QuestionManager qManager;

    LeerhulpmiddelMainWindow mainWindow(qManager);
    mainWindow.show();

    return app.exec();
}

