#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "iostream"

using namespace std;

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    qDebug()<<"start";
    QDoubleValidator* amountValidator = new QDoubleValidator(this);
    amountValidator->setNotation(QDoubleValidator::StandardNotation);
    amountValidator->setDecimals(2);
    ui->amount_ed->setValidator(amountValidator);
    ui->date_dateEdit->setDate(date);
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("cheq.db");
    if (db.open())
    {
        qDebug()<<"Opening Database";
        QSqlQuery query;
        if(db.open())
        {
            query.exec("SELECT count(*) FROM sqlite_master WHERE type='table' AND name='lib'");
            while (query.next())
            {
                check_table = query.value(0).toInt();
                qDebug() << check_table;
            }
            if(check_table == 0)
            {
                query.exec("CREATE TABLE lib( "
                           "id INTEGER primary key AUTOINCREMENT NOT NULL,"
                           "Bank TEXT NOT NULL,"
                           "Chequeno TEXT NOT NULL,"
                           "Payee TEXT NOT NULL,"
                           "Amount TEXT NOT NULL,"
                           "Date TEXT NOT NULL,"
                           "Category TEXT NOT NULL)");
                query.exec("CREATE TABLE cheq( "
                           "id INTEGER primary key AUTOINCREMENT,"
                           "bank TEXT NOT NULL,"
                           "size_x REAL NOT NULL,"
                           "size_y REAL NOT NULL,"
                           "payto_lm REAL ,"
                           "payto_tm REAL ,"
                           "payto_w REAL ,"
                           "payto_h REAL ,"
                           "amount_lm REAL ,"
                           "amount_tm REAL ,"
                           "amount_w REAL ,"
                           "amount_h REAL ,"
                           "cheque_lm REAL ,"
                           "cheque_tm REAL ,"
                           "cheque_w REAL ,"
                           "cheque_h REAL ,"
                           "date_lm REAL ,"
                           "date_tm REAL ,"
                           "date_w REAL ,"
                           "date_h REAL ,"
                           "bearer_lm REAL ,"
                           "bearer_tm REAL ,"
                           "bearer_w REAL ,"
                           "bearer_h REAL ,"
                           "only_lm REAL ,"
                           "only_tm REAL ,"
                           "only_w REAL ,"
                           "only_h REAL )");
            }
            else
            {
            tableModel = new QSqlTableModel();
            tableModel->setTable("lib");
            ui->cheqView->setModel(tableModel);
            ui->cheqView->setColumnHidden(0,true);
            ui->cheqView->setEditTriggers(QAbstractItemView::NoEditTriggers);
            ui->cheqView->setSelectionBehavior(QAbstractItemView::SelectRows);
            for( int a = 1; a <= 6; a = a + 1 )
                ui->cheqView->setColumnWidth(a,140);
            tableModel->setHeaderData(1, Qt::Horizontal, "Bank");
            tableModel->setHeaderData(2, Qt::Horizontal, "Cheque No.");
            tableModel->setHeaderData(3, Qt::Horizontal, "Payee");
            tableModel->setHeaderData(4, Qt::Horizontal, "Amount");
            tableModel->setHeaderData(5, Qt::Horizontal, "Date");
            tableModel->setHeaderData(6, Qt::Horizontal, "Remark");
            tableModel->select();
            renew_bank_list();
            QString name_pri = info.defaultPrinterName();
            ui->print_ed->setText(name_pri);
            QStringList payee_list = pay_list();
            StringComplater = new QCompleter(payee_list,this);
            StringComplater->setCaseSensitivity(Qt::CaseInsensitive);
            ui->payto_ed->setCompleter(StringComplater);
            ui->ac_only_check->setChecked(true);
            ui->bearer_check->setChecked(true);

            }
        }
    }
    else {
        qDebug()<<"Database Error";
    }

    qDebug()<<"end";
}

MainWindow::~MainWindow()
{
    db.close();
    delete ui;
}

void MainWindow::on_clear_bu_clicked()
{
    ui->cheq_nub_ed->clear();
    ui->payto_ed->clear();
    ui->amount_ed->clear();
    ui->remark_ed->clear();
    qDebug()<<"Clear Edit";
}

