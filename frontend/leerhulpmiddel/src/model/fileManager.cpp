#include <QApplication>
#include <QDir>
#include <QDebug>
#include <QString>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include <QJsonArray>
#include <QProcess>
#include <QBuffer>
#include <QTemporaryFile>
#include <QThread>

#include "fileManager.h"
#include "Questions/MultipleChoiceQuestion.h"
#include "Questions/Flashcard.h"
#include "Questions/FillInQuestion.h"
#include "../Exceptions/unzipException.h"

namespace filesystem = std::filesystem; 

// Function te get the basic path to the project
QString FileManager::getPath() const {
    // Gets the path to the .exe file. Pay attention this isnt the file to the project folder
    QString projectDirPath = QCoreApplication::applicationDirPath();

    QStringList segments = projectDirPath.split('/');
    // Remove the last 2 segments of the path to the .exe file to get to the project folder
    segments.removeLast(); // Remove 'Debug'
    if (segments.last() != "frontend")
    {
        segments.removeLast(); // Remove 'x64'
    }

    // Combine all the segments
    QString newPath = segments.join('/');
    return newPath + "/leerhulpmiddel/questionSets";        // Add the questionSets folder to the path for where the question sets are stored locally
}

/**
 * Loads the question sets from the specified path.
 *
 * @param path The directory where the question sets are located. If no path is given, the default project directory is used.
 * @return QMap<QString, QVariantList> Map that associates the folder name with a list of questions or components of the question set.
 * @throws FolderQuestionSetMovedException if the folder cannot be found at the base path
 */
QMap<QString, QVariantList> FileManager::loadQuestionSets(QString path) const {
    if (path.isEmpty()) {
        // Use the standard path if no path is given
        path = getPath();
    }

    QDir dir(path);

    // Control if the map exists
    if (!dir.exists()) {
        throw FolderQuestionSetMovedException("The question set folder is not found on the given path: " + dir.path());
    }

    // Load the file and questions from the given path
    QMap<QString, QVariantList> list = loadFilesAndQuestions(dir);

    return list;
}

// Makes a new folder for a new question set
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
 * Helper function that loads questions and components of the question set from a path.
 *
 * @pre dir is a correct path to all saved question sets and questions
 * @param dir The QDir directory where all questions and components are stored.
 * @param currentDepth The current depth of recursion, to limit a maximum level.
 * @return QMap<QString, QVariantList> The contents of the directory, grouped by subdirectory.
 */
QMap<QString, QVariantList> FileManager::loadFilesAndQuestions(const QDir& dir, int currentDepth) const {
    // Control if the max depth is reached
    const int MAX_DEPTH = 1;
    if (currentDepth > MAX_DEPTH) {
        return QMap<QString, QVariantList>(); // Return empty if the max depth is reached
    }

    QMap<QString, QVariantList> folderFiles;

    // List of all subdirectories in the current directory
    QStringList directories = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);

    // Go through all sub directories
    foreach(const QString & dirName, directories) {
        // Make a new QDir for the sub folder
        QDir subDir(dir.absoluteFilePath(dirName));
        QStringList files = subDir.entryList(QDir::Files);
        QVariantList contents;

        // Go recursively through the subfolder
        QMap<QString, QVariantList> subfolderContents = loadFilesAndQuestions(subDir, ++currentDepth);


        // Add the subfolder contents to the main content
        foreach(const QString & subDirName, subfolderContents.keys()) {
            QVariantList subfolderInfo;
            subfolderInfo.append(subDirName);                       // Add the name of the subfolder
            subfolderInfo.append(subfolderContents[subDirName]);    // Add the data of the subfolder
            contents.append(QVariant::fromValue(subfolderInfo));    // Add the full info for subfolder to the main content
        }

        // Add the found files to the content
        foreach(const QString& fileName, files) {
            contents.append(fileName);
        }

        // Add the data to the map, with the folder name as key
        folderFiles[dirName] = contents;
        currentDepth = 0;
    }

    return folderFiles;
}

