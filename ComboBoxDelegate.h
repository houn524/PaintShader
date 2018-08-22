#pragma once
#ifndef __COMBOBOXDELEGATE_H__
#define __COMBOBOXDELEGATE_H__

#include <QItemDelegate>
#include <QComboBox>
#include <QLineEdit>
#include "UniformManager.h"

class ComboBoxDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	ComboBoxDelegate(QObject *parent = 0);
	~ComboBoxDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget* editor, const QModelIndex &index) const;
	void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif