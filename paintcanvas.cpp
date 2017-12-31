#include "paintcanvas.h"
#include <queue>

PaintCanvas::PaintCanvas(QWidget *parent) : QFrame(parent) {
    pixmap = QPixmap(this->width(), this->height());
    pixmap.fill();
}

void PaintCanvas::setColor(QColor c) { color = c; }

QPixmap PaintCanvas::getPixmap() const { return pixmap; }

void PaintCanvas::setTableModel(TableModel *tM) { tModel = tM; }

void PaintCanvas::setThickness(int val) { penThickness = val; }

void PaintCanvas::clearArea() {
    pixmap.fill();
    repaint();
}

void PaintCanvas::paintEvent(QPaintEvent *) {
    // pixmap.fill();
    for (Drawable *d : tModel->getData())
        d->draw(pixmap);
    QPainter p(this);
    p.drawPixmap(0, 0, pixmap);
}

void PaintCanvas::mousePressEvent(QMouseEvent *e) {
    curPos = e->pos();
    if (e->buttons() & Qt::LeftButton)
        tModel->add(new DrawablePoint(curPos));
    pixmap.fill();
    repaint();
}

void PaintCanvas::resizeEvent(QResizeEvent *) {
    pixmap = QPixmap(width(), height());
    pixmap.fill();
}

void PaintCanvas::paintConvexHull(QList<Drawable *> const & cv) {
    for (int l = cv.length(), i = 0; i < l; ++i) {
        DrawableLine line(
            dynamic_cast<DrawablePoint *>(cv[i])->get_center(),
            dynamic_cast<DrawablePoint *>(cv[(i + 1) % l])->get_center()
        );
        line.draw(pixmap);
    }
    repaint();
}