/**
 * Saves a question locally in a JSON file at a specified location.
 *
 * @param questionPath The file path where the question should be saved.
 * @param question The question to save in JSON format.
 * @throw saveException if a file with the same name already exists or if something goes wrong while saving the file.
 * @return bool Returns true if the save was successful.
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

    // Open the file in write-only mode. This makes the JSON file incase it doesn't exist
    QString path = getPath()+ addedPath + "/" + QString::fromStdString(question.getName().toStdString()) + ".json";
    QFile file(path);
    if (file.exists()) {
        throw saveException("This file already exists.");
    }

    if (!file.open(QIODevice::WriteOnly)) {
        throw saveException("Something went wrong while creating the file.");
    }

    // Write the Json data to the file
    file.write(jsonDoc.toJson());  // Convert the QJsonDocument to JSON-formaat
    file.close();
}


/**
 * Retrieves a question from local storage and converts it to the appropriate question class, then returns it.
 *
 * @param questionPath The file path where the question is stored.
 * @param questionName The name of the question to search for.
 * @throw loadException If the question does not exist, if the file cannot be read, if it cannot be converted to JSON format, or if a non-existent question is given.
 * @return unique_ptr<Question> Returns the appropriate question.
 */
unique_ptr<Question> FileManager::loadQuestionFromJSON(const QString questionSet, const QString subset, const QString questionName) const {

    // Path created from the question set and subset
    QString addedPath = "/" + questionSet;
    if (subset != "")
    {
        addedPath += "/" + subset;
    }

    QString path = getPath() + addedPath + "/" + questionName + ".json";
    QFile file(path);

    // Check if the question file exists
    if (!file.exists()) {
        throw loadException("The file does not exist: " + path);
    }

    // Opens the file in read-only mode
    if (!file.open(QIODevice::ReadOnly)) {
        throw loadException("Could not open the file: " + path);
    }

    // Reads the data from the file
    QByteArray fileData = file.readAll();
    file.close(); // Closes the file after reading

    // convert to a JSON document
    QJsonDocument jsonDoc = QJsonDocument::fromJson(fileData);
    if (jsonDoc.isNull() || !jsonDoc.isObject()) {
        throw loadException("Failed to create JSON doc from the file: " + path);
    }

    QJsonObject jsonObject = jsonDoc.object();

    // Gets the needed data from the JSON object
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
    QDir dir = getPath() + "/" + questionSetPath;

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
                QString subfolderName = fileFolderInfo.fileName(); // This is the current subdirectory name

                try {
                    // Load question from the file in the subdirectory
                    auto question = loadQuestionFromJSON(questionSetPath, subfolderName, fileName);
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
                auto question = loadQuestionFromJSON(questionSetPath, "", fileName);
                questions.append(std::move(question));  // Add the loaded question to the vector
            }
            catch (const std::exception& e) {
                qWarning() << "Error loading question from file:" << fileFolderInfo.absoluteFilePath() << e.what();
            }
        }
    }

    return questions;
}

/*
* This functions makes a zip of all the question set folders given to it
* @param questionSetPaths all the question sets that need to be zipped
* @return QByteArray is the zip file
* @pre the list of question set paths need to be valid and readable
* @post 
*   - Returns a QByteArray containing the zip file data if successful.
*   - Returns an empty QByteArray if zipping fails.
*/
QByteArray FileManager::createZip(const QStringList& questionSetPaths) {
    QProcess zipProcess;
    QByteArray zipData;
    QBuffer buffer(&zipData);
    buffer.open(QIODevice::WriteOnly);

    // Zipping command for windows
    #if defined(Q_OS_WIN)

        // Construct a PowerShell command to create a zip file using Compress-Archive
        QString script = "Compress-Archive -Path ";
        for (const QString& path : questionSetPaths) {
            // Append each question set path to the script
            script += "\"" + (getPath() + "/" + path).replace("/", "\\") + "\",";
        }

        script.chop(1);                                             // Remove the trailing comma from the list of paths
        script += " -DestinationPath \"$env:TEMP\\temp.zip\"";      // Use a temporary file

        // Start the PowerShell process to execute the zipping command
        QStringList args;
        args << "-Command" << script;
        zipProcess.start("powershell", args);
        
        // Check if the process finished successfully
        if (!zipProcess.waitForFinished() || zipProcess.exitCode() != 0) {
            qWarning() << "PowerShell zipping failed:" << zipProcess.readAllStandardError();
            return QByteArray();
        }

        // Read the resulting zip file from the temporary location
        QFile tempFile(qgetenv("TEMP") + "/temp.zip");
        if (tempFile.open(QIODevice::ReadOnly)) {
            zipData = tempFile.readAll();
            tempFile.remove(); // Clean up the temporary file
        }
        else {
            qWarning() << "Failed to read temporary zip file!";
        }
    // Construct a zip command for Linux/macOS
    #else
        QStringList zipArgs;
        zipArgs << "-r" << "-"; 
        for (const QString& path : questionSetPaths) {
            // Append each question set path to the zip arguments
            zipArgs << getPath() + "/" + path;
        }

        // Start the PowerShell process to execute the zipping command
        zipProcess.start("zip", zipArgs);

        // Check if the process finished successfully
        if (!zipProcess.waitForFinished() || zipProcess.exitCode() != 0) {
            qWarning() << "Zipping failed:" << zipProcess.readAllStandardError();
            return QByteArray();
        }

        // Read the zip output directly from the process's standard output
        zipData = zipProcess.readAllStandardOutput();
    #endif

    // Return the zip file
    return zipData;    
}

