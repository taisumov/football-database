#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QListWidgetItem>
MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{

    ui->setupUi(this);

        QString bgImg ="C://Users/mi/Desktop/qdatamapperwidget/ucl.png"; // загрузка с ресурса
           QPixmap bg(bgImg);
           bg = bg.scaled(this->size(), Qt::IgnoreAspectRatio);

           QPalette p;
           p.setBrush(QPalette::Background, bg);
           this->setPalette(p);

    this->setWindowTitle("Администрация футбольного клуба");
    /* Первым делом необходимо создать объект для работы с базой данных
     * и инициализировать подключение к базе данных
     * */
    db = new DataBase();
    db->connectToDataBase();
    this->setupModelMatch(MATCH,
                     QStringList() << trUtf8("id")
                                         << trUtf8("Соперник")
                                         << trUtf8("Забито")
                                         << trUtf8("Пропущено")
                                         << trUtf8("Дата")
               );

    this->setupModel(PLAYER,
                     QStringList() << trUtf8("id")
                                         << trUtf8("Имя игрока")
                                         << trUtf8("Позиция")
                                         << trUtf8("Голы")
               );
    this->createUI();
    this->createUIMatch();
    connect(ui->sortButton, SIGNAL(clicked()), this, SLOT(on_sortButton_clicked()));
}



MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setupModel(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * */
    modelDevice = new QSqlTableModel(this);
    modelDevice->setTable(tableName);
    modelDevice->select();
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelDevice->columnCount(); i++, j++){
        modelDevice->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::setupModelMatch(const QString &tableName, const QStringList &headers)
{
    /* Производим инициализацию модели представления данных
     * */
    modelMatch = new QSqlTableModel(this);
    modelMatch->setTable(tableName);
    modelMatch->select();
    /* Устанавливаем названия колонок в таблице с сортировкой данных
     * */
    for(int i = 0, j = 0; i < modelMatch->columnCount(); i++, j++){
        modelMatch->setHeaderData(i,Qt::Horizontal,headers[j]);
    }
}

void MainWindow::createUI()
{
        ui->deviceTableView->setModel(modelDevice);
        ui->deviceTableView->setColumnHidden(0, true);
        ui->deviceTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->deviceTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->deviceTableView->resizeColumnsToContents();
        ui->deviceTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->deviceTableView->horizontalHeader()->setStretchLastSection(true);

        // Устанавливаем Контекстное Меню
        ui->deviceTableView->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(ui->deviceTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecord()));
        // Подключаем СЛОТ вызова контекстного меню
        connect(ui->deviceTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequested(QPoint)));
}

void MainWindow::createUIMatch()
{
        ui->matchTableView->setModel(modelMatch);

        ui->matchTableView->setColumnHidden(0, true);
        ui->matchTableView->setSelectionBehavior(QAbstractItemView::SelectRows);
        ui->matchTableView->setSelectionMode(QAbstractItemView::SingleSelection);
        ui->matchTableView->resizeColumnsToContents();
        ui->matchTableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->matchTableView->horizontalHeader()->setStretchLastSection(true);

        // Устанавливаем Контекстное Меню
        ui->matchTableView->setContextMenuPolicy(Qt::CustomContextMenu);

        connect(ui->matchTableView, SIGNAL(doubleClicked(QModelIndex)), this, SLOT(slotEditRecordMatch()));
        // Подключаем СЛОТ вызова контекстного меню
        connect(ui->matchTableView, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(slotCustomMenuRequestedMatch(QPoint)));
}

/* Метод для активации диалога добавления записей
 * */
void MainWindow::on_addDeviceButton_clicked()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */
    DialogAddDevice *addDeviceDialog = new DialogAddDevice();
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDeviceDialog->setWindowTitle(trUtf8("Добавить Устройство"));
    addDeviceDialog->exec();
}

void MainWindow::on_sortButton_clicked()
{
    if(ui->sortButton->isChecked())
    {
       ui->matchTableView->setSortingEnabled(true);
    }
    else {
       ui->matchTableView->setSortingEnabled(false);
    }
}
void MainWindow::on_addMatchButton_clicked()
{
    /* Создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных
     * */
    DialogAddMatch *addMatchDialog = new DialogAddMatch();
    connect(addMatchDialog, SIGNAL(signalReadyMatch()), this, SLOT(slotUpdateModelsMatch()));

    /* Выполняем запуск диалогового окна
     * */
    addMatchDialog->setWindowTitle(trUtf8("Добавить Устройство"));
    addMatchDialog->exec();
}

