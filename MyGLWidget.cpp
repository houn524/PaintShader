#include "MyGLWidget.h"
#include <QtDebug>

bool firstMousePress = true;
bool rightButton = false;
float init_vertices[] = {
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
	0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
	0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
	-0.5f,  0.5f, -0.5f,  0.0f, 1.0f
};
MyGLWidget::MyGLWidget(QWidget *parent)
	: QGLWidget(QGLFormat(QGL::SampleBuffers), parent)
{
	//initializeGL();
	setFocusPolicy(Qt::FocusPolicy::StrongFocus);
	startTimer(1000 / 60);

	camera = new Camera(glm::vec3(0.0f, 0.0f, 10.0f));
	uniformManager = new UniformManager();
	/*QGLFormat format;
	format.setDepthBufferSize(24);
	format.setVersion(4, 5);
	format.setSamples(4);
	format.setProfile(QGLFormat::OpenGLContextProfile::CoreProfile);
	setFormat(format);
	create();*/

	//this->context.makeCurrent();
	
}

MyGLWidget::~MyGLWidget()
{
	VAO->deleteLater();
	glDeleteBuffers(1, &VBO);
	delete camera;
}

QSize MyGLWidget::minimumSizeHint() const
{
	return QSize(50, 50);
}

QSize MyGLWidget::sizeHint() const
{
	return QSize(400, 400);
}

static void qNormalizeAngle(int &angle)
{
	while (angle < 0)
		angle += 360 * 16;
	while (angle > 360)
		angle -= 360 * 16;
}

void MyGLWidget::initializeGL()
{
	initializeGLFunctions();

	VAO = new QOpenGLVertexArrayObject(this);
	VAO->create();
	glGenBuffers(1, &VBO);

	int sizes[2] = { 3, 2 };
	setupBuffers(1, sizeof(sizes) / sizeof(int), sizes, sizeof(init_vertices) / sizeof(float), init_vertices);
	setupShaders();

	glEnable(GL_DEPTH_TEST);
	//glEnable(GL_CULL_FACE);
	//glShadeModel(GL_SMOOTH);
	//glEnable(GL_LIGHTING);
	//glEnable(GL_LIGHT0);
	//QMessageBox::warning(this, "주의", "파일을 열 수 없습니다: ");
}

void MyGLWidget::applyVertexBuffer(int layoutCount, QVector<int> sizes, std::vector<float> vertices)
{
	setupBuffers(layoutCount, sizes.count(), sizes.begin(), vertices.size(), &vertices[0]);
	updateGL();
}

void MyGLWidget::setupBuffers(int layoutCount, size_t sizesCount, int* sizes, size_t verticesCount, float* vertices)
{
	VAO->bind();
	glBindBuffer(GL_ARRAY_BUFFER, VBO);

	glBufferData(GL_ARRAY_BUFFER, verticesCount * sizeof(float), vertices, GL_STATIC_DRAW);

	int count = 0;
	for (int i = 0; i < sizesCount; i++)
	{
		count += sizes[i];
	}

	numberOfVertex = (verticesCount * sizeof(float)) / (count * sizeof(float));

	float *offset = 0;
	size_t offsetVal = 0;
	for (int i = 0; i < layoutCount; i++)
	{
		qDebug() << "layout " << i;
		qDebug() << "sizes[" << i << "] : " << sizes[i];
		qDebug() << "sizeof(float) * count : " << sizeof(float) * count;
		glEnableVertexAttribArray(i);
		glVertexAttribPointer(i, sizes[i], GL_FLOAT, GL_FALSE, sizeof(float) * count, (void*)(reinterpret_cast<const void*>(offsetVal)));
		offsetVal += (sizeof(float) * sizes[i]);
		
		//offset = &offsetVal;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void MyGLWidget::setupShaders()
{
	QString vertexShaderCode;
	QString fragmentShaderCode;

	QString vsFileName, fsFileName;
	QFile vsFile, fsFile;

	vsFileName = "Shaders/SimpleVS.glsl";
	fsFileName = "Shaders/SimpleFS.glsl";
	vsFile.setFileName(vsFileName);
	fsFile.setFileName(fsFileName);
	if (!vsFile.open(QIODevice::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "주의", "파일을 열 수 없습니다: " + vsFile.errorString() + vsFileName);
		return;
	}
	if (!fsFile.open(QIODevice::ReadOnly | QFile::Text))
	{
		QMessageBox::warning(this, "주의", "파일을 열 수 없습니다: " + fsFile.errorString());
		return;
	}
	QTextStream vsIn(&vsFile);
	QTextStream fsIn(&fsFile);
	vsCode = vsIn.readAll();
	qDebug() << "vs;; :" << vsCode;
	fsCode = fsIn.readAll();

	QByteArray ba = vsCode.toLatin1();
	const char *c_vsCode = ba.constData();
	QByteArray ba1 = fsCode.toLatin1();
	const char *c_fsCode = ba1.constData();

	vsFile.close();
	fsFile.close();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &c_vsCode, NULL);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID, "VS");

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &c_fsCode, NULL);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID, "FS");
	
	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	emit sigLoadedShaders(vsCode, fsCode);
}

void MyGLWidget::paintGL()
{
	
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);
	glClearColor(0.2, 0.2, 0.2, 1.0);
	draw();
}

void MyGLWidget::resizeGL(int width, int height)
{
	int side = qMin(width, height);
	glViewport(0, 0, width, height);
	this->width = width;
	this->height = height;
}

