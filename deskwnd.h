#ifndef DESKWND_H
#define DESKWND_H

#include <QMainWindow>
#include <QFrame>
#include <QGridLayout>
#include <QKeyEvent>

namespace Ui {
  class DeskWnd;
}

class Board : public QMainWindow
{
  Q_OBJECT

public:
  Board(QWidget *parent = nullptr, int _deskSize = 10);
  void burn(int x, int y, const QString& color);
  void clear();
  ~Board();
public slots:
  void score(QString);
signals:
  void pressKey(int);
private:
  virtual void keyPressEvent(QKeyEvent *event);

private:
  int deskSize;
  QGridLayout *mainLay;
  QFrame** cell;
  Ui::DeskWnd *ui;
};

#endif // DESKWND_H
