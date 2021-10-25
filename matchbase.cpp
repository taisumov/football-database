#include "matchbase.h"

MatchBase::MatchBase(QObject *parent) : QObject(parent)
{

}

MatchBase::~MatchBase()
{

}

/* Методы для подключения к базе данных
 * */
void MatchBase::connectToMatchBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("C:/Users/mi/Desktop/qdatamapperwidget/" MATCHBASE_NAME).exists()){
        this->restoreMatchBase();
    } else {
        this->openMatchBase();
    }
}

/* Методы восстановления базы данных
 * */
bool MatchBase::restoreMatchBase()
{
    if(this->openMatchBase()){
        if(!this->createMatchTable()){
            return false;
        } else {
            return true;
        }
    } else {
        qDebug() << "Не удалось восстановить базу данных";
        return false;
    }
    return false;
}

/* Метод для открытия базы данных
 * */
bool MatchBase::openMatchBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    mb = QSqlDatabase::addDatabase("QSQLITE");
    mb.setHostName(MATCHBASE_HOSTNAME);
    mb.setDatabaseName("C:/Users/mi/Desktop/qdatamapperwidget/" MATCHBASE_NAME);
    if(mb.open()){
        return true;
    } else {
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void MatchBase::closeMatchBase()
{
    mb.close();
}

/* Метод для создания таблицы устройств в базе данных
 * */
bool MatchBase::createMatchTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " PLAYER " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            PLAYER_NAME  " VARCHAR(255)    NOT NULL,"
                            PLAYER_NUM   " INT     NOT NULL,"
                            PLAYER_POS   " VARCHAR(18)     NOT NULL"
                        " )"
                    )){
        qDebug() << "DataBase: error of create " << PLAYER;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}

/* Метод для вставки записи в таблицу устройств
 * */
bool MatchBase::inserIntoMatchTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " PLAYER " ( " PLAYER_NAME ", "
                                              PLAYER_NUM ", "
                                              PLAYER_POS " ) "
                  "VALUES (:Hostname, :IP, :MAC )");
    query.bindValue(":Hostname",    data[0].toString());
    query.bindValue(":IP",          data[1].toString());
    query.bindValue(":MAC",         data[2].toString());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << PLAYER;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


