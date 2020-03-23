#ifndef PLOITWIDGET_H
#define PLOITWIDGET_H

#include <QWidget>
#include <QPoint>
#include <QPaintEvent>
#include <QList>


struct Range
{
  Range()= default;
  Range(int begin, int end) { this->begin = begin; this->end = end;}
  int begin;
  int end;
};

struct ElementOfDate
{
  ElementOfDate() = default;
  ElementOfDate(double x, double y) {this->xComponent = x, this->yComponent = y;}
  double xComponent;
  double yComponent;
};

class GraphWidget : public QWidget
{
  Q_OBJECT
public:
  GraphWidget(Range xR, Range yR,QWidget *parent = nullptr);
  ~GraphWidget();

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
