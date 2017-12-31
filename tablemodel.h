#ifndef TABLEMODEL_H
#define TABLEMODEL_H

#include "drawable.h"
#include <QtWidgets>

class TableModel : public QAbstractListModel {
  Q_OBJECT
public:
  explicit TableModel(QObject *parent = 0) : QAbstractListModel(parent) {}
  int rowCount(const QModelIndex &parent = QModelIndex()) const override {
    //    qDebug() << "row count " << drawables.size();
    return drawables.size();
  }

  QVariant data(const QModelIndex &index,
                int role = Qt::DisplayRole) const override {
    if (role == Qt::DisplayRole || index.row() < 0 ||
        index.row() >= drawables.size()) {
      return drawables[index.row()]->toString();
    }
    return QVariant();
  }

  QVector<Drawable *> getData() const { return drawables; }

  void add(Drawable *item) {
    beginInsertRows(QModelIndex(), drawables.size(), drawables.size());
    drawables.push_back(item);
    endInsertRows();

    QModelIndex top = createIndex(drawables.size() - 1, 0, nullptr);
    QModelIndex bottom = createIndex(drawables.size() - 1, 0, nullptr);

    emit dataChanged(top, bottom);
  }

  bool removeRows(int row, int count,
                  const QModelIndex &parent = QModelIndex()) override {
    beginRemoveRows(QModelIndex(), row, row + count - 1);
    while (count--)
      delete drawables.takeAt(row);
    endRemoveRows();
    return true;
  }

private:
  QVector<Drawable *> drawables;

  QString posToStr(Position pos) {
      switch (pos) {
          case Position::Left: return "Left";
          case Position::Right: return "Right";
      }
  }
};

#endif // TABLEMODEL_H
