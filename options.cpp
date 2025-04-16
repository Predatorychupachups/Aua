#include "options.h"
#include "ui_options.h"

Options::Options(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Options)
{
    ui->setupUi(this);
    ui->OptionstStackedWidget->insertWidget(0,&general);
    ui->OptionstStackedWidget->setCurrentIndex(0);
}

Options::~Options()
{
    delete ui;
}


void Options::on_saveButton_clicked()
{
    connect(this, &Options::saveSignal, &general, &General::saveSlot);
    emit(saveSignal());
    this->close();
}


void Options::on_closeButton_clicked()
{
    connect(this, &Options::closeSignal, &general, &General::closeSlot);
    emit(closeSignal());
    this->close();
}

