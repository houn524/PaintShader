#pragma once
#ifndef __TEXTUREFILENAMEDELEGATE_H__
#define __TEXTUREFILENAMEDELEGATE_H__

#include <QItemDelegate>
#include <QLineEdit>
#include <QFileDialog>
#include <QDebug>

class TextureFileNameDelegate : public QItemDelegate
{
	Q_OBJECT

public:
	TextureFileNameDelegate(QObject *parent);
	~TextureFileNameDelegate();

	QWidget* createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const;
	void setEditorData(QWidget* editor, const QModelIndex &index) const;
	void setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const;
	void updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif