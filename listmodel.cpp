#include "listmodel.h"
#include <QColor>
#include <QBrush>
Listmodel::Listmodel(const int column, QObject * parent) : QAbstractListModel(parent)
{
	for (int i = 0; i < column; ++i)
	{
		m_list.append(NULL);
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
	//����������ã�������flags�ô�
	if (index.row() == 0)
		flags |= Qt::ItemIsEditable | Qt::ItemIsUserCheckable;
	return flags;
}

QVariant Listmodel::data(const QModelIndex &idx, int role) const
{
	if (role == Qt::UserRole)
	{
 		return QVariant(uint(m_list[idx.row()]));
	}
	else if (role == Qt::DisplayRole)
	{
		return QVariant(!m_list[idx.row()] ? QString() : m_list[idx.row()]->getName());
	}
	//��Ϊ����Ч�����Ա���������
	else if (role == Qt::CheckStateRole)
	{
		return QVariant(true);
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
		m_list[index.row()] = (RSColorBar*)value.toUInt();
	}
	else if (role == Qt::DisplayRole)
	{
		m_list[index.row()]->setName(value.toString());
	}
	//ע�ⲻ�������Ч��
	else if (role == Qt::EditRole)
	{
		m_list[index.row()]->setName(value.toString());
	}
	return true;
}

bool Listmodel::insertRow (int row, const QModelIndex &parent)
{
	m_list.append(NULL);
	return QAbstractListModel::insertRow(row,parent);
}