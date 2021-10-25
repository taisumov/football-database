#include "dialogaddmatch.h"
#include "ui_dialogaddmatch.h"
#include <QDebug>
#include <QRegExpValidator>
DialogAddMatch::DialogAddMatch(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddMatch)
{
    ui->setupUi(this);
    /* Метода для инициализации модели,
     * из которой будут транслироваться данные
     * */
    setupModelMatch();

    /* Если строка не задана, то есть равна -1,
     * тогда диалог работает по принципу создания новой записи.
     * А именно, в модель вставляется новая строка и работа ведётся с ней.
     * */
    if(row == -1){
        model->insertRow(model->rowCount(QModelIndex()));
        mapper->toLast();
    /* В противном случае диалог настраивается на заданную запись
     * */
    } else {
        mapper->setCurrentModelIndex(model->index(row,row));
    }

    createUI();
}

DialogAddMatch::~DialogAddMatch()
{
    delete ui;
}



/* Метод настройки модели данных и mapper
 * */
void DialogAddMatch::setupModelMatch()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(MATCH);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Инициализируем mapper и привязываем
     * поля данных к объектам LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->NameLineEdit, 1);
    mapper->addMapping(ui->ScoredLineEdit, 2);
    mapper->addMapping(ui->MissedLineEdit, 3);
    mapper->addMapping(ui->DateLineEdit, 4);
    /* Ручное подтверждение изменения данных
     * через mapper
     * */
    mapper->setSubmitPolicy(QDataWidgetMapper::ManualSubmit);

    qDebug() << ui->NameLineEdit->text();
    /* Подключаем коннекты от кнопок пролистывания
     * к прилистыванию модели данных в mapper
     * */
    connect(ui->previousButton, SIGNAL(clicked()), mapper, SLOT(toPrevious()));
    connect(ui->nextButton, SIGNAL(clicked()), mapper, SLOT(toNext()));
    /* При изменении индекса в mapper изменяем состояние кнопок
     * */
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtonsMatch(int)));
}


void DialogAddMatch::createUI()
{
}

void DialogAddMatch::on_buttonBoxMatch_accepted()
{
        mapper->submit();
        model->submitAll();
        emit signalReadyMatch();
        this->close();

}

void DialogAddMatch::acceptMatch()
{

qDebug() << ui->NameLineEdit->text();
}

/* Метод изменения состояния активности кнопок пролистывания
 * */
void DialogAddMatch::updateButtonsMatch(int row)
{
    /* В том случае, если мы достигаем одного из крайних (самый первый или
     * самый последний) из индексов в таблице данных,
     * то мы изменяем состояние соответствующей кнопки на
     * состояние неактивна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
