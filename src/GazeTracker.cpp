#include "GazeTracker.hpp"

#include <QPaintEvent>
#include <QPainter>

constexpr auto TRACKER_WIDTH = 40;
constexpr auto TRACKER_HEIGHT = 40;

GazeTracker::GazeTracker(QWidget *parent) : QWidget(parent)
{
    this->resize(TRACKER_WIDTH, TRACKER_HEIGHT);
    this->setAttribute(Qt::WA_TranslucentBackground, true);
    this->setWindowFlag(Qt::FramelessWindowHint, true);
    this->setWindowFlag(Qt::WindowStaysOnTopHint, true);
}

void GazeTracker::MoveTo(int x, int y)
{
    this->move(x + TRACKER_WIDTH / 2, y + TRACKER_HEIGHT / 2);
}

void GazeTracker::paintEvent(QPaintEvent *event)
{
    QPainter p{ this };
    auto rect = event->rect();
    rect.setWidth(TRACKER_WIDTH - 3);
    rect.setHeight(TRACKER_HEIGHT - 3);
    p.drawEllipse(rect);
}