void MyGLWidget::mousePressEvent(QMouseEvent *event)
{
	keys[event->button()] = true;
}

void MyGLWidget::mouseReleaseEvent(QMouseEvent *event)
{
	keys[event->button()] = false;
}


void MyGLWidget::wheelEvent(QWheelEvent *event)
{
	camera->processMouseScroll(event->delta() / 120);
	updateGL();
}

void MyGLWidget::keyPressEvent(QKeyEvent *event)
{
	keys[event->key()] = true;
	QWidget::keyPressEvent(event);
}

void MyGLWidget::keyReleaseEvent(QKeyEvent *event)
{
	keys[event->key()] = false;
	QWidget::keyReleaseEvent(event);
}

void MyGLWidget::timerEvent(QTimerEvent *event)
{
	QPoint pos = mapFromGlobal(QCursor::pos());
	int dx = pos.x() - lastPos.x();
	int dy = pos.y() - lastPos.y();

	if (keys[Qt::RightButton])
		camera->processMouseMovement(dx, dy, false);

	if (keys[Qt::LeftButton])
		camera->processMouseMovement(dx, dy, true);

	if (keys[Qt::RightButton])
	{
		if (keys[Qt::Key_W])
			camera->processKeyboard(CameraMovement::FORWARD);
		if (keys[Qt::Key_A])
			camera->processKeyboard(CameraMovement::LEFT);
		if (keys[Qt::Key_S])
			camera->processKeyboard(CameraMovement::BACKWARD);
		if (keys[Qt::Key_D])
			camera->processKeyboard(CameraMovement::RIGHT);

		
	}
	updateGL();
	lastPos = pos;
}

void MyGLWidget::draw()
{
	setupUniforms();
	glUseProgram(programID);

	glm::mat4 model = glm::mat4(1.0f);
	model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
	//model = glm::rotate(model, (float)(3.14 / 4), glm::vec3(1.0f, 1.0f, 0.0f));
	
	glm::mat4 view = glm::mat4(1.0f);
	view = camera->getViewMatrix();// glm::lookAt(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glm::mat4 projection = glm::mat4(1.0f);
	projection = glm::perspective<float>(glm::radians(camera->zoom), width / height, 0.1f, 1000.0f);

	glUniformMatrix4fv(glGetUniformLocation(programID, "model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(glGetUniformLocation(programID, "view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(glGetUniformLocation(programID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

	VAO->bind();
	glDrawArrays(GL_TRIANGLES, 0, numberOfVertex);
}

void MyGLWidget::checkCompileErrors(GLuint shader, QString type)
{
	GLint success;
	GLchar infoLog[1024];
	if (type != "PROGRAM")
	{
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 1024, NULL, infoLog);
			QMessageBox::warning(this, "warnning", "ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
		}
	}
	else
	{
		glGetProgramiv(shader, GL_LINK_STATUS, &success);
		if (!success)
		{
			glGetProgramInfoLog(shader, 1024, NULL, infoLog);
			QMessageBox::warning(this, "warnning", "ERROR::SHADER_COMPILATION_ERROR of type: " + type + "\n" + infoLog);
		}
	}
}

void MyGLWidget::applyShaders(QString _vsCode, QString _fsCode)
{
	this->vsCode = _vsCode;
	this->fsCode = _fsCode;

	QByteArray ba = vsCode.toLatin1();
	const char *c_vsCode = ba.constData();
	QByteArray ba1 = fsCode.toLatin1();
	const char *c_fsCode = ba1.constData();

	vertexShaderID = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vertexShaderID, 1, &c_vsCode, NULL);
	glCompileShader(vertexShaderID);
	checkCompileErrors(vertexShaderID, "VS");

	fragmentShaderID = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fragmentShaderID, 1, &c_fsCode, NULL);
	glCompileShader(fragmentShaderID);
	checkCompileErrors(fragmentShaderID, "FS");

	programID = glCreateProgram();
	glAttachShader(programID, vertexShaderID);
	glAttachShader(programID, fragmentShaderID);
	glLinkProgram(programID);

	glDeleteShader(vertexShaderID);
	glDeleteShader(fragmentShaderID);

	updateGL();
}

void MyGLWidget::setupUniforms()
{
	glUseProgram(programID);

	int count = uniformManager->count();
	qDebug() << count;
	for (int i = 0; i < count; i++)
	{
		Uniform uniform = uniformManager->get(i);

		if (uniform.type == UniformTypes::_FLOAT)
			glUniform1f(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), uniform.value[0]);
		else if (uniform.type == UniformTypes::_INT)
			glUniform1i(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), int(uniform.value[0]));
		else if (uniform.type == UniformTypes::VEC3)
			glUniform3fv(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), 1, uniform.value.begin());
		else if (uniform.type == UniformTypes::VEC2)
			glUniform2fv(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), 1, uniform.value.begin());
		else if (uniform.type == UniformTypes::VEC4)
		{
			glUniform4fv(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), 1, uniform.value.begin());
			qDebug() << uniform.value[1];
		}

		else if (uniform.type == UniformTypes::MAT4)
			glUniformMatrix4fv(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), 1, GL_FALSE, uniform.value.begin());
		else if (uniform.type == UniformTypes::MAT3)
			glUniformMatrix3fv(glGetUniformLocation(programID, uniform.name.toStdString().c_str()), 1, GL_FALSE, uniform.value.begin());
	}
}

void MyGLWidget::applyUniforms(const UniformManager &uniformManager)
{
	this->uniformManager = new UniformManager(uniformManager);
	updateGL();
}