#include "UniformValueLineEditDelegate.h"

UniformValueLineEditDelegate::UniformValueLineEditDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

UniformValueLineEditDelegate::~UniformValueLineEditDelegate()
{
}

QWidget* UniformValueLineEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit* editor = new QLineEdit(parent);
	return editor;
}

void UniformValueLineEditDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
	QItemDelegate::setEditorData(editor, index);
}

void UniformValueLineEditDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit* txtEdit = static_cast<QLineEdit*>(editor);
	model->setData(index, txtEdit->text(), Qt::EditRole);
}

void UniformValueLineEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}