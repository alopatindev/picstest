#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QCloseEvent>
#include <QMap>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

    Ui::MainWindow *ui;
    QString questionText;
    QMap<QString, QString> imageTypes;
    int timeToChoose;
    int maxQuestions;

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

protected:
    void paintEvent(QPaintEvent* event);
    void closeEvent(QCloseEvent* event);

private:
    void loadSettings(const QString& path);
    void generateQuestion();
};

#endif // MAINWINDOW_H
