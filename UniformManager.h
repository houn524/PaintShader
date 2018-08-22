#pragma once
#ifndef __UNIFORMMANAGER_H__
#define __UNIFORMMANAGER_H__

#include <QObject>
#include <QVector>
#include <glm/glm.hpp>

enum UniformTypes
{
	_FLOAT, _INT, VEC2, VEC3, VEC4, MAT3 ,MAT4
};

Q_DECLARE_METATYPE(UniformTypes)

struct Uniform
{
	UniformTypes type;
	QString name;
	QVector<float> value;
};

Q_DECLARE_METATYPE(Uniform)

class UniformManager : public QObject
{
	Q_OBJECT

private:
	QVector<Uniform>* uniforms;

public:
	UniformManager(QObject *parent = 0);
	UniformManager(const UniformManager &uniformManager);
	~UniformManager();
	Uniform get(int index) const;
	void add(Uniform uniform);
	void insert(QVector<Uniform> uniformVector);
	void remove(int index);
	int count() const;
	void clear();
};

Q_DECLARE_METATYPE(UniformManager)

#endif