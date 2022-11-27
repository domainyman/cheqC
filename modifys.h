#ifndef MODIFYS_H
#define MODIFYS_H

#include <QDialog>
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QStringList>
#include <string>

namespace Ui {
class Modifys;
}

class Modifys : public QDialog
{
    Q_OBJECT

public:
    double lm = 0 ;
    double tm = 0;
    double w = 0;
    double h = 0;
    explicit Modifys(QWidget *parent = nullptr);
    ~Modifys();


private slots:
    void on_create_bu_clicked();
    void renew_bank_list();

    void on_paynm_bu_clicked();

    void on_cheq_bu_clicked();

    void on_amount_bu_clicked();

    void on_bearer_bu_clicked();

    void on_date_bu_clicked();

    void on_acc_bu_clicked();

    void on_upload_clicked();

    void clearbu();

    void on_delete_bu_clicked();

private:
    Ui::Modifys *ui;
    QSqlDatabase db;
};

#endif // MODIFYS_H
