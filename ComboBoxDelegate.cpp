#include "ComboBoxDelegate.h"

ComboBoxDelegate::ComboBoxDelegate(QObject *parent)
	: QItemDelegate(parent)
{
}

ComboBoxDelegate::~ComboBoxDelegate()
{
}

QWidget* ComboBoxDelegate::createEditor(QWidget* parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	QComboBox* editor = new QComboBox(parent);
	QStringList itemList;
	itemList << "Float" << "Int" << "Vec2" << "Vec3" << "Vec4" << "Mat3" << "Mat4";
	editor->setEditable(true);
	editor->lineEdit()->setReadOnly(true);
	editor->lineEdit()->setAlignment(Qt::AlignCenter);
	editor->insertItems(0, itemList);
	return editor;
}

void ComboBoxDelegate::setEditorData(QWidget* editor, const QModelIndex &index) const
{
	QAbstractItemDelegate::setEditorData(editor, index);
}

void ComboBoxDelegate::setModelData(QWidget* editor, QAbstractItemModel *model, const QModelIndex &index) const
{
	QComboBox* comboBox = static_cast<QComboBox*>(editor);
	model->setData(index, comboBox->currentText(), Qt::EditRole);
}

void ComboBoxDelegate::updateEditorGeometry(QWidget* editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
	editor->setGeometry(option.rect);
}