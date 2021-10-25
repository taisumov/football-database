#ifndef DATABASE_H
#define DATABASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define DATABASE_HOSTNAME   "Base"
#define DATABASE_NAME       "Base.db"

#define PLAYER                  "PlayerTable"
#define PLAYER_NAME             "Name"
#define PLAYER_POS              "Position"
#define PLAYER_NUM              "Number"

#define MATCH                   "MatchTable"
#define MATCH_NAME              "Name"
#define MATCH_NUM1              "ScoredGoals"
#define MATCH_NUM2              "MissedGoals"
#define MATCH_DATE              "MatchDate"

class DataBase : public QObject
{
    Q_OBJECT
public:
    explicit DataBase(QObject *parent = 0);
    ~DataBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToDataBase();
    bool insertIntoDeviceTable(const QVariantList &data);
    bool insertIntoMatchTable(const QVariantList &data);

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    db;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openDataBase();
    bool restoreDataBase();
    void closeDataBase();
    bool createDeviceTable();
    bool createMatchTable();
};

#endif // DATABASE_H
