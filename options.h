#ifndef OPTIONS_H
#define OPTIONS_H

#include <QMainWindow>
#include "general.h"

namespace Ui {
class Options;
}

class Options : public QMainWindow
{
    Q_OBJECT

public:
    explicit Options(QWidget *parent = nullptr);
    ~Options();

signals:
    void saveSignal();

    void closeSignal();

private slots:
    void on_saveButton_clicked();

    void on_closeButton_clicked();

private:
    Ui::Options *ui;
    General general;
};

#endif // OPTIONS_H
