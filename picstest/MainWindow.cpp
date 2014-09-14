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
    state(None)
{
    ui->setupUi(this);

    QString resourcesDir("../resources/");
    loadSettings(resourcesDir + "settings.xml");
    generateQuestion();
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*void MainWindow::paintEvent(QPaintEvent* event)
{
    //QPainter painter(this);
}*/

void MainWindow::loadSettings(const QString& path)
{
    QFile f(path);
    if (!f.exists())
    {
        QMessageBox::critical(this, "", "Cannot find " + path);
        close();
        return;
    }

    if (!f.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "", "Cannot read " + path);
        close();
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
    } while (badRand != goodRand);
    TypesMap::iterator badIterator = imageTypes.begin();
    for (int i = 0; i < badRand; ++i)
        badIterator++;

    ui->questionLabel->setText(questionText.arg(goodIterator.value()));

    timer.setInterval(timeToChoose);
}

void MainWindow::keyReleaseEvent(QKeyEvent *event)
{
    if (state == None)
    {
        state = InProgress;
        timer.start();
        return;
    }

    switch (event->key())
    {
    case Qt::Key_Left:
        break;
    case Qt::Key_Right:
        break;
    default:
        break;
    }
}

void MainWindow::closeEvent(QCloseEvent* event)
{
    event->accept();
}
