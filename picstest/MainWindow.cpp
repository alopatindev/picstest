#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QFile>
#include <QtXml>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    timeToChoose(0),
    maxQuestions(0),
    question(0),
    leftIsGood(false),
    state(None),
    goodAnswers(0),
    avgAnswerSpeed(0)
{
    ui->setupUi(this);

    resourcesDir = "../resources/";
    loadSettings(resourcesDir + "settings.xml");
    generateQuestion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::loadSettings(const QString& path)
{
    QFile f(path);
    if (!f.exists())
    {
        QMessageBox::critical(this, "", "Cannot find " + path);
        emit close();
        return;
    }

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "", "Cannot read " + path);
        emit close();
        return;
    }

    QString text = f.readAll();
    f.close();

    QDomDocument doc;
    QString errorMsg;
    int errorLine;
    if (!doc.setContent(&f, &errorMsg, &errorLine))
    {
        QMessageBox::critical(this, "", errorMsg + " line " + QString::number(errorLine));
        close();
        return;
    }

    QDomElement docElem = doc.documentElement();

    QDomNode n = docElem.firstChild();
    while(!n.isNull())
    {
        QDomElement e = n.toElement();
        if (n.nodeName() == "question")
            questionText = e.attribute("text");
        else if (n.nodeName() == "image_types")
        {
            QDomNode nn = n.firstChild();
            while (!nn.isNull())
            {
                QDomElement ee = nn.toElement();
                imageTypes[ee.attribute("directory")] = ee.attribute("text");
                nn = nn.nextSibling();
            }
        }
        else if (n.nodeName() == "time_to_choose")
            timeToChoose = e.attribute("time").toInt();
        else if (n.nodeName() == "max_questions")
            maxQuestions = e.attribute("number").toInt();
        else if (n.nodeName() == "image_size")
            imageSize = QSize(e.attribute("width").toInt(), e.attribute("height").toInt());
        n = n.nextSibling();
    }
}

void MainWindow::generateQuestion()
{
    qsrand(QTime::currentTime().msec());

    int goodRand = qrand() % imageTypes.size();
    TypesMap::iterator goodIterator = imageTypes.begin();
    for (int i = 0; i < goodRand; ++i)
        goodIterator++;

    int badRand;
    do {
        badRand = qrand() % imageTypes.size();
    } while (badRand == goodRand);
    TypesMap::iterator badIterator = imageTypes.begin();
    for (int i = 0; i < badRand; ++i)
        badIterator++;

    ui->questionLabel->setText(questionText.arg(goodIterator.value()));
    ui->leftImage->setPictureSize(imageSize);
    ui->rightImage->setPictureSize(imageSize);

    listFiles(resourcesDir + goodIterator.key(), goodFiles);
    listFiles(resourcesDir + badIterator.key(), badFiles);

    timer.setInterval(timeToChoose);
}

void MainWindow::listFiles(const QString& directory, QStringList& files)
{
    QDir dir(directory);
    dir.setFilter(QDir::Files);
    QFileInfoList list = dir.entryInfoList();
    for (int i = 0; i < list.size(); ++i)
        files.append(list[i].absoluteFilePath());
}

void MainWindow::done()
{
    state = Done;
    ui->leftImage->setVisible(false);
    ui->rightImage->setVisible(false);
    float result = (float(goodAnswers) * 100.0f) / float(maxQuestions);
    ui->questionLabel->setText(
        QString("Your result is %1%. Your average answer speed is %2 ms.")
                .arg(result)
                .arg(avgAnswerSpeed));
}

void MainWindow::generateNextPair()
{
    ++question;
    if (question > maxQuestions)
    {
        done();
        return;
    }

    QString goodFile = pickRandom(goodFiles, seenGood);
    QString badFile = pickRandom(badFiles, seenBad);
    leftIsGood = qrand() % 2 == 0;
    if (leftIsGood)
    {
        ui->leftImage->setPicture(goodFile);
        ui->rightImage->setPicture(badFile);
    }
    else
    {
        ui->leftImage->setPicture(badFile);
        ui->rightImage->setPicture(goodFile);
    }
}

const QString& MainWindow::pickRandom(const QStringList& list, QIntSet& outSet)
{
    int i;
    do {
        i = qrand() % list.size();
    } while (outSet.contains(i));
    outSet.insert(i);
    return list[i];
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (state == None)
    {
        state = InProgress;
        generateNextPair();
        timer.start();
        return;
    }

    if (state == Done)
        return;

    switch (event->key())
    {
    case Qt::Key_Left:
        if (leftIsGood)
            goodAnswers++;
        generateNextPair();
        break;
    case Qt::Key_Right:
        if (!leftIsGood)
            goodAnswers++;
        generateNextPair();
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}
