#include "TextureFileNameDelegate.h"

TextureFileNameDelegate::TextureFileNameDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

TextureFileNameDelegate::~TextureFileNameDelegate()
{
}

QWidget* TextureFileNameDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	qDebug() << "createEditor";

	QLineEdit* editor = new QLineEdit(parent);
	return editor;
}

void TextureFileNameDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
	QString filename = QFileDialog::getOpenFileName(0, "Open", QDir::currentPath(), "All files (*.*)");
	QLineEdit* txtEdit = static_cast<QLineEdit*>(editor);
	if (!filename.isEmpty())
		txtEdit->setText(filename);
	else
		txtEdit->setText(index.data().toString());
}

void TextureFileNameDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit* txtEdit = static_cast<QLineEdit*>(editor);
	model->setData(index, txtEdit->text(), Qt::EditRole);
}

void TextureFileNameDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}