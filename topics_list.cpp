#include "topics_list.h"
#include "ui_topics_list.h"

Topics_list::Topics_list(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Topics_list)
{
    ui->setupUi(this);

    ui->textBrowser->clear();
    ui->textBrowser->setFontPointSize(16);
    ui->textBrowser->setFontUnderline(false); //подчеркивание текста

    connect(ui->open_book, SIGNAL(clicked(bool)), this, SLOT(open()));
}

Topics_list::~Topics_list()
{
    delete ui;
}

void Topics_list::open()
{
    QString name = QFileDialog::getOpenFileName(this, "open", QDir::currentPath(), "");
    if(name.isEmpty())
        return;
    QFile f(name);
    ui->textBrowser->clear();
    if(name.endsWith(".fb2"));
        openFB2(&f);
}

void Topics_list::openFB2(QFile* f)
{
    if(!f->open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << "File was not open";
        return;
    }
    QXmlStreamReader sr(f);
    QString book;
    QString imgId;
    QString imgType;

    while(!sr.atEnd())
    {
        switch( sr.readNext() )
        {
        case QXmlStreamReader::NoToken:
            qDebug() << "QXmlStreamReader::NoToken";
            break;
        case QXmlStreamReader::StartDocument:
            book = "<!DOCTYPE HTML><html><body style=\"font-size:14px\">";
            break;
        case QXmlStreamReader::EndDocument:
            book.append("</body></html>");
            break;
        case QXmlStreamReader::StartElement:
            thisToken.append( sr.name().toString() );
            if( sr.name().toString() == "image" ) // расположение рисунков
            {
                if(sr.attributes().count() > 0)
                    book.append("<p align=\"center\">"+sr.attributes().at(0).value().toString()+"</p>");
            }
            if(sr.name() == "binary") // хранилище рисунков
            {
                imgId = sr.attributes().at(0).value().toString();
                imgType = sr.attributes().at(1).value().toString();
            }
            break;
        case QXmlStreamReader::EndElement:
            if( thisToken.last() == sr.name().toString() )
                thisToken.removeLast();
            else
                qDebug() << "error token";
            break;
        case QXmlStreamReader::Characters:
            if( sr.text().toString().contains( QRegExp("[A-Z]|[a-z]|[А-Я]|[а-я]") )) // если есть текст в блоке
            {
                if(thisToken.contains("description")) // ОПИСАНИЕ КНИГИ
                {
                    break; // не выводим
                }
                if(thisToken.contains("div"))
                    break;
                if(!thisToken.contains( "binary" ))
                    book.append("<p>" + sr.text().toString() + "</p>");
            }
            if(thisToken.contains( "binary" ) )//для рисунков
            {
                QString image = "<img src=\"data:"
                        + imgType +";base64,"
                        + sr.text().toString()
                        + "\"/>";
                book.replace("#"+imgId, image);
            }
            break;
        }
    }
    f->close();
    ui->textBrowser->setHtml(book);
    ui->textBrowser->verticalScrollBar()->setValue(0);
}























