#ifndef TOPICS_LIST_H
#define TOPICS_LIST_H

#include "QtXml/QtXml"
#include <QtWidgets>
#include <QtCore>
#include <QMainWindow>

namespace Ui {
class Topics_list;
}

class Topics_list : public QMainWindow
{
    Q_OBJECT

public:
    explicit Topics_list(QWidget *parent = 0);
    ~Topics_list();

private slots:
    void open();
    void openFB2(QFile *f);

private:
    Ui::Topics_list *ui;

    QStringList /*tokens,*/ thisToken;
};

#endif // TOPICS_LIST_H
