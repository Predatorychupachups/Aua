#ifndef MUSICBROWSER_H
#define MUSICBROWSER_H

#include <QWidget>
#include <QtWebEngineWidgets/qwebengineview.h>

namespace Ui {
class MusicBrowser;
}

class MusicBrowser : public QWidget
{
    Q_OBJECT

public:
    explicit MusicBrowser(QWidget *parent = nullptr);
    ~MusicBrowser();

signals:
    void mainWindowShowSignal();

public slots:
    void urlSlot(QString urlString);

private slots:
    void on_backButton_clicked();

private:
    Ui::MusicBrowser *ui;
    QWebEngineView *web;
};

#endif // MUSICBROWSER_H
