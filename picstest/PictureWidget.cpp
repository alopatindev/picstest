#include "PictureWidget.h"
#include <QPainter>

PictureWidget::PictureWidget(QWidget *parent) :
    QWidget(parent)
{
}

void PictureWidget::setPictureSize(const QSize& size)
{
    pixmapSize = size;
}

void PictureWidget::setPicture(const QString& filename)
{
    if (pixmap.load(filename))
        repaint();
}

void PictureWidget::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    int w = width();
    int h = (height() * pixmapSize.height()) / pixmapSize.width();
    p.drawPixmap(0, 0, w, h, pixmap);
}
