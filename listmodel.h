#include <QAbstractListModel>
#include <QDateTime>

class ModelData
{
public:
    ModelData();
    QDateTime dateTime;
    double hours;
    QString name;
    QString reminder;
    QString infomation;
    friend QDataStream &operator<<(QDataStream &out, const ModelData &myObj);
    friend QDataStream &operator>>(QDataStream &in, ModelData &myObj);
};
Q_DECLARE_METATYPE(ModelData)

class Listmodel : public QAbstractListModel
{
	Q_OBJECT
public:
    explicit Listmodel(const int row = 0, QObject * parent = 0);
	~Listmodel();
	virtual int rowCount(const QModelIndex & parent = QModelIndex()) const;
	virtual Qt::ItemFlags flags(const QModelIndex &index) const;
	virtual QVariant data(const QModelIndex &idx, int role = Qt::DisplayRole) const;
	virtual bool setData(const QModelIndex &index, const QVariant &value, int role = Qt::EditRole);
    virtual bool insertRows(int row, int count, const QModelIndex & parent = QModelIndex());
    virtual bool removeRows(int row, int count, const QModelIndex & parent = QModelIndex());

public:
    QList<ModelData> m_list;
	
};
