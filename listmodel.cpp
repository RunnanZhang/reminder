#include "listmodel.h"
#include <QColor>
#include <QBrush>

friend QDataStream &operator<<(QDataStream &out, const ModelData &myObj)
{
    out << myObj.dateTime << myObj.hours <<
           myObj.name << myObj.reminder << myObj.infomation;
    return out;
}

friend QDataStream &operator>>(QDataStream &in, ModelData &myObj)
{
    in >> myObj.dateTime >> myObj.hours >>
           myObj.name >> myObj.reminder >> myObj.infomation;
    return in;
}

Listmodel::Listmodel(const int row, QObject * parent) : QAbstractListModel(parent)
{
    for (int i = 0; i < row; ++i)
	{
        m_list.append(ModelData());
	}
}

Listmodel::~Listmodel(void)
{
}

int Listmodel::rowCount(const QModelIndex &) const
{
	return m_list.count();
}

Qt::ItemFlags Listmodel::flags(const QModelIndex &index) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
	return flags;
}

QVariant Listmodel::data(const QModelIndex &idx, int role) const
{
	if (role == Qt::UserRole)
	{
        return QVariant(m_list[idx.row()]);
	}
	else if (role == Qt::DisplayRole)
	{
        return QVariant(m_list[idx.row()].name);
	}
	return QVariant();
}

bool Listmodel::setData(const QModelIndex &index, const QVariant &value, int role)
{
	if(!index.isValid())
		return false;
	QAbstractListModel::setData(index, value, role);
	if (role == Qt::UserRole)
	{
        m_list[index.row()] = value.value<ModelData>();
	}
	else if (role == Qt::DisplayRole)
	{
        m_list[index.row()].name = value.toString();
	}
	return true;
}

bool Listmodel::insertRow (int row, const QModelIndex &parent)
{
    m_list.append(ModelData());
	return QAbstractListModel::insertRow(row,parent);
}
