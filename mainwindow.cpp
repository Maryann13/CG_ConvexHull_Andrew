#include "mainwindow.h"
#include "tablemodel.h"
#include "drawable.h"
#include "ui_mainwindow.h"
#include <algorithm>

MainWindow::MainWindow(QWidget *parent)
    : QWidget(parent), ui(new Ui::MainWindow) {
  ui->setupUi(this);

  TableModel *tModel = new TableModel();

  ui->primitivesList->setModel(tModel);
  ui->canvas->setTableModel(tModel);
  ui->primitivesList->setSelectionMode(QAbstractItemView::ExtendedSelection);

  connect(ui->primitivesList, &MyListView::itemsChanged, [&]() {
    ui->canvas->clearArea();
    ui->canvas->repaint();
  });
}

MainWindow::~MainWindow() { delete ui; }

bool comparingY(Drawable * p1, Drawable * p2)
{
    return dynamic_cast<DrawablePoint *>(p1)->get_center().y()
                < dynamic_cast<DrawablePoint *>(p2)->get_center().y();
}

bool comparingX(Drawable * p1, Drawable * p2)
{
    return dynamic_cast<DrawablePoint *>(p1)->get_center().x()
                < dynamic_cast<DrawablePoint *>(p2)->get_center().x();
}

DrawableLine get_line(Drawable * p1, Drawable * p2)
{
    return DrawableLine(
        dynamic_cast<DrawablePoint *>(p1)->get_center(),
        dynamic_cast<DrawablePoint *>(p2)->get_center()
    );
}

void MainWindow::on_constructButton_clicked()
{
    QList<Drawable *> points = dynamic_cast<TableModel *>
            (ui->primitivesList->model())->getData().toList();
    if (points.empty())
        return;
    auto l_it = std::min_element(points.begin(), points.end(), comparingX);
    auto r_it = std::max_element(points.begin(), points.end(), comparingX);
    DrawableLine line(dynamic_cast<DrawablePoint *>(*l_it)->get_center(),
                      dynamic_cast<DrawablePoint *>(*r_it)->get_center());
    points.erase(l_it);
    if (l_it != r_it)
        points.erase(r_it);
    auto p_it = std::stable_partition(points.begin(), points.end(),
                          [&line](Drawable * p) {
        return dynamic_cast<DrawablePoint *>(p)->relativePosition(line)
                == Position::Right;
    });
    QList<Drawable *> psBelow, psAbove;
    for (auto it = points.begin(); it != p_it; ++it)
        psBelow.append(*it);
    for (auto it = p_it; it != points.end(); ++it)
        psAbove.append(*it);

    qSort(psBelow.begin(), psBelow.end(), comparingX);
    qSort(psAbove.begin(), psAbove.end(), [](Drawable * p1, Drawable * p2) {
        return !comparingX(p1, p2);
    });

    DrawablePoint pb(line.get_p1());
    DrawablePoint pa(line.get_p2());

    QList<Drawable *> belowHull, aboveHull;
    belowHull.append(&pb);
    aboveHull.append(&pa);

    if (!psBelow.empty()) {
        psBelow.append(&pa);
        belowHull.push_front(psBelow.first());
        psBelow.erase(psBelow.begin());

        for (int i = 0; i < psBelow.length(); ++i) {
            DrawablePoint * p = dynamic_cast<DrawablePoint *>(psBelow[i]);

            while (belowHull.length() > 1 &&
                   p->relativePosition(get_line(belowHull[1], belowHull[0]))
                       == Position::Right)
                belowHull.erase(belowHull.begin());
            belowHull.push_front(p);
        }
    }

    if (!psAbove.empty()) {
        psAbove.append(&pb);
        aboveHull.push_front(psAbove.first());
        psAbove.erase(psAbove.begin());

        for (int i = 0; i < psAbove.length(); ++i) {
            DrawablePoint * p = dynamic_cast<DrawablePoint *>(psAbove[i]);

            while (aboveHull.length() > 1 &&
                   p->relativePosition(get_line(aboveHull[1], aboveHull[0]))
                       == Position::Right)
                aboveHull.erase(aboveHull.begin());
            aboveHull.push_front(p);
        }
    }

    for (auto it = aboveHull.begin(); it != aboveHull.end(); ++it)
        belowHull.append(*it);
    ui->canvas->paintConvexHull(belowHull);
}
