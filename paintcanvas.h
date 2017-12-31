#ifndef PAINTCANVAS_H
#define PAINTCANVAS_H

#include "drawable.h"
#include "tablemodel.h"
#include <QtWidgets>

class PaintCanvas : public QFrame {
  Q_OBJECT

public:
  PaintCanvas(QWidget *parent = nullptr);
  void setColor(QColor);
  QPixmap getPixmap() const;
  void setTableModel(TableModel *);
  void paintConvexHull(QList<Drawable *> const &);

public slots:
  void setThickness(int);
  void clearArea();

protected:
  void paintEvent(QPaintEvent *);
  void mousePressEvent(QMouseEvent *);
  void resizeEvent(QResizeEvent *);

private:
  QColor color = Qt::black;
  QColor bgColor = Qt::white;
  QPixmap pixmap;
  QPointF curPos;
  int penThickness = 2;
  TableModel *tModel;
};

#endif // PAINTCANVAS_H
