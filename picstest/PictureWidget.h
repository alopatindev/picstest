#ifndef PICTUREWIDGET_H
#define PICTUREWIDGET_H

#include <QWidget>

class PictureWidget : public QWidget
{
    Q_OBJECT

    QPixmap pixmap;
    QSize pixmapSize;

public:
    explicit PictureWidget(QWidget *parent = 0);
    void setPictureSize(const QSize& size);
    void setPicture(const QString& filename);

protected:
    void paintEvent(QPaintEvent* event);

signals:

public slots:

};

#endif // PICTUREWIDGET_H
