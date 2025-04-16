#ifndef NEWPLAYLIST_H
#define NEWPLAYLIST_H

#include <QDialog>

namespace Ui {
class newplaylist;
}

class newplaylist : public QDialog
{
    Q_OBJECT

public:
    explicit newplaylist(QWidget *parent = nullptr);
    ~newplaylist();

private slots:
    void on_buttonBox_accepted();

private:
    Ui::newplaylist *ui;

signals:
    void newplaylistsignal(QString);
};

#endif // NEWPLAYLIST_H
