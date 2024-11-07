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

    try {
        FileManager FileManager;
        QMap<QString, QVariantList> questionsets = FileManager.loadQuestionSets();
        qDebug() << questionsets;
        FileManager.makeQuestionSet("placeholder", "testingfolder");
        MultipleChoiceQuestion multiquestion = MultipleChoiceQuestion("wiskunde", "wat is 2+2", Answer(QList<QString>{"test","test2"}, QList<int> {1}));
        FileManager.saveQuestionToJSON("placeholder", multiquestion);
        unique_ptr<Question> loadedquestion = FileManager.loadQuestionFromJSON("placeholder", "wiskunde");
        qDebug() << loadedquestion->getName() << " " << loadedquestion->getQuestion() << " " << loadedquestion->getAnswer().getAnswers();
    }
    catch (const FolderQuestionSetMovedException& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const filesystem::filesystem_error& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const saveException& e) {
        qDebug() << "an error has occured: " << e.what();
    }
    catch (const loadException& e) {
        qDebug() << "an error has occured: " << e.what();
    }

    return app.exec();
}
