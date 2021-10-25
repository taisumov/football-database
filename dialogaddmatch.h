#ifndef DIALOGADDMATCH_H
#define DIALOGADDMATCH_H

#include <QDialog>
#include <QSqlTableModel>
#include <QDataWidgetMapper>
#include <QMessageBox>

#include <database.h>

namespace Ui {
class DialogAddMatch;
}

class DialogAddMatch : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAddMatch(int row = -1, QWidget *parent = 0);
    ~DialogAddMatch();

signals:
    void signalReadyMatch();

private slots:
    void on_buttonBoxMatch_accepted();
    void updateButtonsMatch(int row);

private:
    Ui::DialogAddMatch          *ui;
    QSqlTableModel              *model;
    QDataWidgetMapper           *mapper;

private:
    void setupModelMatch();
    void createUI();
    void acceptMatch();
};

#endif // DIALOGADDMATCH_H
