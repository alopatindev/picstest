#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMap>
#include <QTimer>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    typedef QMap<QString, QString> TypesMap;
    enum State {
        None,
        InProgress,
        Done,
    } state;

    Ui::MainWindow *ui;
    QString questionText;
    TypesMap imageTypes;
    int timeToChoose;
    int maxQuestions;
    QSize imageSize;
    QTimer timer;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

public slots:
    void generateNextPair();

protected:
    void closeEvent(QCloseEvent* event);
    void keyReleaseEvent(QKeyEvent *event);

private:
    void loadSettings(const QString& path);
    void generateQuestion();
};

#endif // MAINWINDOW_H
