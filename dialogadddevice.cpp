#include "dialogadddevice.h"
#include "ui_dialogadddevice.h"
#include <QDebug>
#include <QRegExpValidator>
DialogAddDevice::DialogAddDevice(int row, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::DialogAddDevice)
{
    ui->setupUi(this);

    /* Метода для инициализации модели,
     * из которой будут транслироваться данные
     * */
    setupModel();

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
        mapper->setCurrentModelIndex(model->index(row,0));
    }

    createUI();
}

DialogAddDevice::~DialogAddDevice()
{
    delete ui;
}

/* Метод настройки модели данных и mapper
 * */
void DialogAddDevice::setupModel()
{
    /* Инициализируем модель и делаем выборку из неё
     * */
    model = new QSqlTableModel(this);
    model->setTable(PLAYER);
    model->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model->select();

    /* Инициализируем mapper и привязываем
     * поля данных к объектам LineEdit
     * */
    mapper = new QDataWidgetMapper();
    mapper->setModel(model);
    mapper->addMapping(ui->NameLineEdit, 1);
    mapper->addMapping(ui->PositionLineEdit, 2);
    mapper->addMapping(ui->GoalsLineEdit, 3);
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
    connect(mapper, SIGNAL(currentIndexChanged(int)), this, SLOT(updateButtons(int)));
}

/* Метод для установки валидатора на поле ввода IP и MAC адресов
 * */
void DialogAddDevice::createUI()
{
    QString ipRange = "(.)";
    QRegExp ipRegex (ipRange);
    QRegExpValidator *ipValidator = new QRegExpValidator(ipRegex, this);
    ui->NameLineEdit->setValidator(ipValidator);
}

void DialogAddDevice::on_buttonBox_accepted()
{
            mapper->submit();
            model->submitAll();
            emit signalReady();
            this->close();
}

void DialogAddDevice::accept()
{
qDebug() << ui->NameLineEdit->text();
}

/* Метод изменения состояния активности кнопок пролистывания
 * */
void DialogAddDevice::updateButtons(int row)
{
    /* В том случае, если мы достигаем одного из крайних (самый первый или
     * самый последний) из индексов в таблице данных,
     * то мы изменяем состояние соответствующей кнопки на
     * состояние неактивна
     * */
    ui->previousButton->setEnabled(row > 0);
    ui->nextButton->setEnabled(row < model->rowCount() - 1);
}
