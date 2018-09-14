#pragma once
#ifndef __UNIFORMNAMELINEEDITDELEGATE_H__
#define __UNIFORMNAMELINEEDITDELEGATE_H__

#include <QItemDelegate>
#include <QLineEdit>
#include <QTextEdit>
#include <QDebug>

class UniformNameLineEditDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	UniformNameLineEditDelegate(QObject *parent = 0);
	~UniformNameLineEditDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget* editor, const QModelIndex &index) const;
	void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif