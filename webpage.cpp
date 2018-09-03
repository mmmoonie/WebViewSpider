#include "webpage.h"
#include <QDir>

WebPage::WebPage(QObject *parent) : QWebPage(parent)
{
    networkAccessManager = new NetWorkAccessManager(this);
    setNetworkAccessManager(networkAccessManager);
    setLinkDelegationPolicy(QWebPage::DelegateAllLinks);
    QWebSettings * settings = this->settings();
    settings->setAttribute(QWebSettings::AutoLoadImages, true);
    settings->setAttribute(QWebSettings::DnsPrefetchEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptEnabled, true);
    settings->setAttribute(QWebSettings::JavascriptCanOpenWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanCloseWindows, false);
    settings->setAttribute(QWebSettings::JavascriptCanAccessClipboard, false);
    settings->setAttribute(QWebSettings::JavaEnabled, false);
    settings->setAttribute(QWebSettings::PluginsEnabled, false);
    settings->setAttribute(QWebSettings::PrivateBrowsingEnabled, false);
    settings->setAttribute(QWebSettings::SpatialNavigationEnabled, false);
    settings->setAttribute(QWebSettings::LinksIncludedInFocusChain, false);
    settings->setAttribute(QWebSettings::ZoomTextOnly, false);
    settings->setAttribute(QWebSettings::PrintElementBackgrounds, false);
    settings->setAttribute(QWebSettings::OfflineStorageDatabaseEnabled, true);
    settings->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, false);
    settings->setAttribute(QWebSettings::LocalStorageEnabled, true);
    settings->setAttribute(QWebSettings::LocalContentCanAccessRemoteUrls, true);
    settings->setAttribute(QWebSettings::LocalContentCanAccessFileUrls, true);
    settings->setAttribute(QWebSettings::XSSAuditingEnabled, true);
    settings->setAttribute(QWebSettings::AcceleratedCompositingEnabled, false);
    settings->setAttribute(QWebSettings::Accelerated2dCanvasEnabled, false);
    settings->setAttribute(QWebSettings::TiledBackingStoreEnabled, false);
    settings->setAttribute(QWebSettings::FrameFlatteningEnabled, false);
    settings->setAttribute(QWebSettings::SiteSpecificQuirksEnabled, true);
    settings->setAttribute(QWebSettings::CSSGridLayoutEnabled, false);
    settings->setAttribute(QWebSettings::ScrollAnimatorEnabled, false);
    settings->setAttribute(QWebSettings::CaretBrowsingEnabled, false);
    settings->setAttribute(QWebSettings::NotificationsEnabled, false);
    settings->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
}

WebPage::~WebPage()
{

}

void WebPage::javaScriptAlert(QWebFrame *frame, const QString &msg)
{
    QWebPage::javaScriptAlert(frame, msg);
}
bool WebPage::javaScriptConfirm(QWebFrame *frame, const QString &msg)
{
    return QWebPage::javaScriptConfirm(frame, msg);
}

QString WebPage::userAgentForUrl(const QUrl &url) const
{
    return "Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/64.0.3282.186 Safari/537.36";
}

NetWorkAccessManager * WebPage::getNetworkAccessManager()
{
    return networkAccessManager;
}
