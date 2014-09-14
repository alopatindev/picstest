#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>

class PictureWidget : public QWidget
{
    Q_OBJECT
public:
    explicit PictureWidget(QWidget *parent = 0);

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // PICTUREWIDGET_H
