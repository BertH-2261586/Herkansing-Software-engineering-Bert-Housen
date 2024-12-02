#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include <QJsonArray>

#include "fileManager.h"
#include "Questions/MultipleChoiceQuestion.h"
#include "Questions/Flashcard.h"
#include "Questions/FillInQuestion.h"

namespace filesystem = std::filesystem; 

// Functie om het basisprojectpad te krijgen
QString FileManager::getPath() const {
    // Dit haalt het pad van de .exe file op. Let op dit is niet het pad van het project
    QString projectDirPath = QCoreApplication::applicationDirPath();

    QStringList segments = projectDirPath.split('/');
    // Verwijder de laatste 2 segmenten om van het pad naar de .exe file om naar de project folder te gaan
    segments.removeLast(); // Verwijder 'Debug'
    if (segments.last() != "frontend")
    {
        segments.removeLast(); // Verwijder 'x64'
    }

    // Voeg de segmenten weer samen tot één pad
    QString newPath = segments.join('/');
    return newPath + "/leerhulpmiddel/questionSets";        // Voeg het pad toe voor aan de lokale opslag plaats te geraken
}

/**
 * Laadt de vraagensets uit het opgegeven pad.
 *
 * @param path De directory waarin de vraagensets zich bevinden. Indien geen path gegeven, wordt de standaardprojectdirectory gebruikt.
 * @return QMap<QString, QVariantList> Map die de mapnaam koppelt aan een lijst met vragen of onderdelen van de vragenset.
 * @throws FolderQuestionSetMovedException als de map niet gevonden kan worden op het basis pad
 */
QMap<QString, QVariantList> FileManager::loadQuestionSets(QString path) const {
    if (path.isEmpty()) {
        // Gebruik het standaardpad als er geen pad is opgegeven
        path = getPath();
    }

    QDir dir(path);

    // Controleer of de map bestaat
    if (!dir.exists()) {
        throw FolderQuestionSetMovedException("The question set folder is not found on the given path: " + dir.path());
    }

    // Laad bestanden en vragen vanuit de map
    QMap<QString, QVariantList> list = loadFilesAndQuestions(dir);

    return list;
}

// Er wordt een folder voor een nieuwe question set gemaakt
void FileManager::makeQuestionSet(QString path, QString fileName) const {
    try {
        QString pathName = getPath() /*path*/ + "/" + fileName;
        filesystem::create_directory(pathName.toStdString());
    }
    catch (const filesystem::filesystem_error& e) {
        throw;
    }
}

/**
 * Hulpfunctie die vragen en onderdelen van de vragenset laadt vanuit een pad.
 *
 * @pre dir is een correct pad naar alle opgeslagen vragensets en vragen
 * @param dir De QDir-map waar alle vragen en onderdelen zijn opgeslagen.
 * @param currentDepth De huidige diepte van recursie, om een maximaal niveau te beperken.
 * @return QMap<QString, QVariantList> De inhoud van de map, gegroepeerd per submap.
 */
QMap<QString, QVariantList> FileManager::loadFilesAndQuestions(const QDir& dir, int currentDepth) const {
    // Controleer of de maximale diepte is overschreden
    const int MAX_DEPTH = 1;
    if (currentDepth > MAX_DEPTH) {
        return QMap<QString, QVariantList>(); // Return leeg als de maximale diepte is bereikt
    }

    QMap<QString, QVariantList> folderFiles;

    // Lijst alle submappen
    QStringList directories = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Doorloop elke submap
    foreach(const QString & dirName, directories) {
        // Maak een nieuwe QDir aan voor de submap die je recursief gaat doorlopen
        QDir subDir(dir.absoluteFilePath(dirName));
        QStringList files = subDir.entryList(QDir::Files);
        QVariantList contents;

        // Ga recursief over de submap heen om dieper te kijken in de structuur

        QMap<QString, QVariantList> subfolderContents = loadFilesAndQuestions(subDir, ++currentDepth);


        // Voeg de inhoud van de submap toe aan de volledige mapinhoud
        foreach(const QString & subDirName, subfolderContents.keys()) {
            QVariantList subfolderInfo;
            subfolderInfo.append(subDirName);                       // Voeg de naam van de submap toe
            subfolderInfo.append(subfolderContents[subDirName]);    // Voeg de inhoud van de submap toe
            contents.append(QVariant::fromValue(subfolderInfo));    // Voeg de volledige submap-info toe aan inhoud
        }

        // Voeg de gevonden files toe aan de inhoudslijst
        foreach(const QString& fileName, files) {
            contents.append(fileName);
        }

        // Sla de inhoud op in de map, met de mapnaam als sleutel
        folderFiles[dirName] = contents;
    }


    return folderFiles;
}

