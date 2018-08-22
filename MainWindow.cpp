#include "MainWindow.h"
#include <qfiledialog.h>
#include <qmessagebox.h>
#include <qtextstream.h>
#include <qfontdialog.h>
#include "AnalogClock.h"
#include "VertexAttributeLayout.h"
#include <QDebug>

MainWindow::MainWindow(QWidget *parent)
	: QMainWindow(parent), ui(new Ui::MainWindow)
{
	QTranslator *translator = new QTranslator();
	translator->load("paintshader_ko.qm");
	QApplication::installTranslator(translator);
	ui->setupUi(this);
	ui->viewShaderSplitter->setStretchFactor(0, 3);
	ui->viewShaderSplitter->setStretchFactor(1, 2);
	ui->mainSplitter->setStretchFactor(0, 3);
	ui->mainSplitter->setStretchFactor(1, 2);
	
	//ui->layoutsTab->tabBar()->setTabsClosable(true);
	connect(ui->myGLWidget, SIGNAL(sigLoadedShaders(QString, QString)), this, SLOT(setShaderCode(QString, QString)));
	connect(ui->btnApplyShaders, SIGNAL(clicked()), this, SLOT(clickedApplyShaders()));
	connect(this, SIGNAL(clickedApplyShaders(QString, QString)), ui->myGLWidget, SLOT(applyShaders(QString, QString)));
	connect(ui->btnApplyVertexBuffer, SIGNAL(clicked()), this, SLOT(clickedApplyVertexAttribute()));
	connect(this, SIGNAL(clickedApplyVertexAttribute(int, QVector<int>, std::vector<float>)), ui->myGLWidget, SLOT(applyVertexBuffer(int, QVector<int>, std::vector<float>)));
	connect(ui->layoutsTab, SIGNAL(tabCloseRequested(int)), this, SLOT(clickedCloseLayoutTab(int)));
	connect(ui->btnAddUniform, SIGNAL(clicked()), this, SLOT(clickedAddUniform()));
	connect(ui->btnApplyUniforms, SIGNAL(clicked()), this, SLOT(clickedApplyUniforms()));
	connect(this, SIGNAL(applyUniforms(UniformManager)), ui->myGLWidget, SLOT(applyUniforms(UniformManager)));

	ui->layoutsTab->removeTab(0);
	ui->layoutsTab->removeTab(0);

	VertexAttributeLayout *VALayout = new VertexAttributeLayout;
	QWidget *widget = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(widget);
	layout->addWidget(VALayout);

	ui->layoutsTab->addTab(widget, "Layout 0");
	
	VAManager = new VertexAttributeManager;
	VAManager->add(VALayout->getSizeWidget(), VALayout->getDataWidget());
	
	QToolButton *tb = new QToolButton();
	tb->setText("+");

	ui->layoutsTab->addTab(new QLabel("Add tabs by pressing \"+\""), QString());
	ui->layoutsTab->setTabEnabled(1, false);
	ui->layoutsTab->tabBar()->setTabButton(1, QTabBar::RightSide, tb);
	connect(tb, SIGNAL(clicked()), this, SLOT(clickedAddLayout()));
	
	uniformModel = new QStandardItemModel();
	uniformModel->setHorizontalHeaderItem(0, new QStandardItem(tr("Type")));
	uniformModel->setHorizontalHeaderItem(1, new QStandardItem(tr("Name")));
	uniformModel->setHorizontalHeaderItem(2, new QStandardItem(tr("Value")));

	ui->table_Uniforms->setModel(uniformModel);
	ui->table_Uniforms->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->table_Uniforms->horizontalHeader()->setStretchLastSection(true);
	
	ui->table_Uniforms->resizeColumnsToContents();
	addUniform(0);

	uniformManager = new UniformManager();

	vsHighlighter = new Highlighter(ui->txtEdit_VertexShader->document());
	fsHighlighter = new Highlighter(ui->txtEdit_FragmentShader->document());
}

MainWindow::~MainWindow()
{
	delete ui;
	delete VAManager;
	delete uniformModel;
	delete uniformManager;
}

void MainWindow::setShaderCode(QString vsCode, QString fsCode)
{
	ui->txtEdit_VertexShader->setText(vsCode);
	ui->txtEdit_FragmentShader->setText(fsCode);
}

void MainWindow::keyPressEvent(QKeyEvent *e)
{
	if (e->key() == Qt::Key_Escape)
		close();
	else
		QWidget::keyPressEvent(e);
}

void MainWindow::clickedApplyShaders()
{
	emit clickedApplyShaders(ui->txtEdit_VertexShader->toPlainText(), ui->txtEdit_FragmentShader->toPlainText());
}