void MainWindow::slotCustomMenuRequested(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editDevice = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteDevice = new QAction(trUtf8("Удалить"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editDevice, SIGNAL(triggered()), this, SLOT(slotEditRecord()));     // Обработчик вызова диалога редактирования
    connect(deleteDevice, SIGNAL(triggered()), this, SLOT(slotRemoveRecord())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(editDevice);
    menu->addAction(deleteDevice);
    /* Вызываем контекстное меню */
    menu->popup(ui->deviceTableView->viewport()->mapToGlobal(pos));
}

void MainWindow::slotCustomMenuRequestedMatch(QPoint pos)
{
    /* Создаем объект контекстного меню */
    QMenu * menu = new QMenu(this);
    /* Создаём действия для контекстного меню */
    QAction * editMatch = new QAction(trUtf8("Редактировать"), this);
    QAction * deleteMatch = new QAction(trUtf8("Удалить"), this);
    /* Подключаем СЛОТы обработчики для действий контекстного меню */
    connect(editMatch, SIGNAL(triggered()), this, SLOT(slotEditRecordMatch()));     // Обработчик вызова диалога редактирования
    connect(deleteMatch, SIGNAL(triggered()), this, SLOT(slotRemoveRecordMatch())); // Обработчик удаления записи
    /* Устанавливаем действия в меню */
    menu->addAction(editMatch);
    menu->addAction(deleteMatch);
    /* Вызываем контекстное меню */
    menu->popup(ui->matchTableView->viewport()->mapToGlobal(pos));
}

void MainWindow::slotRemoveRecord()
{
    int row = ui->deviceTableView->selectionModel()->currentIndex().row();
    /* Выясняем, какая из строк была выбрана
     * */
    /* Проверяем, что строка была действительно выбрана
     * */
    if(row >= 0){
        /* Задаём вопрос, стоит ли действительно удалять запись.
         * При положительном ответе удаляем запись
         * */
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            /* При отрицательном ответе делаем откат действий
             * и закрываем диалог без удаления записи
             * */
            QSqlDatabase::database().rollback();
            return;
        } else {
            /* В противном случае производим удаление записи.
             * При успешном удалении обновляем таблицу.
             * */
            if(!modelDevice->removeRow(row)){
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить запись\n"
                                            "Возможно она используется другими таблицами\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            modelDevice->select();
            ui->deviceTableView->setCurrentIndex(modelDevice->index(-1, -1));
        }
    }
}

void MainWindow::slotRemoveRecordMatch()
{

    int row = ui->matchTableView->selectionModel()->currentIndex().row();
    if(row >= 0){
        if (QMessageBox::warning(this,
                                 trUtf8("Удаление записи"),
                                 trUtf8("Вы уверены, что хотите удалить эту запись?"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::No)
        {
            QSqlDatabase::database().rollback();
            return;
        } else {
            /* В противном случае производим удаление записи.
             * При успешном удалении обновляем таблицу.
             * */
            if(!modelMatch->removeRow(row)){
                QMessageBox::warning(this,trUtf8("Уведомление"),
                                     trUtf8("Не удалось удалить запись\n"
                                            "Возможно она используется другими таблицами\n"
                                            "Проверьте все зависимости и повторите попытку"));
            }
            modelMatch->select();
            ui->matchTableView->setCurrentIndex(modelMatch->index(-1, -1));
        }
    }
}


/* Слот обновления модели представления данных
 * */
void MainWindow::slotUpdateModels()
{
    modelDevice->select();
    ui->deviceTableView->resizeColumnsToContents();
}

void MainWindow::slotUpdateModelsMatch()
{
    modelMatch->select();
    ui->matchTableView->resizeColumnsToContents();
}

/* Метод для активации диалога добавления записей в режиме редактирования
 * с передачей индекса выбранной строки
 * */
void MainWindow::slotEditRecord()
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    int x = ui->deviceTableView->selectionModel()->currentIndex().row();
    qDebug() << x;
    DialogAddDevice *addDeviceDialog = new DialogAddDevice(x);
    connect(addDeviceDialog, SIGNAL(signalReady()), this, SLOT(slotUpdateModels()));

    /* Выполняем запуск диалогового окна
     * */
    addDeviceDialog->setWindowTitle(trUtf8("Редактировать"));
    addDeviceDialog->exec();
}

void MainWindow::slotEditRecordMatch()
{
    /* Также создаем диалог и подключаем его сигнал завершения работы
     * к слоту обновления вида модели представления данных, но передаём
     * в качестве параметров строку записи
     * */
    DialogAddMatch *addMatchDialog = new DialogAddMatch(ui->matchTableView->selectionModel()->currentIndex().row());
    connect(addMatchDialog, SIGNAL(signalReadyMatch()), this, SLOT(slotUpdateModelsMatch()));

    /* Выполняем запуск диалогового окна
     * */
    addMatchDialog->setWindowTitle(trUtf8("Редактировать Устройство"));
    addMatchDialog->exec();
}
