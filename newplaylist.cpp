#include "newplaylist.h"
#include "ui_newplaylist.h"
#include "QDir"
#include "QFile"
#include "QMessageBox"

newplaylist::newplaylist(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::newplaylist)
{
    ui->setupUi(this);
}

newplaylist::~newplaylist()
{
    delete ui;
}

void newplaylist::on_buttonBox_accepted()
{
    QMessageBox msgBox;
    if(QDir("Playlists").exists() == false)
        QDir().mkdir("Playlists");

    QString playlist = ui->playlistNameEdit->text();
    ui->playlistNameEdit->clear();

    if(playlist.isEmpty())
    {
        QFile file("Playlists/Default.txt");
        file.open(QIODevice::Append);
        file.close();
    }
    else
    {
        QFile file("Playlists/" + playlist + ".txt");

        if(file.exists())
        {
            msgBox.setText("Плейлист с данным названием уже существует");
            msgBox.exec();
        }
        else
        {
            file.open(QIODevice::Append);
            file.close();
            emit newplaylistsignal(playlist);
        }
    }
}



