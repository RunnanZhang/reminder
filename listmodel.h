#include <QAbstractListModel>

class RSColorBar
{
public:
	RSColorBar(int value, QString name)
	{
		m_Name = name;
		m_Value = value;
	}
	inline QString getName(){return m_Name;}
	inline void setName(QString name){m_Name = name;}
private:
	int m_Value;
	QString m_Name;
};

class Listmodel : public QAbstractListModel
{
	Q_OBJECT
public:
	explicit Listmodel(const int column = 0, QObject * parent = 0);
	~Listmodel();
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
	bool insertRow (int row, const QModelIndex &parent = QModelIndex());

private:
	QList<RSColorBar*> m_list;
	
};