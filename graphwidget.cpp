#include "graphwidget.h"
#include <QPen>
#include <QPainter>

GraphWidget::GraphWidget(Range xR, Range yR, QWidget *parent)
  : QWidget(parent)
{
  setRange(xR, yR);
  points = new QList<ElementOfDate>;
}

GraphWidget::~GraphWidget()
{
  delete points;
}

void GraphWidget::paintEvent(QPaintEvent *)
{
  //Axis
  QPainter pen(this);
  pen.setPen(QPen(Qt::blue, 5, Qt::SolidLine));
  pen.drawLine(30, this->height() - 30, 30, 0);
  pen.drawLine(30, this->height() - 30, this->width(), this->height() - 30);
  pen.setPen(QPen(Qt::black, 1, Qt::PenStyle::SolidLine, Qt::PenCapStyle::FlatCap));
  int xStep = this->width()/(xRange.end - xRange.begin);
  for(int i(xRange.begin); i < xRange.end; i++)
  {
    if((xRange.end - xRange.begin > 5 && i%((xRange.end - xRange.begin)/5) == 0) || xRange.end - xRange.begin <= 5)
      pen.drawText(xStep * (i - xRange.begin) + 30, this->height() - 15, QString::number(xRange.begin + (i - xRange.begin)));
  }
  int yStep = this->height()/(yRange.end - yRange.begin);
  for(int i(yRange.end); i >= yRange.begin ; i--)
  {
    if((yRange.end - yRange.begin > 10 && i%((yRange.end - yRange.begin)/5) == 0) || yRange.end - yRange.begin <= 10)
      pen.drawText(5, yStep * (yRange.end - i) - 30,QString::number((yRange.end - (yRange.end - i))));
  }

  plot(&pen);
}

void GraphWidget::addElement(QPoint p)
{
  ElementOfDate date(p.x(),p.y());
  points->push_back(date);
  update();
}

void GraphWidget::addElement(ElementOfDate date)
{
  points->push_back(date);
  update();
}

void GraphWidget::addElement(double x, double y)
{
  ElementOfDate date(x,y);
  points->push_back(date);
  update();
}

void GraphWidget::setRange(Range xR, Range yR)
{
  this->xRange = xR;
  this->yRange = yR;
}

void GraphWidget::plot(QPainter *pen)
{
  if(points->isEmpty())
    return;
  for(int i(1); i < points->size(); i++)
  {
    pen->setPen(QPen(Qt::black, 4, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap));
    pen->drawLine(
          (this->width()/(xRange.end - xRange.begin)) * (points->at(i-1).xComponent - xRange.begin) + 30,
          this->height() - ((this->height()/(yRange.end - xRange.begin)) * (points->at(i-1).yComponent - yRange.begin)) - 30,
          (this->width()/(xRange.end - xRange.begin)) * (points->at(i).xComponent - xRange.begin) + 30,
          this->height() - ((this->height()/(yRange.end - xRange.begin)) * (points->at(i).yComponent - yRange.begin)) - 30);
  }
  pen->setPen(QPen(Qt::red, 10, Qt::PenStyle::SolidLine, Qt::PenCapStyle::RoundCap));
  pen->drawPoint(
        (this->width()/(xRange.end - xRange.begin)) * (points->back().xComponent - xRange.begin) + 30,
        this->height() - ((this->height()/(yRange.end - xRange.begin)) * (points->back().yComponent - yRange.begin)) - 30);
}
