#ifndef GENERAL_H
#define GENERAL_H

#include <QWidget>
#include <QSettings>

namespace Ui {
class General;
}

class General : public QWidget
{
    Q_OBJECT

public:
    explicit General(QWidget *parent = nullptr);
    ~General();

    void themesList();
private slots:
    void on_themesBox_currentTextChanged(const QString &arg1);

    void on_addThemeButton_clicked();

public slots:
    void saveSlot();

    void closeSlot();

private:
    Ui::General *ui;
    QSettings *settings;
};

#endif // GENERAL_H
