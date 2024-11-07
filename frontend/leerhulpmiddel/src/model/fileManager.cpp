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
    segments.removeLast(); // Verwijder 'x64'

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
        throw FolderQuestionSetMovedException("The question set folder is not found on the given path: " + dir.absolutePath());
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
        qDebug() << "Directory:" << dirName;

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
            qDebug() << "  File:" << fileName;
            contents.append(fileName);
        }

        // Sla de inhoud op in de map, met de mapnaam als sleutel
        folderFiles[dirName] = contents;
    }

    // Voeg bestanden uit de huidige map toe
    QStringList filesInCurrentDir = dir.entryList(QDir::Files);
    foreach(const QString & fileName, filesInCurrentDir) {
        qDebug() << "File in current directory:" << fileName;
        folderFiles[dir.dirName()].append(fileName); // Sla bestanden direct op als QString
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
void FileManager::saveQuestionToJSON(const string questionPath, const Question& question) const {
    QJsonObject jsonObject;
    jsonObject["QuestionType"] = question.questionTypeToString();
    jsonObject["Question"] = question.getQuestion();
    jsonObject["Answer"] = question.getAnswer().toJson();

    // Wrap the JSON object in a QJsonDocument
    QJsonDocument jsonDoc(jsonObject);

    // Open de file in write-only mode. Dit maakt de JSON file indien deze niet bestaat
    QString path = getPath() + "/" + QString::fromStdString(question.getName().toStdString()) + ".json";
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
unique_ptr<Question> FileManager::loadQuestionFromJSON(const string questionPath, const QString questionName) const {
    // Construeer de file path
    QString path = getPath() + "/" + questionName + ".json";
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

