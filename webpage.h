#ifndef WEBPAGE_H
#define WEBPAGE_H

#include <QWebPage>
#include <QWebFrame>
#include "networkaccessmanager.h"

class WebPage : public QWebPage
{
    Q_OBJECT
public:
    explicit WebPage(QObject *parent = 0);
    ~WebPage();
    NetWorkAccessManager * getNetworkAccessManager();
    QString userAgentForUrl(const QUrl &url) const;

signals:

public slots:

private:
    NetWorkAccessManager * networkAccessManager;

protected:
    void javaScriptAlert(QWebFrame * frame, const QString & msg);
};

#endif // WEBPAGE_H
