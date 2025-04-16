#include "general.h"
#include "ui_general.h"
#include <QStyleFactory>
#include <QFile>
#include <QDirIterator>
#include <QFileDialog>
#include <QDebug>

General::General(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::General)
{
    ui->setupUi(this);

    themesList();

    settings = new QSettings(QDir::currentPath()+"/settings.ini", QSettings::IniFormat);

    QString theme = settings->value("Theme").toString();
    QFileInfo file (theme);
    theme = file.fileName();
    int pos = theme.lastIndexOf(".qss");
    ui->themesBox->setCurrentText(theme.left(pos));
}

General::~General()
{
    QString theme = settings->value("Theme").toString();
    QFileInfo file (theme);
    theme = file.fileName();
    int pos = theme.lastIndexOf(".qss");
    ui->themesBox->setCurrentText(theme.left(pos));
    delete ui;
}

QString currentTheme;

void General::on_themesBox_currentTextChanged(const QString &arg1)
{
    QFile styleSheetFile;
    int pos = QCoreApplication::applicationDirPath().lastIndexOf("/");
    QString folderPath(QCoreApplication::applicationDirPath().left(pos)+"/themes/");
    if(QFile(":/themes/themes/"+arg1+".qss").exists())
    {
        currentTheme = ":/themes/themes/"+arg1+".qss";
        styleSheetFile.setFileName(currentTheme);
    }
    else
    {
        currentTheme = folderPath+arg1+".qss";
        styleSheetFile.setFileName(currentTheme);
    }
    styleSheetFile.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(styleSheetFile.readAll());
    qApp->setStyleSheet(styleSheet);
}

void General::on_addThemeButton_clicked()
{
    QStringList filesNames;
    filesNames.append( QFileDialog::getOpenFileName(this, "Выберите файл темы", "/","Файл темы (*.qss)"));
    if(!QDir("themes").exists())
    {
       QDir().mkdir("themes");
    }
    int pos = QCoreApplication::applicationDirPath().lastIndexOf("/");
    QString folderPath(QCoreApplication::applicationDirPath().left(pos));
    foreach(QString file, filesNames)
    {
        QFileInfo fileInfo(file);
        QString pathToFile(folderPath+"/themes/"+fileInfo.fileName());
        if(!QFile(pathToFile).exists())
        QFile::copy(file, pathToFile);
    }
    themesList();
}

void General::saveSlot()
{    
    settings->setValue("Theme", currentTheme);
    settings->sync();
}

void General::closeSlot()
{
    QString theme = settings->value("Theme").toString();
    QFileInfo file (theme);
    theme = file.fileName();
    int pos = theme.lastIndexOf(".qss");
    ui->themesBox->setCurrentText(theme.left(pos));
}

void General::themesList()
{
    ui->themesBox->clear();
    QDirIterator themesRes(":/themes/themes/", QStringList()<< "*.qss", QDir::Files, QDirIterator::Subdirectories);
    while (themesRes.hasNext())
    {
        QFileInfo f(themesRes.next());
        int pos = f.fileName().lastIndexOf(".qss");
        ui->themesBox->addItem(f.fileName().left(pos));
    }
    if(QDir("themes").exists())
    {
        int pos = QCoreApplication::applicationDirPath().lastIndexOf("/");
        QString folderPath(QCoreApplication::applicationDirPath().left(pos)+"/themes/");
        QDirIterator themes(folderPath, QStringList()<< "*.qss", QDir::Files, QDirIterator::Subdirectories);
        while(themes.hasNext())
        {
           QFileInfo f(themes.next());
           int pos = f.fileName().lastIndexOf(".qss");
           ui->themesBox->addItem(f.fileName().left(pos));
        }
    }
}
