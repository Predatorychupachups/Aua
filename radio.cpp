#include "radio.h"
#include "ui_radio.h"
#include <QDir>
#include <QUrl>

Radio::Radio(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Radio)
{
    ui->setupUi(this);
    m_radio = new QMediaPlayer;
    m_radio->setVolume(0);
    m_radio->setMuted(true);
    settings = new QSettings(QDir::currentPath()+"/playerSaves.ini", QSettings::IniFormat);
    loadRadio();
}

Radio::~Radio()
{
    delete ui;
}

void Radio::savedRadio()
{
    settings->setValue("Radio url", ui->radioLineEdit->text());
    settings->setValue("Radio volume", ui->volumeSlider->value());
    settings->sync();
}

void Radio::loadRadio()
{
    ui->radioLineEdit->setText(settings->value("Radio url","").toString());
    ui->volumeSlider->setValue(settings->value("Radio volume",0).toInt());
}

void Radio::on_loadButton_clicked()
{
    emit(radioSinganl());
    m_radio->setMedia(QUrl(ui->radioLineEdit->text()));
    m_radio->play();
}


void Radio::on_CloseButton_clicked()
{
    savedRadio();
    m_radio->stop();
    this->close();
}


void Radio::on_MuteButton_clicked()
{
    m_radio->setMuted(!m_radio->isMuted());
    if(m_radio->isMuted())
        ui->MuteButton->setIcon(QIcon(":/images/images/0%.svg"));
    else
        ui->MuteButton->setIcon(QIcon(":/images/images/100%.svg"));
}


void Radio::on_volumeSlider_valueChanged(int value)
{
    m_radio->setVolume(value);
    if(m_radio->isMuted())
    {
        m_radio->setMuted(false);
        ui->MuteButton->setIcon(QIcon(":/images/images/100%.svg"));
    }
}

void Radio::closeEvent(QCloseEvent *event)
{
    savedRadio();
    m_radio->stop();
}

