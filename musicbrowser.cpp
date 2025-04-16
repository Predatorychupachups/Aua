#include "musicbrowser.h"
#include "ui_musicbrowser.h"

MusicBrowser::MusicBrowser(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::MusicBrowser),
    web(new QWebEngineView(this))
{
    ui->setupUi(this);
    ui->verticalLayout->addWidget(web);
}

MusicBrowser::~MusicBrowser()
{
    delete ui;
}

void MusicBrowser::urlSlot(QString urlString)
{
    web->load(QUrl(urlString));
}

void MusicBrowser::on_backButton_clicked()
{
    web->load(QUrl("empty.html"));
    emit mainWindowShowSignal();
    this->close();
}
