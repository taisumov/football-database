#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlTableModel>

#include <database.h>
#include <dialogadddevice.h>
#include <dialogaddmatch.h>
namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_addDeviceButton_clicked();
    void on_sortButton_clicked();
    void slotUpdateModels();
    /* К СЛОТу по редактировнию записи
     * добавляем СЛОТ по удалению записи.
     * Также добавляем СЛОТ для обработки вызова контекстного меню
     * */
    void slotEditRecord();
    void slotRemoveRecord();
    void slotCustomMenuRequested(QPoint pos);


    void on_addMatchButton_clicked();
    void slotUpdateModelsMatch();
    /* К СЛОТу по редактировнию записи
     * добавляем СЛОТ по удалению записи.
     * Также добавляем СЛОТ для обработки вызова контекстного меню
     * */
    void slotEditRecordMatch();
    void slotRemoveRecordMatch();
    void slotCustomMenuRequestedMatch(QPoint pos);


    void on_tab_customContextMenuRequested(const QPoint &pos);

private:
    Ui::MainWindow              *ui;
    DataBase                    *db;
    QSqlTableModel              *modelDevice;
    QSqlTableModel              *modelMatch;

private:
    void setupModel(const QString &tableName, const QStringList &headers);
    void createUI();

    void setupModelMatch(const QString &tableName, const QStringList &headers);
    void createUIMatch();
};

#endif // MAINWINDOW_H
