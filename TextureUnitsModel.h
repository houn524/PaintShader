#pragma once
#ifndef __TEXTUREUNITSMODEL_H__
#define __TEXTUREUNITSMODEL_H__

#include <QAbstractTableModel>
#include <QDebug>

class TextureUnitsModel : public QAbstractTableModel
{
	Q_OBJECT
public:
	TextureUnitsModel(QObject *parent = 0);
	~TextureUnitsModel();

	void setTextureVector(QVector<QString> *vector);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	bool insertRows(int position, int rows, const QModelIndex &parent) override;
	bool removeRows(int position, int rows, const QModelIndex &index) override;
	void refresh();

private:


	QVector<QString> *textureVector;
};

#endif