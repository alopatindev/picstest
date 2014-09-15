#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMap>
#include <QSet>
#include <QTimer>
#include <cstdint>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef QMap<QString, QString> TypesMap;
    typedef QSet<int> QIntSet;
    enum State {
        None,
        InProgress,
        Done,
    } state;

    Ui::MainWindow *ui;
    QString resourcesDir;
    QString questionText;
    QString resultText;
    TypesMap imageTypes;
    int timeToChoose;
    int maxQuestions;
    QSize imageSize;
    QTimer timer;
    QStringList goodFiles;
    QStringList badFiles;
    QIntSet seenGood;
    QIntSet seenBad;
    int question;
    bool leftIsGood;
    int goodAnswers;
    uint64_t lastTime;
    int answersTime;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    static const uint64_t timestampMs();

public slots:
    void generateNextPair();
    void incrementAnswersTime();

protected:
    void closeEvent(QCloseEvent* event);
    void keyReleaseEvent(QKeyEvent* event);

private:
    void loadSettings(const QString& path);
    void generateQuestion();
    void listFiles(const QString& directory, QStringList& files);
    const QString& pickRandom(const QStringList& list, QIntSet& outSet);
    void setState(State state);
};

#endif // MAINWINDOW_H
