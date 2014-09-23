#include "listmodel.h"
#include <QColor>
#include <QBrush>

ModelData::ModelData()
{
    dateTime = QDateTime::currentDateTime().addSecs(-1);
    name = "new device";
    hours = 0.0;
    reminder = QString();
    infomation = QString();
}

QDataStream &operator<<(QDataStream &out, const ModelData &myObj)
{
    out << myObj.dateTime << myObj.hours <<
           myObj.name << myObj.reminder << myObj.infomation;
    return out;
}

QDataStream &operator>>(QDataStream &in, ModelData &myObj)
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

Qt::ItemFlags Listmodel::flags(const QModelIndex &) const
{
	Qt::ItemFlags flags = Qt::ItemIsEnabled | Qt::ItemIsSelectable | Qt::ItemIsDragEnabled;
	return flags;
}

QVariant Listmodel::data(const QModelIndex &idx, int role) const
{
	if (role == Qt::UserRole)
	{
        QVariant var;
        var.setValue(m_list[idx.row()]);
        return var;
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

//只针对此程序，正常需要前两个形参，我们默认直插入一行至末尾。insertRow函数会调用此函数。
bool Listmodel::insertRows(int , int , const QModelIndex & parent)
{
    int count = m_list.count();
    beginInsertRows(parent, count, count);
    m_list.append(ModelData());
    endInsertRows();
    return true;
}

//删除指定一行
bool Listmodel::removeRows(int row, int , const QModelIndex & parent)
{
    beginRemoveRows(parent, row, row);
    m_list.removeAt(row);
    endRemoveRows();
    return true;
}