void MainWindow::on_save_bu_clicked()
{
    if(db.open() and ui->cheq_nub_ed->text() !="" and ui->payto_ed->text() !="" and ui->amount_ed->text() !="")
    {
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "ADD", "ADD ? ",QMessageBox::Yes | QMessageBox::Close);
        if(replay == QMessageBox::Yes)
        {
            qDebug()<<"Upload Data";
            QString bank = ui->bank_combox->currentText();
            QString cheqno = ui->cheq_nub_ed->text();
            QString payee = ui->payto_ed->text();
            QString amount = ui->amount_ed->text();
            QString dat = ui->date_dateEdit->text();
            QString categ = ui->remark_ed->text();
            QSqlQuery query;
            query.prepare("INSERT INTO lib (id,Bank,Chequeno,Payee,Amount,Date,Category)VALUES (NULL,?,?,?,?,?,?)");
            query.addBindValue(bank);
            query.addBindValue(cheqno);
            query.addBindValue(payee);
            query.addBindValue(amount);
            query.addBindValue(dat);
            query.addBindValue(categ);
            query.exec();
            tableModel->select();
            qDebug()<<"Upload Data Complete";
            on_print_save_bu_clicked();
            on_clear_bu_clicked();
        }
    }
}

void MainWindow::on_cheqView_activated()
{
    int curosor = ui->cheqView->currentIndex().row();
    qDebug()<<curosor;
    ui->cheq_nub_ed->setText(ui->cheqView->model()->index(curosor , 2).data().toString());
    ui->payto_ed->setText(ui->cheqView->model()->index(curosor , 3).data().toString());
    ui->amount_ed->setText(ui->cheqView->model()->index(curosor , 4).data().toString());
    ui->remark_ed->setText(ui->cheqView->model()->index(curosor , 6).data().toString());
    qDebug()<<"Reload Data Complete";
}


void MainWindow::on_delete_bu_clicked()
{
    if(db.open() and ui->cheq_nub_ed->text() !="")
    {
        qDebug()<<"Delete Data";
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "Delete", "Delete  ? ",QMessageBox::Yes | QMessageBox::Close);
        if(replay == QMessageBox::Yes)
        {
        QString bank = ui->bank_combox->currentText();
        QString cheqno = ui->cheq_nub_ed->text();
        QString payee = ui->payto_ed->text();
        QString amount = ui->amount_ed->text();
        QSqlQuery query;
        query.prepare("DELETE FROM lib WHERE Bank =? and Chequeno=? and Payee=? and Amount=? ");
        query.addBindValue(bank);
        query.addBindValue(cheqno);
        query.addBindValue(payee);
        query.addBindValue(amount);
        query.exec();
        tableModel->select();
        qDebug()<<"Delete Data Complete";
        on_clear_bu_clicked();
        }
    }
}


void MainWindow::renew_bank_list()
{
    QStringList bank_items;
    QSqlQuery query;
    query.exec("SELECT bank FROM cheq GROUP BY bank ORDER BY bank");
    while (query.next())
    {
        bank_items<<(query.value(0).toStringList());
    }
    ui->bank_combox->clear();
    ui->bank_combox->addItems(bank_items);
}

void MainWindow::on_bank_setbu_clicked()
{
    modifys = new Modifys(this);
    modifys->show();
}

