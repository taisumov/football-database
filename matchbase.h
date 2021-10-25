#ifndef MATCHBASE_H
#define MATCHBASE_H

#include <QObject>
#include <QSql>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlDatabase>
#include <QFile>
#include <QDate>
#include <QDebug>

/* Директивы имен таблицы, полей таблицы и базы данных */
#define MATCHBASE_HOSTNAME   "MatchBase"
#define MATCHBASE_NAME       "MatchBase.db"

#define PLAYER                  "PLayerTable"
#define PLAYER_NAME             "Name"
#define PLAYER_NUM              "Number"
#define PLAYER_POS              "Position"

class MatchBase : public QObject
{
    Q_OBJECT
public:
    explicit MatchBase(QObject *parent = 0);
    ~MatchBase();
    /* Методы для непосредственной работы с классом
     * Подключение к базе данных и вставка записей в таблицу
     * */
    void connectToMatchBase();
    bool inserIntoMatchTable(const QVariantList &data);

private:
    // Сам объект базы данных, с которым будет производиться работа
    QSqlDatabase    mb;

private:
    /* Внутренние методы для работы с базой данных
     * */
    bool openMatchBase();
    bool restoreMatchBase();
    void closeMatchBase();
    bool createMatchTable();
};

#endif // MATCHBASE_H
