#include "UniformsModel.h"

UniformsModel::UniformsModel(QObject *parent)
	: QAbstractTableModel(parent)
{
}

UniformsModel::~UniformsModel()
{
}

int UniformsModel::rowCount(const QModelIndex &) const
{
	return uniformVector->count();
}

int UniformsModel::columnCount(const QModelIndex &) const
{
	return 3;
}

QVariant UniformsModel::data(const QModelIndex &index, int role) const
{
	if (!index.isValid())
		return QVariant();
	if (role == Qt::TextAlignmentRole)
	{
		return int(Qt::AlignRight | Qt::AlignVCenter);
	}
	else if (role == Qt::DisplayRole)
	{
		Uniform uniform = uniformVector->at(index.row());
		switch (index.column())
		{
		case 0:
			return QVariant::fromValue(uniform.type).value<QString>();
		case 1:
			return uniform.name;
		case 2:
		{
			QString str = "";
			if(!uniform.value.isEmpty())
				str += QString::number(uniform.value[0]);
			for (int i = 1; i < uniform.value.count(); i++)
			{
				str += ", " + QString::number(uniform.value[i]);
			}
			return str;
		}
		}
	}

	return QVariant();
}

QVariant UniformsModel::headerData(int section, Qt::Orientation orientation, int role) const
{
	if (role == Qt::DisplayRole)
	{
		if (orientation == Qt::Horizontal)
		{
			switch (section)
			{
			case 0:
				return QString(tr("Type"));
			case 1:
				return QString(tr("Name"));
			case 2:
				return QString(tr("Value"));
			}
		}
	}
	return QVariant();
}

void UniformsModel::setUniformVector(QVector<Uniform> *vector)
{
	uniformVector = vector;
}

Qt::ItemFlags UniformsModel::flags(const QModelIndex &index) const
{
	if (!index.isValid())
		return Qt::ItemIsEnabled;

	return QAbstractItemModel::flags(index) | Qt::ItemIsEditable;
}

bool UniformsModel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if (!index.isValid())
		return false;

	if (index.row() >= uniformVector->count())
	{
		QVector<float> vector;
		Uniform uniform = { UniformTypes::Float, "", vector };
		uniformVector->push_back(uniform);
	}
		
	switch (index.column())
	{
	case 0:
		(*uniformVector)[index.row()].type = stringToEnum(value.toString());
		break;
	case 1:
		(*uniformVector)[index.row()].name = value.toString();
		break;
	case 2:
		(*uniformVector)[index.row()].value = stringToVector(value.toString());
		break;
	}
	return true;
}

bool UniformsModel::insertRows(int position, int rows, const QModelIndex &parent)
{
	beginInsertRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		QVector<float> vector;
		Uniform uniform = { UniformTypes::Float, "", vector };
		uniformVector->push_back(uniform);
	}

	endInsertRows();
	return true;
}

bool UniformsModel::removeRows(int position, int rows, const QModelIndex &index)
{
	beginRemoveRows(QModelIndex(), position, position + rows - 1);

	for (int row = 0; row < rows; ++row)
	{
		uniformVector->removeAt(position - row);
	}

	endRemoveRows();
	return true;
}

QString UniformsModel::enumToString(UniformTypes type)
{
	int index = metaObject()->indexOfEnumerator("UniformTypes");
	QMetaEnum metaEnum = metaObject()->enumerator(index);
	return metaEnum.valueToKey(type);
}

UniformsModel::UniformTypes UniformsModel::stringToEnum(QString str)
{
	if (str == "Float")
		return UniformsModel::UniformTypes::Float;
	else if (str == "Int")
		return UniformsModel::UniformTypes::Int;
	else if (str == "Vec2")
		return UniformsModel::UniformTypes::Vec2;
	else if (str == "Vec3")
		return UniformsModel::UniformTypes::Vec3;
	else if (str == "Vec4")
		return UniformsModel::UniformTypes::Vec4;
	else if (str == "Mat3")
		return UniformsModel::UniformTypes::Mat3;
	else if (str == "Mat4")
		return UniformsModel::UniformTypes::Mat4;
}

QVector<float> UniformsModel::stringToVector(QString strValue)
{
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

	return values;
}