void MainWindow::on_print_save_bu_clicked()
{
    if(db.open() and ui->cheq_nub_ed->text() !="" and ui->payto_ed->text() !="" and ui->amount_ed->text() !="")
    {
        QString pay_to = ui->payto_ed->text();
        QString amount_text = ui->amount_ed->text();
        double value = amount_text.toDouble();
        QString amount=QString::number(value,'f',2);
        QString date = ui->date_dateEdit->text();
        QString bearer = "XXXXXXX";

        QPrinter imprinter;
        imprinter.setPageOrientation(QPageLayout::Landscape);
        imprinter.setResolution(150);
        imprinter.setPageMargins(QMarginsF(-10, -10, -10, -10));
        imprinter.setPageSize(QPageSize(QPageSize::A4));
        auto page = imprinter.pageRect(QPrinter::Unit::Point);
        qDebug()<<page.width()<<page.height();
        //QPrinter imprinter(QPrinter::HighResolution);

        QPrintDialog pd(&imprinter,this);
        if (pd.exec() == QDialog::Rejected)
        {
                return;
        }
        else
        {
        //imprinter = pd.printer();

        QString bank = ui->bank_combox->currentText();

        QPainter painter;
        painter.begin(&imprinter);

        QFont font;
        font.setPointSize(12);
        painter.setFont(font);

        QSqlQuery query;
        query.prepare("SELECT size_x,size_y,payto_lm,payto_tm,payto_w,payto_h,amount_lm,amount_tm,amount_w,amount_h,cheque_lm,cheque_tm,cheque_w,cheque_h,date_lm,date_tm,date_w,date_h,bearer_lm,bearer_tm,bearer_w,bearer_h,only_lm,only_tm,only_w,only_h FROM cheq WHERE bank=? ");
        query.addBindValue(bank);
        query.exec();
        while (query.next())
        {
            size_w = query.value(0).toDouble();
            size_h = query.value(1).toDouble();
            pay_lm = query.value(2).toDouble();
            pay_tm = query.value(3).toDouble();
            pay_w = query.value(4).toDouble();
            pay_h = query.value(5).toDouble();
            am_lm = query.value(6).toDouble();
            am_tm = query.value(7).toDouble();
            am_w = query.value(8).toDouble();
            am_h = query.value(9).toDouble();
            cqam_lm = query.value(10).toDouble();
            cqam_tm = query.value(11).toDouble();
            cqam_w = query.value(12).toDouble();
            cqam_h = query.value(13).toDouble();
            da_lm = query.value(14).toDouble();
            da_tm = query.value(15).toDouble();
            da_w = query.value(16).toDouble();
            da_h = query.value(17).toDouble();
            be_lm = query.value(18).toDouble();
            be_tm = query.value(19).toDouble();
            be_w = query.value(20).toDouble();
            be_h = query.value(21).toDouble();
            on_lm = query.value(22).toDouble();
            on_tm = query.value(23).toDouble();
            on_w = query.value(24).toDouble();
            on_h = query.value(25).toDouble();
        }
        if (ui->printer_settint_cb->currentText() == "Left")
        {
            painter.translate(-10,-10);
        }
        else if (ui->printer_settint_cb->currentText() == "Right" )
        {
            painter.translate(-10,410);
        }else if (ui->printer_settint_cb->currentText() == "Center")
        {
            painter.translate(-10,209.625);
        }
        QRect rect( 0, 0, size_w, size_h );
        painter.fillRect(rect,QColor(255, 255, 255));
        //painter.drawRect(rect);
        painter.drawText(QRectF(pay_lm, pay_tm, pay_w, pay_h), pay_to);
        qDebug()<<pay_lm<<pay_tm;
        if (ui->notamount_check->isChecked())
        {

        }
        else
        {
            painter.drawText(QRectF(am_lm, am_tm,am_w,am_h), "**"+amount+"**");
            qDebug()<<am_lm<<am_tm;
            painter.drawText(QRectF(cqam_lm, cqam_tm, cqam_w, cqam_h), "***"+amount+"***");
            qDebug()<<cqam_lm<<cqam_tm;
        }

        if (ui->notdate_check->isChecked())
        {

        }
        else
        {
            painter.drawText(QRectF(da_lm, da_tm, da_w, da_h), date);
            qDebug()<<da_lm<<da_tm;
        }

        if (ui->bearer_check->isChecked())
        {
            painter.drawText(QRectF(be_lm, be_tm, be_w, be_h), bearer);
            qDebug()<<be_lm<<be_tm;
        }

        if (ui->ac_only_check->isChecked())
        {
            painter.drawPixmap(on_lm, on_tm, on_w, on_h, QPixmap(":/res/acpayeeonly.jpg"));
            qDebug()<<on_lm<<on_tm;
        }

        painter.end();
        }
        on_clear_bu_clicked();

    }

}

void MainWindow::on_upload_bu_clicked()
{
    if(db.open() and ui->cheq_nub_ed->text() !="" and ui->payto_ed->text() !="" and ui->amount_ed->text() !="")
    {
        QMessageBox::StandardButton replay;
        replay = QMessageBox::question(this, "UPLOAD", "UPLOAD ? ",QMessageBox::Yes | QMessageBox::Close);
        if(replay == QMessageBox::Yes)
        {
            qDebug()<<"Upload Data";
            QString cheqno = ui->cheq_nub_ed->text();
            QString payee = ui->payto_ed->text();
            QString amount = ui->amount_ed->text();
            QString dat = ui->date_dateEdit->text();
            QString categ = ui->remark_ed->text();
            QSqlQuery query;
            query.prepare("UPDATE lib SET Payee=?,Amount=?,Date=?,Category=? WHERE Chequeno=? ");
            query.addBindValue(payee);
            query.addBindValue(amount);
            query.addBindValue(dat);
            query.addBindValue(categ);
            query.addBindValue(cheqno);
            query.exec();
            qDebug()<<"Upload Data Complete";
            tableModel->select();
            on_clear_bu_clicked();
        }
    }
}

QStringList MainWindow::pay_list()
{
    QStringList list;
    if(db.open())
    {
        QSqlQuery query;
        query.exec("SELECT Payee FROM lib GROUP BY Payee ORDER BY Payee");
        while (query.next())
        {
            list<<(query.value(0).toStringList());
        }
    }
    qDebug()<<list;
    return list;
}

