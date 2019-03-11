#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QtSql>
#include "topics_list.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_translate_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_ShowEngWords_clicked();

    void on_pushButton_ShowRusWords_clicked();

    void on_pushButton_delete_clicked();

    void on_pushButton_reader_clicked();

private:
    Ui::MainWindow *ui;

    Topics_list *topics_list;

    QString eng_word, rus_word;
    QString pathToDB_translate = QDir::toNativeSeparators(QDir::currentPath() + "/dictionaryDB.db");
    QString pathToDB_MyWords   = QDir::toNativeSeparators(QDir::currentPath() + "/MyWords.db");

    QSqlDatabase  db = QSqlDatabase::addDatabase("QSQLITE");

    QSqlQuery *query = new QSqlQuery();

    QSqlTableModel *RussianWordsModel = new QSqlTableModel(this);
    QSqlTableModel *EnglishWordsModel = new QSqlTableModel(this);

    int  countEng = 2;
    int  countRus = 2;
    bool countDelete = false;   //удаление
};

#endif // MAINWINDOW_H
