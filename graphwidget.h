#ifndef PLOITWIDGET_H
#define PLOITWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPaintEvent>
#include <QList>


struct Range
{
  Range()= default;
  Range(double begin, double end) { this->begin = begin; this->end = end;}
  double begin;
  double end;
};

struct ElementOfDate
{
  ElementOfDate() = default;
  ElementOfDate(double x, double y) {this->xComponent = x, this->yComponent = y;}
  double xComponent;
  double yComponent;
};

class PloitWidget : public QWidget
{
  Q_OBJECT
public:
  PloitWidget(Range xR, Range yR,QWidget *parent = nullptr);
  ~PloitWidget();

  void paintEvent(QPaintEvent *);
  void addElement(QPoint p);
  void addElement(ElementOfDate date);
  void addElement(double x, double y);
  void setRange(Range xR, Range yR);
  Range getXRange() { return xRange;}
  Range getYRange(){return yRange;}

private:
  void plot(QPainter *pen);

  QList<ElementOfDate> *points;
  Range xRange;
  Range yRange;

};

#endif // PLOITWIDGET_H