/**
 * Slaat een vraag lokaal op in een JSON-bestand op een gespecificeerde locatie.
 *
 * @param questionPath Het bestandspad waar de vraag moet worden opgeslagen.
 * @param question De vraag om op te slaan in JSON-formaat.
 * @throw saveException als er een file bestaat met dezelfde naam of als er iets fout gaat met het saven van de file
 * @return bool Geeft true terug als het opslaan succesvol was.
 */
void FileManager::saveQuestionToJSON(const QString questionSet, const QString subset, const Question& question) const {
    QJsonObject jsonObject;

    jsonObject["QuestionType"] = question.questionTypeToString();
    jsonObject["Question"] = question.getQuestion();
    jsonObject["Answer"] = question.getAnswer().toJson();

    // Wrap the JSON object in a QJsonDocument
    QJsonDocument jsonDoc(jsonObject);


    // Path created from the question set and subset
    QString addedPath = "/" + questionSet;
    if (subset != "")
    {
        addedPath += "/" + subset;
    }

    // Open de file in write-only mode. Dit maakt de JSON file indien deze niet bestaat
    QString path = getPath()+ addedPath + "/" + QString::fromStdString(question.getName().toStdString()) + ".json";
    QFile file(path);
    if (file.exists()) {
        throw saveException("This file already exists.");
    }

    if (!file.open(QIODevice::WriteOnly)) {
        throw saveException("Something went wrong while creating the file.");
    }

    // Schrijf de JSON data naar de file
    file.write(jsonDoc.toJson());  // Converteer de QJsonDocument naar een JSON-formaat
    file.close();
}


/**
 * Haalt een vraag uit de lokale opslag op en zet deze om naar de gepaste question klasse en deze geeft deze terug mee.
 *
 * @param questionPath Het bestandspad waar de vraag moet worden opgeslagen.
 * @param questionName De naam van de vraag waar naar gezocht wordt.
 * @throw loadException Als de vraag niet bestaat, wanneer de file niet gelezen kan worden, kan niet naar JSON formaat omgezet worden, er wordt een niet bestaande vraag gegeven
 * @return unique_ptr<Question> Geeft de gepaste vraag.
*/
unique_ptr<Question> FileManager::loadQuestionFromJSON(const QString questionSet, const QString subset, const QString questionName) const {
    // Construeer de file path

    // Path created from the question set and subset
    QString addedPath = "/" + questionSet;
    if (subset != "")
    {
        addedPath += "/" + subset;
    }

    QString path = getPath() + addedPath + "/" + questionName + ".json";
    QFile file(path);

    // Check als de vraag bestaat in de gegeven folder
    if (!file.exists()) {
        throw loadException("The file does not exist: " + path);
    }

    // Open de file in read-only mode
    if (!file.open(QIODevice::ReadOnly)) {
        throw loadException("Could not open the file: " + path);
    }

    // Lees de data uit de file
    QByteArray fileData = file.readAll();
    file.close(); // Sluit de file nadat alle data is gelezen

    // Zet het om naar een JSON document
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        throw loadException("Failed to create JSON doc from the file: " + path);
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Haal de nodige data uit de file
    QString questionTypeString = jsonObject["QuestionType"].toString();
    QuestionType questionType = Question::stringToQuestionType(questionTypeString);
    QString question = jsonObject["Question"].toString();
    Answer answer = convertToAnswerObject(jsonObject["Answer"].toObject());

    switch (questionType) {
        case QuestionType::FillIn:
            return std::make_unique<FillInQuestion>(questionName, question, answer);
        case QuestionType::MultipleChoice:
            return std::make_unique<MultipleChoiceQuestion>(questionName, question, answer);
        case QuestionType::Flashcard:
            return std::make_unique<Flashcard>(questionName, question, answer);
        default:
            throw loadException("Unsupported QuestionType");
    }
}


/* Loads the question sets from the given path.
 *
 * @param path The directory where the question sets are located. If no path is given, the default project directory is used.
 * @return QList<Questionset*> List of question sets loaded from the given path.
 */
