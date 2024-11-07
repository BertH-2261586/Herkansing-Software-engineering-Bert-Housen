#include <QApplication>
#include <QMainWindow>

#include "fileManager.h"
#include "model/Questions/MultipleChoiceQuestion.h"

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    // Create the main window
    QMainWindow mainWindow;
    mainWindow.setWindowTitle("Leerhulpmiddel");
    mainWindow.showMaximized();

    //try {
    //    FileManager fileManager;
    //    QMap<QString, QVariantList> questionSets = fileManager.loadQuestionSets();
    //    qDebug() << questionSets;
    //    fileManager.makeQuestionSet("placeholder", "testingfolder");
    //    MultipleChoiceQuestion multiQuestion = MultipleChoiceQuestion("wiskunde", "wat is 2+2", "4");
    //    fileManager.saveQuestionToJSON("placeholder", multiQuestion);
    //    unique_ptr<Question> loadedQuestion = fileManager.loadQuestionFromJSON("placeholder", "wiskunde");
    //    qDebug() << loadedQuestion->getName() << " " << loadedQuestion->getQuestion() << " " << loadedQuestion->getAnswer();
    //}
    //catch (const FolderQuestionSetMovedException& e) {
    //    qDebug() << "An error has occured: " << e.what();
    //}
    //catch (const filesystem::filesystem_error& e) {
    //    qDebug() << "An error has occured: " << e.what();
    //}
    //catch (const saveException& e) {
    //    qDebug() << "An error has occured: " << e.what();
    //}
    //catch (const loadException& e) {
    //    qDebug() << "An error has occured: " << e.what();
    //}

    return app.exec();
}
