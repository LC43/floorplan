#ifndef _FLOOR_BROWSER_
#defined _FLOOR_BROWSER_

#include <QDesktopServices>
#include <QUrl>

class Browser
{
	Q_OBJECT
public:
	Browser() {url = QUrl()}
public slots:
	static void call(QString link) {QDesktopServices::openUrl(url.set(link))};
private:
	QUrl * url;
};

#endif

