#pragma once

#include <QWidget>

class GazeTracker : public QWidget
{
    Q_OBJECT

  public:
    explicit GazeTracker(QWidget *parent = nullptr);

    void MoveTo(int x, int y);

  protected:
    void paintEvent(QPaintEvent *event) override;
};
