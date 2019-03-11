#include "mainwindow.h"
#include "ui_mainwindow.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_translate_clicked()  //перевод
{
//передали в переменную англ слово
    eng_word = ui->lineEdit->text();

    db.setDatabaseName(pathToDB_translate);
    if(!db.open()) {
        qDebug() << db.lastError().text();
        return;
    }

//ищем перевод
    query->prepare("SELECT RussianWords.word"
                   " FROM RussianWords"
                   " WHERE RussianWords.id = ("
                   " SELECT vocabulary.rusID"
                   " FROM vocabulary"
                   " WHERE vocabulary.engID ="
                   " (SELECT EnglishWords.id"
                   " FROM EnglishWords"
                   " WHERE EnglishWords.word"
                   " LIKE :eng_word))");

    query->bindValue(":eng_word", eng_word);
    query->exec();

//передаем в переменную перевод
    if(query->next()) {
        rus_word = query->value(0).toString();
    }
    else {
        rus_word.clear();
        ui->label->clear();
    }

//выводим перевод в label
    ui->label->setText(rus_word);
}

void MainWindow::on_pushButton_save_clicked()   //сохранение
{
    ui->statusBar->clearMessage();

    on_pushButton_translate_clicked();

    db.setDatabaseName(pathToDB_MyWords);
    if(!db.open()) {
        qDebug() << db.lastError().text();
        return;
    }

//если переменные пусты, те перевод не удался, значит и сохранять нечего
    if(eng_word.size() < 1 || rus_word.size() < 1) {
        ui->statusBar->showMessage("This word has not translate.");
        return;
    }
//иначе заносим данные этих переменных в БД
    else {
        query->prepare("INSERT INTO RussianWords(word)"
                       " VALUES (:rus_word)");
        query->bindValue(":rus_word", rus_word);
        query->exec();

        query->prepare("INSERT INTO EnglishWords(word)"
                       " VALUES (:eng_word)");
        query->bindValue(":eng_word", eng_word);
        query->exec();

        ui->statusBar->showMessage("successfully!");

        for(short i(0); i < 2; ++i) {
            on_pushButton_ShowEngWords_clicked();
            on_pushButton_ShowRusWords_clicked();
        }
    }
}

void MainWindow::on_pushButton_ShowEngWords_clicked()   //показать список сохраненных англ слов
{
    if(countEng % 2 != 1)
    {
        db.setDatabaseName(pathToDB_MyWords);
        if(!db.open()) {
            qDebug() << db.lastError().text();
            return;
        }
//выбор всех английских слов из БД
        EnglishWordsModel->setTable("EnglishWords");
        EnglishWordsModel->select();

//отображение их в tableView
        ui->tableView->setModel(EnglishWordsModel);
        ui->tableView->hideColumn(0);

        countDelete = true;
    }
    else{
        ui->tableView->hideColumn(1);
        countDelete = false;
    }
    ++countEng;
}

void MainWindow::on_pushButton_ShowRusWords_clicked()  //показать список сохраненных рус слов
{
    if(countRus % 2 != 1)
    {
        db.setDatabaseName(pathToDB_MyWords);
        if(!db.open()) {
            qDebug() << db.lastError().text();
            return;
        }
//выбор всех русских слов из БД
        RussianWordsModel->setTable("RussianWords");
        RussianWordsModel->select();

//отображение их в tableView_2
        ui->tableView_2->setModel(RussianWordsModel);
        ui->tableView_2->hideColumn(0);
    }
    else{
        ui->tableView_2->hideColumn(1);
    }
    ++countRus;
}

void MainWindow::on_pushButton_delete_clicked() //удаление выбранного слова из списка и из БД
{
    ui->statusBar->clearMessage();

    if(countDelete != false)    //если tableView отображено
    {
        QItemSelectionModel *selectModel = ui->tableView->selectionModel();

        if(selectModel->hasSelection())
        {
            QModelIndex index = ui->tableView->selectionModel()->selectedIndexes().first();

            on_pushButton_ShowRusWords_clicked();

            EnglishWordsModel->removeRow(index.row());
            RussianWordsModel->removeRow(index.row());

            for(short i = 0; i < 2; ++i) {
                on_pushButton_ShowEngWords_clicked();
            }
            on_pushButton_ShowRusWords_clicked();
        }
    }
    else {
        ui->statusBar->showMessage("Chose english word for delete");
    }
}

void MainWindow::on_pushButton_reader_clicked()
{
    hide();
    topics_list = new Topics_list(this);
    topics_list->show();
}





























