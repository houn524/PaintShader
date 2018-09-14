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
	connect(this, SIGNAL(applyUniforms(QVector<UniformsModel::Uniform>*)), ui->myGLWidget, SLOT(applyUniforms(QVector<UniformsModel::Uniform>*)));
	connect(ui->btnAddTextureUnit, SIGNAL(clicked()), this, SLOT(clickedAddTexture()));
	connect(ui->btnApplyTextureUnits, SIGNAL(clicked()), this, SLOT(clickedApplyTexture()));
	connect(this, SIGNAL(applyTextures(QVector<QString>*)), ui->myGLWidget, SLOT(applyTextures(QVector<QString>*)));

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
	
	uniformModel = new UniformsModel();
	uniforms = new QVector<UniformsModel::Uniform>();
	QVector<float> vec = { -0.7f, -0.8f, -1.0f };
	UniformsModel::Uniform uniform = { UniformsModel::UniformTypes::Vec3, "dirLight.direction", vec };
	uniforms->push_back(uniform);
	vec = { 0.3f, 0.3f, 0.3f };
	uniform = { UniformsModel::UniformTypes::Vec3, "dirLight.ambient", vec };
	uniforms->push_back(uniform);
	vec = { 0.8f, 0.8f, 0.8f };
	uniform = { UniformsModel::UniformTypes::Vec3, "dirLight.diffuse", vec };
	uniforms->push_back(uniform);
	vec = { 0.9f, 0.9f, 0.9f };
	uniform = { UniformsModel::UniformTypes::Vec3, "dirLight.specular", vec };
	uniforms->push_back(uniform);
	vec = { 0 };
	uniform = { UniformsModel::UniformTypes::Int, "material.texture_diffuse", vec };
	uniforms->push_back(uniform);
	vec = { 1 };
	uniform = { UniformsModel::UniformTypes::Int, "material.texture_specular", vec };
	uniforms->push_back(uniform);
	uniformModel->setUniformVector(uniforms);
	ui->myGLWidget->applyUniforms(uniforms);

	ui->table_Uniforms->setModel(uniformModel);
	ui->table_Uniforms->setItemDelegateForColumn(0, new ComboBoxDelegate());
	ui->table_Uniforms->setItemDelegateForColumn(1, new UniformNameLineEditDelegate());
	ui->table_Uniforms->setItemDelegateForColumn(2, new UniformValueLineEditDelegate());
	ui->table_Uniforms->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->table_Uniforms->horizontalHeader()->setStretchLastSection(true);
	ui->table_Uniforms->resizeColumnsToContents();
	ui->table_Uniforms->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->table_Uniforms, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(uniformTableCustomMenuRequested(QPoint)));
	//addUniform(0);

	textureModel = new TextureUnitsModel();
	textures = new QVector<QString>();
	textures->push_back("a");
	textures->push_back("");
	textureModel->setTextureVector(textures);

	ui->table_TextureUnits->setModel(textureModel);
	ui->table_TextureUnits->setItemDelegateForColumn(0, new TextureFileNameDelegate(0));
	ui->table_TextureUnits->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
	ui->table_TextureUnits->resizeColumnsToContents();
	ui->table_TextureUnits->setContextMenuPolicy(Qt::CustomContextMenu);
	connect(ui->table_TextureUnits, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(textureTableCustomMenuRequested(QPoint)));

	vsHighlighter = new Highlighter(ui->txtEdit_VertexShader->document());
	fsHighlighter = new Highlighter(ui->txtEdit_FragmentShader->document());
}

MainWindow::~MainWindow()
{
	delete ui;
	delete VAManager;
	delete uniformModel;
	delete uniforms;
	//delete uniformManager;
	delete textureModel;
	delete textures;
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
			vertices.push_back(data.toFloat());

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
}

void MainWindow::clickedAddUniform()
{
	addUniform(uniformModel->rowCount());
}

void MainWindow::clickedApplyUniforms()
{
	emit applyUniforms(uniforms);
}

void MainWindow::clickedAddTexture()
{
	addTextureUnit();
}

void MainWindow::addTextureUnit()
{
	textureModel->insertRow(textureModel->rowCount());
}

void MainWindow::clickedApplyTexture()
{
	emit applyTextures(textures);
}

void MainWindow::textureTableCustomMenuRequested(QPoint pos)
{
	QModelIndex index = ui->table_TextureUnits->indexAt(pos);

	QMenu* menu = new QMenu(this);
	QAction* actRemove = new QAction(tr("remove"));
	menu->addAction(actRemove);
	menu->popup(ui->table_TextureUnits->viewport()->mapToGlobal(pos));
	connect(actRemove, &QAction::triggered, this, &MainWindow::removeTexture);
}

void MainWindow::uniformTableCustomMenuRequested(QPoint pos)
{
	QModelIndex index = ui->table_Uniforms->indexAt(pos);

	QMenu* menu = new QMenu(this);
	QAction* actRemove = new QAction(tr("remove"));
	menu->addAction(actRemove);
	menu->popup(ui->table_Uniforms->viewport()->mapToGlobal(pos));
	connect(actRemove, &QAction::triggered, this, &MainWindow::removeUniform);
}

void MainWindow::removeTexture()
{
	QModelIndexList indexes = ui->table_TextureUnits->selectionModel()->selectedRows();
	int countRow = indexes.count();

	for (int i = countRow; i > 0; i--)
		textureModel->removeRow(indexes.at(i - 1).row(), QModelIndex());
}

void MainWindow::removeUniform()
{
	QModelIndexList indexes = ui->table_Uniforms->selectionModel()->selectedIndexes();
	int countRow = indexes.count();

	for (int i = countRow; i > 0; i--)
		uniformModel->removeRow(indexes.at(i - 1).row(), QModelIndex());
}