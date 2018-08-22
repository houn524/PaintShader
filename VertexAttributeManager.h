#pragma once

#include <QObject>
#include <QVector>
#include <QLineEdit>
#include <QTextEdit>

struct Attribute
{
	QLineEdit *size;
	QTextEdit *data;
};

class VertexAttributeManager : public QObject
{
	Q_OBJECT

private:
	QVector<Attribute> attributes;

	float *vertices;

public:
	int count() const;
	void add(QLineEdit* lnEdit, QTextEdit* txtEdit);
	Attribute get(int);
	void remove(int);
	bool isEmpty();

	VertexAttributeManager(QObject *parent = 0);
	~VertexAttributeManager();
};
