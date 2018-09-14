#pragma once
#ifndef __MAINWINDOW_H__
#define __MAINWINDOW_H__

#include <QtWidgets/QMainWindow>
#include <QtWidgets>
#include <QMenu>
#include "ui_MainWindow.h"
#include "SyntaxHighlighter/Highlighter.h"
#include "VertexAttributeManager.h"
#include "ComboBoxDelegate.h"
#include "UniformNameLineEditDelegate.h"
#include "UniformValueLineEditDelegate.h"
//#include "UniformManager.h"
#include "UniformsModel.h"
#include "TextureUnitsModel.h"
#include "TextureFileNameDelegate.h"


class MainWindow : public QMainWindow
{
	Q_OBJECT

public:
	MainWindow(QWidget *parent = Q_NULLPTR);
	~MainWindow();

signals:
	void clickedApplyShaders(QString, QString);
	void clickedApplyVertexAttribute(int, QVector<int>, std::vector<float>);// int count, int sizes[], float* vertices);
	void applyUniforms(QVector<UniformsModel::Uniform>*);
	void applyTextures(QVector<QString>*);

public slots:
	void setShaderCode(QString vsCode, QString fsCode);
	void clickedApplyShaders();
	void clickedApplyVertexAttribute();
	void clickedAddLayout();
	void clickedCloseLayoutTab(int);
	void clickedAddUniform();
	void clickedApplyUniforms();
	void clickedAddTexture();
	void clickedApplyTexture();

	void removeTexture();
	void removeUniform();

	void textureTableCustomMenuRequested(QPoint pos);
	void uniformTableCustomMenuRequested(QPoint pos);
	/*void on_actionNew_triggered();
	void on_actionOpen_triggered();
	void on_actionSave_triggered();
	void on_actionSave_as_triggered();
	void on_actionPrint_triggered();
	void on_actionExit_triggered();
	void on_actionCopy_triggered();
	void on_actionCut_triggered();
	void on_actionPaste_triggered();
	void on_actionUndo_triggered();
	void on_actionRedo_triggered();
	void on_actionFont_triggered();
	void on_actionBold_triggered();
	void on_actionUnderline_triggered();
	void on_actionItalic_triggered();
	void on_actionAbout_triggered();*/

protected:
	void keyPressEvent(QKeyEvent *event);

private:
	void addUniform(int row);
	void addTextureUnit();

private:
	Ui::MainWindow *ui;
	Highlighter *vsHighlighter;
	Highlighter *fsHighlighter;
	VertexAttributeManager *VAManager;
	UniformsModel *uniformModel;
	QVector<UniformsModel::Uniform> *uniforms;
	TextureUnitsModel *textureModel;
	QVector<QString> *textures;
	//UniformManager *uniformManager;
	//QString currentFile;
};

#endif