/*
* Unzip a given file 
* @pre zipData must contain valid zip data
* @post add the unzipped question to the question set list 
*/
void FileManager::unzip(QByteArray zipData) {
    // Write the zip data to a temporary file
    QString tempZipPath = QDir::tempPath() + "/temp.zip";
    QFile tempZipFile(tempZipPath);
    if (!tempZipFile.open(QIODevice::WriteOnly)) {
        qWarning() << "Failed to create temporary file for zip data!";
        throw unzipException("Failed to download the file");
    }
    tempZipFile.write(zipData);
    tempZipFile.close();

    QString destinationDir = getPath(); // Get the destination path

    // Zipping command for windows
    #if defined(Q_OS_WIN)
        // Use PowerShell to extract the zip
        QString script = QString("Expand-Archive -Path \"%1\" -DestinationPath \"%2\"")
            .arg(tempZipFile.fileName().replace("/", "\\"))
            .arg(destinationDir.replace("/", "\\"));

        // Start the PowerShell process to execute the unzipping command
        QProcess process;
        process.start("powershell", QStringList() << "-Command" << script);
        // Check if the extraction process finished successfully
        if (!process.waitForFinished() || process.exitCode() != 0) {
            qWarning() << "PowerShell unzip failed:" << process.readAllStandardError();
            throw unzipException("Failed to download the file");
        }

        // Rename duplicates to avoid overwriting
        QDir dir(destinationDir);
        QStringList existingNames = dir.entryList(QDir::Dirs | QDir::NoDotAndDotDot);
        foreach(const QFileInfo & fileInfo, dir.entryInfoList(QDir::Dirs | QDir::NoDotAndDotDot)) {
            QString baseName = fileInfo.fileName();
            QString uniqueName = getUniqueName(baseName, existingNames);

            if (uniqueName != baseName) {
                dir.rename(baseName, uniqueName);
            }

            // Update existing names after renaming
            existingNames.append(uniqueName);
        }

    // Construct a unzip command for Linux/macOS
    #else
        QProcess process;
        process.start("unzip", QStringList() << tempZipFile.fileName() << "-d" << destinationDir);
        // Check if the extraction process finished 
        if (!process.waitForFinished() || process.exitCode() != 0) {
            qWarning() << "Unzip failed:" << process.readAllStandardError();
            throw unzipException("Failed to download the file");
        }
    #endif
    
    // Delete the previously made temporary file
    if (!QFile::remove(tempZipPath)) {
        qWarning() << "Failed to delete temporary file:" << tempZipPath;
        throw unzipException("Something went wrong while deleting the temporary file");
    }
}

// Helper function to generate a unique name if a file or directory already exists
QString FileManager::getUniqueName(QString baseName, QStringList existingNames) {
    QString uniqueName = baseName;
    int counter = 1;

    // Adjust name format for duplicates
    while (existingNames.contains(uniqueName)) {
        uniqueName = QString("%1 (%2)").arg(baseName).arg(counter);
        counter++;
    }

    return uniqueName;
}
