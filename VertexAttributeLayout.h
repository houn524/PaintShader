#pragma once

#include <QWidget>
#include "ui_VertexAttributeLayout.h"

class VertexAttributeLayout : public QWidget
{
	Q_OBJECT

public:
	VertexAttributeLayout(QWidget *parent = Q_NULLPTR);
	QLineEdit* getSizeWidget() { return ui.lnEdit_Size; };
	QTextEdit* getDataWidget() { return ui.txtEdit_Data; };
	~VertexAttributeLayout();

private:
	Ui::VertexAttributeLayout ui;
};
