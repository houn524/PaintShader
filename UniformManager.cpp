#include "UniformManager.h"

UniformManager::UniformManager(QObject *parent)
	: QObject(parent)
{
	uniforms = new QVector<Uniform>();
}

UniformManager::UniformManager(const UniformManager &uniformManager)
{
	const QVector<Uniform>* _uniforms = uniformManager.uniforms;
	this->uniforms = new QVector<Uniform>(*_uniforms);
}

UniformManager::~UniformManager()
{
	delete uniforms;
}

Uniform UniformManager::get(int index) const
{
	return uniforms->at(index);
}

void UniformManager::add(Uniform uniform)
{
	uniforms->push_back(uniform);
}

void UniformManager::insert(QVector<Uniform> uniformVector)
{
	uniforms = new QVector<Uniform>(uniformVector);
}

void UniformManager::remove(int index)
{
	uniforms->remove(index);
}

int UniformManager::count() const
{
	return uniforms->count();
}

void UniformManager::clear()
{
	uniforms = new QVector<Uniform>();
}