QList<Questionset*> FileManager::loadQuestionSetsObject(const QString path) const
{
   QList<Questionset*> questionSets = QList<Questionset*>();
   QMap<QString,QVariantList> MapFolders = loadQuestionSets(path);


   for (const QString& folderName : MapFolders.keys()) {
	   QVariantList folder = MapFolders[folderName];
	   QList<Question*> questions = QList<Question*>();
	   QList<Questionset*> subsets = QList<Questionset*>();


	   for (const QVariant& file : folder) {

           //if a json File is found
		   if (file.canConvert<QString>()) {
			   QString questionName = file.toString();
               questionName.chop(5); //remove .json
			   unique_ptr<Question> loadedQuestion = loadQuestionFromJSON(folderName, "", questionName);
			   questions.append(loadedQuestion.release());
		   }

           //if an other folder (subset) is found
           //IF ADDING MULTIPLE NESTED SUBSETS NEEDS TO BE CHANGED

		   else if (file.canConvert<QVariantList>()) {
			   QVariantList subsetValue = file.toList();
			   QString subsetName = subsetValue[0].toString();
			   QList<Question*> subsetQuestions = QList<Question*>();
			   for (int i = 1; i < subsetValue.length(); i++) {
				   QString questionName = subsetValue[i].toString();
                   questionName.chop(5); //remove .json
				   unique_ptr<Question> loadedQuestion = loadQuestionFromJSON(folderName, subsetName, questionName);
				   subsetQuestions.append(loadedQuestion.release());
			   }
               subsets.append(new Questionset(subsetName, subsetQuestions, {}));
		   }
	   }
	   questionSets.append(new Questionset(folderName, questions, subsets));
   }
    return questionSets;
}


Answer FileManager::convertToAnswerObject(QJsonObject answer) const {
    QList<QString> answers;
    QList<int> correctAnswers;
    QJsonArray answersArray = answer["answers"].toArray();
    for (const QJsonValue& value : answersArray) {
        answers.append(value.toString());
    }

    QJsonArray correctAnswersArray = answer["correctAnswers"].toArray();
    for (const QJsonValue& value : correctAnswersArray) {
        correctAnswers.append(value.toInt());
    }
    return Answer(answers, correctAnswers);
}

/*
* Given a path to a question set return all the questions associated with it 
* 
* @param questionSetPath this is the path to the question set where you want all the questions from
* @return QVector<Question*> this vector contains all the questions in the question set
*/
QVector<shared_ptr<Question>> FileManager::getAllQuestionsFromQuestionSet(const QString questionSetPath) const {
    QVector<shared_ptr<Question>> questions;  // Create the vector where you'll store the questions 

    // Create a QDir object to access the question set folder
    QDir dir = getPath() + "/test";

    // Filter only JSON files and directories
    dir.setFilter(QDir::Files | QDir::Dirs);  // Consider both files and directories

    // Iterate over each file and directory in the main directory
    QFileInfoList questionSetInfo = dir.entryInfoList();
    for (const QFileInfo& fileFolderInfo : questionSetInfo) {
        // Skip the current directory and parent directory entries
        if (fileFolderInfo.fileName() == "." || fileFolderInfo.fileName() == "..") {
            continue;
        }

        if (fileFolderInfo.isDir()) {
            // Handle subdirectory: recursively look for .json files in subdirectories
            QDir subDir(fileFolderInfo.absoluteFilePath());
            subDir.setNameFilters(QStringList() << "*.json");
            subDir.setFilter(QDir::Files);  // Only consider files, not directories

            QFileInfoList subFiles = subDir.entryInfoList();
            for (const QFileInfo& subFileInfo : subFiles) {
                QString fileName = subFileInfo.baseName();
                try {
                    // Load question from the file in the subdirectory

                    //HARDCODED FOR DEMO, NEEDS TO CHANGE
                    auto question = loadQuestionFromJSON("test", "testingfolder", fileName);
                    questions.append(std::move(question));  // Add the loaded question to the vector
                }
                catch (const std::exception& e) {
                    qWarning() << "Error loading question from file:" << subFileInfo.absoluteFilePath() << e.what();
                }
            }
        }
        else if (fileFolderInfo.isFile()) {
            // Handle file in the main directory
            QString fileName = fileFolderInfo.baseName();
            try {
                // Load question from the main directory

                //HARDCODED FOR DEMO, NEEDS TO CHANGE
                auto question = loadQuestionFromJSON("test", "", fileName);
                questions.append(std::move(question));  // Add the loaded question to the vector
            }
            catch (const std::exception& e) {
                qWarning() << "Error loading question from file:" << fileFolderInfo.absoluteFilePath() << e.what();
            }
        }
    }

    for (shared_ptr<Question> question : questions) {
        qDebug() << question->getQuestion() << question->questionTypeToString();  // Print each entry after the loop
    }

    return questions;
}
