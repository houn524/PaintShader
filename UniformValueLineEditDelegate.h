#pragma once
#ifndef __UNIFORMVALUELINEEDITDELEGATE_H__
#define __UNIFORMVALUELINEEDITDELEGATE_H__

#include <QItemDelegate>
#include <QLineEdit>

class UniformValueLineEditDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	UniformValueLineEditDelegate(QObject *parent = 0);
	~UniformValueLineEditDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget* editor, const QModelIndex &index) const;
	void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif