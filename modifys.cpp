#include "modifys.h"
#include "ui_modifys.h"

Modifys::Modifys(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Modifys)
{
    ui->setupUi(this);
    qDebug()<<"start";
    if (QSqlDatabase::contains("qt_sql_default_connection"))
    {
       db = QSqlDatabase::database("qt_sql_default_connection");
       qDebug()<<"qt_sql_default_connection_start";
    }
    else
    {
        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("cheq.db");
        qDebug()<<"QSQLITE";
    }
    if(db.open())
    {
        qDebug()<<"Opening Database";
        renew_bank_list();

    }
}

Modifys::~Modifys()
{
    delete ui;
}

void Modifys::on_create_bu_clicked()
{
    if(ui->bank_ed->text() !="" and ui->cheq_w_sp->text() !="" and ui->cheq_h_sp->text() !="")
    {
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "ADD", "ADD BANK CHEQUE ? ",QMessageBox::Yes | QMessageBox::Close);
        if(replay == QMessageBox::Yes)
        {
        qDebug()<<"Upload Bank";
        QString bank = ui->bank_ed->text();
        QString cheq_wt = ui->cheq_w_sp->text();
        QString cheq_ht = ui->cheq_h_sp->text();
        double cheq_w = ((cheq_wt.toDouble())*37.795);
        double cheq_h = ((cheq_ht.toDouble())*37.795);
        //QString zone = "0";
        QSqlQuery query;
        query.prepare("INSERT INTO cheq (id,bank,size_x,size_y) VALUES (NULL,?,?,?)");
        query.addBindValue(bank);
        query.addBindValue(cheq_w);
        query.addBindValue(cheq_h);
        query.exec();
        renew_bank_list();
        ui->bank_ed->clear();
        ui->cheq_w_sp->clear();
        ui->cheq_h_sp->clear();
        qDebug()<<"Upload Data Complete";
        }
     }
}

void Modifys::renew_bank_list()
{
    QStringList bank_items;
    QSqlQuery query;
    query.exec("SELECT bank FROM cheq GROUP BY bank ORDER BY bank");
    while (query.next())
    {
        bank_items<<(query.value(0).toStringList());
    }
    ui->bank_cb->clear();
    ui->bank_cb->addItems(bank_items);
}


void Modifys::on_paynm_bu_clicked()
{
    ui->ittm_nm_ed->setText("Payee_Name");
    QString bank = ui->bank_cb->currentText();
    QSqlQuery query;
    query.prepare("SELECT payto_lm,payto_tm,payto_w,payto_h FROM cheq WHERE bank=? ");
    query.addBindValue(bank);
    query.exec();
    while (query.next())
    {
        lm = query.value(0).toDouble();
        tm = query.value(1).toDouble();
        w = query.value(2).toDouble();
        h = query.value(3).toDouble();
    }
    ui->lm_spin->setValue(lm/37.795);
    ui->top_spin->setValue(tm/37.795);
    ui->w_spin->setValue(w/37.795);
    ui->h_spin->setValue(h/37.795);
}


void Modifys::on_cheq_bu_clicked()
{
    ui->ittm_nm_ed->setText("Cheque_Amount");
    QString bank = ui->bank_cb->currentText();
    QSqlQuery query;
    query.prepare("SELECT cheque_lm,cheque_tm,cheque_w,cheque_h FROM cheq WHERE bank=? ");
    query.addBindValue(bank);
    query.exec();
    while (query.next())
    {
        lm = query.value(0).toDouble();
        tm = query.value(1).toDouble();
        w = query.value(2).toDouble();
        h = query.value(3).toDouble();
    }
    ui->lm_spin->setValue(lm/37.795);
    ui->top_spin->setValue(tm/37.795);
    ui->w_spin->setValue(w/37.795);
    ui->h_spin->setValue(h/37.795);
}


void Modifys::on_amount_bu_clicked()
{
   ui->ittm_nm_ed->setText("Amount");
   QString bank = ui->bank_cb->currentText();
   QSqlQuery query;
   query.prepare("SELECT amount_lm,amount_tm,amount_w,amount_h FROM cheq WHERE bank=? ");
   query.addBindValue(bank);
   query.exec();
   while (query.next())
   {
       lm = query.value(0).toDouble();
       tm = query.value(1).toDouble();
       w = query.value(2).toDouble();
       h = query.value(3).toDouble();
   }
   ui->lm_spin->setValue(lm/37.795);
   ui->top_spin->setValue(tm/37.795);
   ui->w_spin->setValue(w/37.795);
   ui->h_spin->setValue(h/37.795);
}


