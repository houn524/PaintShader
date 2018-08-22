#include "UniformNameLineEditDelegate.h"

UniformNameLineEditDelegate::UniformNameLineEditDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

UniformNameLineEditDelegate::~UniformNameLineEditDelegate()
{
}

QWidget* UniformNameLineEditDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QLineEdit* editor = new QLineEdit(parent);
	
	return editor;
}

void UniformNameLineEditDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
	QItemDelegate::setEditorData(editor, index);
}

void UniformNameLineEditDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QLineEdit* txtEdit = static_cast<QLineEdit*>(editor);
	model->setData(index, txtEdit->text(), Qt::EditRole);
}

void UniformNameLineEditDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}