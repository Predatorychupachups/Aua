#ifndef TAGSEDIT_H
#define TAGSEDIT_H

#include <QMainWindow>

namespace Ui {
class TagsEdit;
}

class TagsEdit : public QMainWindow
{
    Q_OBJECT

public:
    explicit TagsEdit(QWidget *parent = nullptr);
    ~TagsEdit();

public slots:
    void audioPathSlot(QString audioPath);

private slots:
    void on_closeButton_clicked();

    void on_saveButton_clicked();

private:
    Ui::TagsEdit *ui;
};

#endif // TAGSEDIT_H
