#ifndef DRAWABLE_H
#define DRAWABLE_H

#include <QtWidgets>

class Drawable {
public:
  virtual void draw(QPixmap &pixmap) const = 0;
  virtual QString toString() = 0;

protected:
  QString pToStr(QPointF const &p) {
    return "(" + QString::number(p.x()) + ", " + QString::number(p.y()) + ")";
  }
};

class DrawableLine : public Drawable {
friend class DrawablePoint;

private:
  QPointF p1, p2;

public:
  DrawableLine(QPointF pFrom, QPointF pTo) : p1(pFrom), p2(pTo) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(QPen(Qt::black, 2));
    p.drawLine(p1, p2);
  }

  QPointF get_p1() const {
      return p1;
  }

  QPointF get_p2() const {
      return p2;
  }

  QString toString() override {
    return "Line " + pToStr(p1) + " -- " + pToStr(p2);
  }
};

enum class Position { Left, Right };

class DrawablePoint : public Drawable {
private:
  QPointF center;

public:
  DrawablePoint(QPointF p) : center(p) {}
  void draw(QPixmap &pixmap) const override {
    QPainter p(&pixmap);
    p.setBrush(QBrush(Qt::black));
    p.setPen(Qt::black);
    p.drawEllipse(center, 3, 3);
  }

  QPointF get_center() const {
      return center;
  }

  Position relativePosition(DrawableLine const & line) {
      float a = line.p2.y() - line.p1.y();
      float b = line.p1.x() - line.p2.x();
      float c = - line.p2.x() * a - line.p2.y() * b;

      float t = a * center.x() + b * center.y() + c;
      if (t > 0)
          return Position::Left;
      else
          return Position::Right;
  }

  QString toString() override { return "Point " + pToStr(center); }
};

#endif // DRAWABLE_H
