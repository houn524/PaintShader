#pragma once
#ifndef __UNIFORMSMODEL_H__
#define __UNIFORMSMODEL_H__

#include <QAbstractTableModel>
#include <QMetaEnum>
#include <QDebug>


class UniformsModel : public QAbstractTableModel
{
	Q_OBJECT

public:
	enum UniformTypes
	{
		Float, Int, Vec2, Vec3, Vec4, Mat3, Mat4
	};

	struct Uniform
	{
		UniformsModel::UniformTypes type;
		QString name;
		QVector<float> value;
	};

	Q_ENUM(UniformTypes)
	UniformsModel(QObject *parent = 0);
	~UniformsModel();

	void setUniformVector(QVector<Uniform> *vector);
	int rowCount(const QModelIndex &parent = QModelIndex()) const override;
	int columnCount(const QModelIndex &parent = QModelIndex()) const override;
	QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;
	QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;
	Qt::ItemFlags flags(const QModelIndex &index) const override;
	bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole) override;
	bool insertRows(int position, int rows, const QModelIndex &parent) override;
	bool removeRows(int position, int rows, const QModelIndex &index) override;

private:
	QString enumToString(UniformTypes type);
	UniformTypes stringToEnum(QString str);
	QVector<float> stringToVector(QString str);

	QVector<Uniform> *uniformVector;
};

#endif