void Modifys::on_bearer_bu_clicked()
{
   ui->ittm_nm_ed->setText("Bearer");
   QString bank = ui->bank_cb->currentText();
   QSqlQuery query;
   query.prepare("SELECT bearer_lm,bearer_tm,bearer_w,bearer_h FROM cheq WHERE bank=? ");
   query.addBindValue(bank);
   query.exec();
   while (query.next())
   {
       lm = query.value(0).toDouble();
       tm = query.value(1).toDouble();
       w = query.value(2).toDouble();
       h = query.value(3).toDouble();
   }
   ui->lm_spin->setValue(lm/37.795);
   ui->top_spin->setValue(tm/37.795);
   ui->w_spin->setValue(w/37.795);
   ui->h_spin->setValue(h/37.795);
}


void Modifys::on_date_bu_clicked()
{
  ui->ittm_nm_ed->setText("Date");
  QString bank = ui->bank_cb->currentText();
  QSqlQuery query;
  query.prepare("SELECT date_lm,date_tm,date_w,date_h FROM cheq WHERE bank=? ");
  query.addBindValue(bank);
  query.exec();
  while (query.next())
  {
      lm = query.value(0).toDouble();
      tm = query.value(1).toDouble();
      w = query.value(2).toDouble();
      h = query.value(3).toDouble();
  }
  ui->lm_spin->setValue(lm/37.795);
  ui->top_spin->setValue(tm/37.795);
  ui->w_spin->setValue(w/37.795);
  ui->h_spin->setValue(h/37.795);

}


void Modifys::on_acc_bu_clicked()
{
  ui->ittm_nm_ed->setText("ACCOUNT_PAYEE_ONLY");
  QString bank = ui->bank_cb->currentText();
  QSqlQuery query;
  query.prepare("SELECT only_lm,only_tm,only_w,only_h FROM cheq WHERE bank=? ");
  query.addBindValue(bank);
  query.exec();
  while (query.next())
  {
      lm = query.value(0).toDouble();
      tm = query.value(1).toDouble();
      w = query.value(2).toDouble();
      h = query.value(3).toDouble();
  }
  ui->lm_spin->setValue(lm/37.795);
  ui->top_spin->setValue(tm/37.795);
  ui->w_spin->setValue(w/37.795);
  ui->h_spin->setValue(h/37.795);
}


void Modifys::on_upload_clicked()
{
    QString bank = ui->bank_cb->currentText();
    QString lm_n = ui->lm_spin->text();
    double lm = lm_n.toDouble() *37.795;
    QString tm_n = ui->top_spin->text();
    double tm = tm_n.toDouble() *37.795;
    QString w_n = ui->w_spin->text();
    double w = w_n.toDouble() *37.795;
    QString h_n = ui->h_spin->text();
    double h = h_n.toDouble() *37.795;
    QString item = ui->ittm_nm_ed->text();
    if (item == "Payee_Name")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET payto_lm=?,payto_tm=?,payto_w=?,payto_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
    if (item == "Cheque_Amount")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET cheque_lm=?,cheque_tm=?,cheque_w=?,cheque_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
    if (item == "Amount")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET amount_lm=?,amount_tm=?,amount_w=?,amount_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
    if (item == "Bearer")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET bearer_lm=?,bearer_tm=?,bearer_w=?,bearer_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
    if (item == "Date")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET date_lm=?,date_tm=?,date_w=?,date_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
    if (item == "ACCOUNT_PAYEE_ONLY")
    {
        QSqlQuery query;
        query.prepare("UPDATE cheq SET only_lm=?,only_tm=?,only_w=?,only_h=? WHERE bank=? ");
        query.addBindValue(lm);
        query.addBindValue(tm);
        query.addBindValue(w);
        query.addBindValue(h);
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Upload Data Complete";
        clearbu();
    }
}

void Modifys::clearbu()
{
    ui->lm_spin->clear();
    ui->top_spin->clear();
    ui->w_spin->clear();
    ui->h_spin->clear();
    ui->ittm_nm_ed->clear();
    ui->bank_ed->clear();
    ui->cheq_w_sp->clear();
    ui->cheq_h_sp->clear();
}

void Modifys::on_delete_bu_clicked()
{
    if(db.open())
    {
        qDebug()<<"Delete Data";
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "Delete Bank INFO", "Delete ? ",QMessageBox::Yes | QMessageBox::Close);
        if(replay == QMessageBox::Yes)
        {
        QString bank = ui->bank_cb->currentText();
        QSqlQuery query;
        query.prepare("DELETE FROM cheq WHERE bank =? ");
        query.addBindValue(bank);
        query.exec();
        qDebug()<<"Delete Data Complete";
        renew_bank_list();

        }
    }
}

