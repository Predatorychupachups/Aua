#ifndef RADIO_H
#define RADIO_H

#include <QDialog>
#include <QMediaPlayer>
#include <QSettings>

namespace Ui {
class Radio;
}

class Radio : public QDialog
{
    Q_OBJECT

public:
    explicit Radio(QWidget *parent = nullptr);
    ~Radio();

    void savedRadio();
    void loadRadio();

signals:
    void radioSinganl();

private slots:
    void on_loadButton_clicked();

    void on_CloseButton_clicked();

    void on_MuteButton_clicked();

    void on_volumeSlider_valueChanged(int value);

private:
    Ui::Radio *ui;
    QMediaPlayer *m_radio;
    QSettings *settings;
    void closeEvent(QCloseEvent *event);
};

#endif // RADIO_H
