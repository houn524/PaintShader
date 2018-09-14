#include "TextureUnitsModel.h"

TextureUnitsModel::TextureUnitsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

TextureUnitsModel::~TextureUnitsModel()
{
}

int TextureUnitsModel::rowCount(const QModelIndex &) const
{
	return textureVector->count();
}

int TextureUnitsModel::columnCount(const QModelIndex &) const
{
	return 1;
}

QVariant TextureUnitsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	//qDebug() << "hphphphpph";
	if (role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignRight | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole)
	{
		return textureVector->at(index.row());
	}

	return QVariant();
}

QVariant TextureUnitsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case 0:
				return QString(tr("File Name"));
			}
		}

		if (orientation == Qt::Vertical)
		{
			return QStringLiteral("GL_TEXTURE%1").arg(section);
		}
	}
	return QVariant();
}

void TextureUnitsModel::setTextureVector(QVector<QString> *vector)
{
	textureVector = vector;
}

Qt::ItemFlags TextureUnitsModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool TextureUnitsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if (index.row() >= textureVector->count())
		textureVector->push_back("");
	(*textureVector)[index.row()] = value.toString();
	return true;
}

bool TextureUnitsModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		textureVector->push_back("");
	}

	endInsertRows();
	return true;
}

bool TextureUnitsModel::removeRows(int position, int rows, const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		textureVector->removeAt(position - row);
	}

	endRemoveRows();
	return true;
}

void TextureUnitsModel::refresh()
{
	emit dataChanged(index(0, 0), index(0, 0));
}