void MainWindow::clickedApplyVertexAttribute()
{
	QString data;
	QStringList dataList;
	QVector<int> sizes;

	for (int i = 0; i < VAManager->count(); i++)
	{
		sizes.push_back(VAManager->get(i).size->text().toInt());
	}

	std::vector<std::vector<float>> vertexSet;

	for (int i = 0; i < VAManager->count(); i++)
	{
		data = VAManager->get(i).data->toPlainText();
		data.simplified();
		data.trimmed();
		data.replace(" ", "");
		data.replace("f", "");
		data.replace("\t", "");
		data.replace("\n", "");
		dataList = data.split(",");

		std::vector<float> vertices;
		foreach(const QString &data, dataList)
		{
			qDebug() << data.toFloat();
			vertices.push_back(data.toFloat());
		}
		vertexSet.push_back(vertices);
	}

	int count = vertexSet[0].size() / sizes[0];
	std::vector<float> resultVertices;
	for (int i = 0; i < count; i++)
	{
		for (int j = 0; j < sizes.count(); j++)
		{
			resultVertices.insert(resultVertices.end(), vertexSet[j].begin() + sizes[j] * i, vertexSet[j].begin() + sizes[j] * i  + sizes[j]);
		}
	}

	for (int i = 0; i < resultVertices.size(); i++)
		qDebug() << "vertices[" << i << "] : " << resultVertices[i];

	emit clickedApplyVertexAttribute(VAManager->count(), sizes, resultVertices);
}

void MainWindow::clickedAddLayout()
{
	VertexAttributeLayout *VALayout = new VertexAttributeLayout;
	QWidget *widget = new QWidget;
	QVBoxLayout *layout = new QVBoxLayout(widget);
	layout->addWidget(VALayout);

	ui->layoutsTab->addTab(widget, QStringLiteral("Layout %1").arg(VAManager->count()));

	VAManager->add(VALayout->getSizeWidget(), VALayout->getDataWidget());

	ui->layoutsTab->tabBar()->moveTab(VAManager->count(), VAManager->count() - 1);
	ui->layoutsTab->setCurrentIndex(VAManager->count() - 1);
}

void MainWindow::clickedCloseLayoutTab(int index)
{
	if (VAManager->count() <= 1)
		return;
	if (ui->layoutsTab->currentIndex() == index)
		ui->layoutsTab->setCurrentIndex(index - 1);
	ui->layoutsTab->removeTab(index);

	for (int i = index; i < VAManager->count() - 1; i++)
		ui->layoutsTab->setTabText(i, QStringLiteral("Layout %1").arg(i));

	
	VAManager->remove(index);
}

void MainWindow::addUniform(int row)
{
	uniformModel->insertRow(row);
	ui->table_Uniforms->setItemDelegateForColumn(0, new ComboBoxDelegate());
	ui->table_Uniforms->setItemDelegateForColumn(1, new UniformNameLineEditDelegate());
	ui->table_Uniforms->setItemDelegateForColumn(2, new UniformValueLineEditDelegate());
	uniformModel->setData(uniformModel->index(uniformModel->rowCount() - 1, 0, QModelIndex()), "float");
	uniformModel->setData(uniformModel->index(uniformModel->rowCount() - 1, 0), Qt::AlignCenter, Qt::TextAlignmentRole);
	uniformModel->setData(uniformModel->index(uniformModel->rowCount() - 1, 1), Qt::AlignCenter, Qt::TextAlignmentRole);
	uniformModel->setData(uniformModel->index(uniformModel->rowCount() - 1, 2), Qt::AlignCenter, Qt::TextAlignmentRole);
}

void MainWindow::clickedAddUniform()
{
	addUniform(uniformModel->rowCount());
}

void MainWindow::clickedApplyUniforms()
{

	int rowCount = uniformModel->rowCount();
	UniformTypes type;
	QVector<Uniform> uniformVector;
	UniformManager uniformManager;
	for (int i = 0; i < rowCount; i++)
	{
		QString strType = uniformModel->data(uniformModel->index(i, 0)).toString();
		if (strType == "float") type = UniformTypes::_FLOAT;
		else if (strType == "int") type = UniformTypes::_INT;
		else if (strType == "vec2") type = UniformTypes::VEC2;
		else if (strType == "vec3") type = UniformTypes::VEC3;
		else if (strType == "vec4") type = UniformTypes::VEC4;
		else if (strType == "mat3") type = UniformTypes::MAT3;
		else if (strType == "mat4") type = UniformTypes::MAT4;

		QString strName = uniformModel->data(uniformModel->index(i, 1)).toString();
		if (strName.isEmpty())
		{
			QMessageBox::warning(this, tr("warnning"), tr("Uniform Name Empty"));
			return;
		}
		
		QString strValue = uniformModel->data(uniformModel->index(i, 2)).toString();
		if (strValue.isEmpty())
		{
			QMessageBox::warning(this, tr("warnning"), tr("Uniform Value Empty"));
			return;
		}
		QStringList strValueList;

		strValue.simplified();
		strValue.trimmed();
		strValue.replace(" ", "");
		strValue.replace("f", "");
		strValue.replace("\t", "");
		strValue.replace("\n", "");
		strValueList = strValue.split(",");
		QVector<float> values;
		foreach(const QString &strValue, strValueList)
		{
			values.push_back(strValue.toFloat());
		}

		Uniform uniform = { type, strName, values };
		qDebug() << type << ", " << values[1] << ", " << strName;
		uniformVector.push_back(uniform);
	}
	//uniformManager->clear();
	uniformManager.insert(uniformVector);

	emit applyUniforms(uniformManager);
}