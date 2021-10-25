#include "database.h"
#include <QMessageBox>
DataBase::DataBase(QObject *parent) : QObject(parent)
{

}

DataBase::~DataBase()
{

}

/* Методы для подключения к базе данных
 * */
void DataBase::connectToDataBase()
{
    /* Перед подключением к базе данных производим проверку на её существование.
     * В зависимости от результата производим открытие базы данных или её восстановление
     * */
    if(!QFile("C:/Users/mi/Desktop/qdatamapperwidget/" DATABASE_NAME).exists()){
        this->restoreDataBase();
    } else {
        this->openDataBase();
    }
}

/* Методы восстановления базы данных
 * */
bool DataBase::restoreDataBase()
{
    if(this->openDataBase()){
        if(!this->createDeviceTable() || !this->createMatchTable()){
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
bool DataBase::openDataBase()
{
    /* База данных открывается по заданному пути
     * и имени базы данных, если она существует
     * */
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setHostName(DATABASE_HOSTNAME);
    db.setDatabaseName("C:/Users/mi/Desktop/qdatamapperwidget/" DATABASE_NAME);
    if(db.open()){
        QMessageBox::information(0, "Уведомление", "База данных была успешно открыта!");
        return true;
    } else {
        QMessageBox::information(0, "Уведомление", "Базы данных не обнаружлось, сейчас будет создана новая.");
        return false;
    }
}

/* Методы закрытия базы данных
 * */
void DataBase::closeDataBase()
{
    db.close();
}

/* Метод для создания таблицы устройств в базе данных
 * */
bool DataBase::createDeviceTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " PLAYER " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            PLAYER_NAME  " VARCHAR(255)    NOT NULL,"
                            PLAYER_POS   " VARCHAR(18)     NOT NULL"
                            PLAYER_NUM   " INT     NOT NULL,"
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

bool DataBase::createMatchTable()
{
    /* В данном случае используется формирование сырого SQL-запроса
     * с последующим его выполнением.
     * */
    QSqlQuery query;
    if(!query.exec( "CREATE TABLE " MATCH " ("
                            "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                            MATCH_NAME  " VARCHAR(255)    NOT NULL,"
                            MATCH_NUM1   " INT      NOT NULL,"
                            MATCH_NUM2   " INT      NOT NULL,"
                            MATCH_DATE   " DATE     NOT NULL"
                        " )"
                    )){
        QMessageBox::information(0, "Ошибка", "Что-то пошло не так... Кажется, не получилось создать БД, но зависело не от Вас!)");
        qDebug() << "DataBase: error of create " << MATCH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


/* Метод для вставки записи в таблицу устройств
 * */
bool DataBase::insertIntoMatchTable(const QVariantList &data)
{
    /* Запрос SQL формируется из QVariantList,
     * в который передаются данные для вставки в таблицу.
     * */
    QSqlQuery query;
    /* В начале SQL запрос формируется с ключами,
     * которые потом связываются методом bindValue
     * для подстановки данных из QVariantList
     * */
    query.prepare("INSERT INTO " MATCH " ( "  MATCH_NAME ", "
                                              MATCH_NUM1 ", "
                                              MATCH_NUM2 ", "
                                              MATCH_DATE " ) "
                  "VALUES (:Name, :ScoredGoals, :MissedGoals, :MatchDate )");
    query.bindValue(":Name",                data[0].toString());
    query.bindValue(":ScoredGoals",         data[1].toString());
    query.bindValue(":MissedGoals",         data[2].toString());
    query.bindValue(":MatchDate",           data[3].toString());
    // После чего выполняется запросом методом exec()
    if(!query.exec()){
        qDebug() << "error insert into " << MATCH;
        qDebug() << query.lastError().text();
        return false;
    } else {
        return true;
    }
    return false;
}


bool DataBase::insertIntoDeviceTable(const QVariantList &data)
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
                  "VALUES (:Name, :Position, :Goals )");
    query.bindValue(":Name",    data[0].toString());
    query.bindValue(":Position",          data[1].toString());
    query.bindValue(":Goals",         data[2].toString());
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

