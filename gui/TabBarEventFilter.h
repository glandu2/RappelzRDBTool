#ifndef TABBAREVENTFILTER_H
#define TABBAREVENTFILTER_H

#include <QObject>

class QTabWidget;
class QTabBar;

class TabBarEventFilter : public QObject
{
		Q_OBJECT
	public:
		TabBarEventFilter(QTabWidget *tabWidget, QObject *parent = 0);
		virtual bool eventFilter(QObject *watched, QEvent *event);
		
	signals:
		void addTab();
		void removeTab(int index);
		
	private:
		QTabBar *watchedTabBar;
		QTabWidget *watchedTabWidget;
};

#endif // TABBAREVENTFILTER_H
