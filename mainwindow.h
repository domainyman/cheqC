#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "modifys.h"
#include <QtSql>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QDebug>
#include <QMessageBox>
#include <QString>
#include <QModelIndex>
#include <QPrintDialog>
#include <QPrinter>
#include <QPrinterInfo>
#include <QPrintPreviewDialog>
#include <QPainter>
#include <QPageSize>
#include <QPaintEvent>
#include <QDoubleValidator>
#include <QCompleter>
#include <QPageSize>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    double size_w = 0 ;
    double size_h = 0;
    double pay_lm = 0 ;
    double pay_tm = 0;
    double pay_w = 0;
    double pay_h = 0;
    double am_lm = 0 ;
    double am_tm = 0;
    double am_w = 0;
    double am_h = 0;
    double cqam_lm = 0 ;
    double cqam_tm = 0;
    double cqam_w = 0;
    double cqam_h = 0;
    double da_lm = 0 ;
    double da_tm = 0;
    double da_w = 0;
    double da_h = 0;
    double be_lm = 0 ;
    double be_tm = 0;
    double be_w = 0;
    double be_h = 0;
    double on_lm = 0 ;
    double on_tm = 0;
    double on_w = 0;
    double on_h = 0;

    QSqlDatabase db;

    int datarow;

    int check_table;

    MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private slots:
    void on_clear_bu_clicked();

    void on_save_bu_clicked();

    void on_cheqView_activated();

    void on_delete_bu_clicked();

    void on_print_save_bu_clicked();

    void on_bank_setbu_clicked();

    void renew_bank_list();

    void on_upload_bu_clicked();

    QStringList pay_list();

protected:


private:
    Ui::MainWindow *ui;

    Modifys *modifys;

    QSqlTableModel *tableModel;

    QDate date = QDate::currentDate();

    QCompleter *StringComplater;

    QPrinterInfo info;

};
#endif // MAINWINDOW_H
