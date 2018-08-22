#include "VertexAttributeManager.h"

VertexAttributeManager::VertexAttributeManager(QObject *parent)
	: QObject(parent)
{
	
}

VertexAttributeManager::~VertexAttributeManager()
{
}

int VertexAttributeManager::count() const
{
	return attributes.count();
}

void VertexAttributeManager::add(QLineEdit* lnEdit, QTextEdit* txtEdit)
{
	Attribute attribute = { lnEdit, txtEdit };
	attributes.push_back(attribute);
}

Attribute VertexAttributeManager::get(int index)
{
	return attributes[index];
}

void VertexAttributeManager::remove(int index)
{
	attributes.remove(index);
}

bool VertexAttributeManager::isEmpty()
{
	return attributes.isEmpty();
}