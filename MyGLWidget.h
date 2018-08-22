#pragma once
#ifndef __MYGLWIDGET_H__
#define __MYGLWIDGET_H__

#include <QGLWidget>
#include <QtWidgets>
#include <QtOpenGL>
#include <QGLFunctions>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <QGuiApplication>
#include <map>
#include "Camera.h"
#include "UniformManager.h"

class MyGLWidget : public QGLWidget, protected QGLFunctions
{
	Q_OBJECT

private:
	float width = 800;
	float height = 600;
	bool pressingRightButton = false;
	QMap<int, bool> keys;

	Camera *camera;
	QPoint lastPos;

	QOpenGLVertexArrayObject *VAO;
	unsigned int VBO, EBO;
	unsigned int vertexShaderID, fragmentShaderID, programID;

	QString vsCode, fsCode;

	UniformManager *uniformManager;

	unsigned int numberOfVertex = 36;

signals:
	void sigLoadedShaders(QString vsCode, QString fsCode);

public slots:
	void applyShaders(QString vsCode, QString fsCode);
	void applyVertexBuffer(int, QVector<int>, std::vector<float>);//int count, int* sizes, float* vertices);
	void applyUniforms(const UniformManager &uniformManager);

public:
	MyGLWidget(QWidget *parent = Q_NULLPTR);
	~MyGLWidget();

	QString getVSCode() { return vsCode; }
	QString getFSCode() { return fsCode; }

protected:
	void initializeGL();
	void paintGL();
	void resizeGL(int width, int height);

	QSize minimumSizeHint() const;
	QSize sizeHint() const;
	void mousePressEvent(QMouseEvent *event);
	void mouseReleaseEvent(QMouseEvent *event);
	void wheelEvent(QWheelEvent *event);
	void keyPressEvent(QKeyEvent *event);
	void keyReleaseEvent(QKeyEvent *event);
	void timerEvent(QTimerEvent *event);

private:
	void draw();
	void setupBuffers(int layoutCount, size_t sizesCount, int* sizes, size_t verticesCount, float* vertices);
	void setupShaders();
	void setupUniforms();

	void checkCompileErrors(GLuint shader, QString type);
};

#endif