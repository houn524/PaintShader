#pragma once

#include <QWidget>
#include "ui_AnalogClock.h"
#include <QTimer>
#include <QTime>
#include <QPainter>

class AnalogClock : public QWidget
{
	Q_OBJECT

public:
	AnalogClock(QWidget *parent = Q_NULLPTR);

protected:
	void paintEvent(QPaintEvent *event) override;
};
