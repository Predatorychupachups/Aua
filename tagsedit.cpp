#include "tagsedit.h"
#include "ui_tagsedit.h"
#include "taglib/fileref.h"

#include <QDebug>
#include <QFile>

TagsEdit::TagsEdit(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::TagsEdit)
{
    ui->setupUi(this);
}

TagsEdit::~TagsEdit()
{
    delete ui;
}


QString pathToAudio;

void TagsEdit::audioPathSlot(QString audioPath)
{
    pathToAudio = audioPath;
    TagLib::FileRef file(QFile::encodeName(audioPath).constData());
    ui->titleLineEdit->setText(QString::fromStdWString(file.tag()->title().toWString()));
    ui->artistLineEdit->setText(QString::fromStdWString(file.tag()->artist().toWString()));
    ui->genreLineEdit->setText(QString::fromStdWString(file.tag()->genre().toWString()));
    ui->yearLineEdit->setText(QString(file.tag()->year()));
    ui->albumTitleLineEdit->setText(QString::fromStdWString(file.tag()->album().toWString()));
    ui->commentLineEdit->setText(QString::fromStdWString(file.tag()->comment().toWString()));
}


void TagsEdit::on_closeButton_clicked()
{
    this->close();
}


void TagsEdit::on_saveButton_clicked()
{
    TagLib::FileRef file(QFile::encodeName(pathToAudio).constData());
    file.tag()->setTitle(ui->titleLineEdit->text().toStdString());
    file.tag()->setArtist(ui->artistLineEdit->text().toStdString());
    file.tag()->setGenre(ui->genreLineEdit->text().toStdString());
    file.tag()->setYear(ui->yearLineEdit->text().toInt());
    file.tag()->setAlbum(ui->albumTitleLineEdit->text().toStdString());
    file.tag()->setComment(ui->commentLineEdit->text().toStdString());
    file.save();
}

