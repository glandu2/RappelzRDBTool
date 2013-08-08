#include "TabBarEventFilter.h"
#include <QTabBar>
#include <QTabWidget>
#include <QEvent>
#include <QMouseEvent>

TabBarEventFilter::TabBarEventFilter(QTabWidget *tabWidget, QObject *parent) :
    QObject(parent)
{
	QObject *tabWidgetParent;

	foreach(tabWidgetParent, tabWidget->children()) {
		QTabBar *tabBar = qobject_cast<QTabBar*>(tabWidgetParent);
		if(tabBar) {
			watchedTabBar = tabBar;
			watchedTabBar->installEventFilter(this);
			watchedTabWidget = tabWidget;
			watchedTabWidget->installEventFilter(this);
			break;
		}
	}
}

bool TabBarEventFilter::eventFilter(QObject *watched, QEvent *event) {
	bool result = QObject::eventFilter(watched, event);

	if(watched != watchedTabBar && watched != watchedTabWidget)
		return result;

	if(result == true)
		return result;

	if(watched == watchedTabWidget && event->type() == QEvent::MouseButtonDblClick) {
		QMouseEvent* me = static_cast<QMouseEvent*>(event);
		if(me->button() == Qt::LeftButton && me->pos().y() < watchedTabBar->sizeHint().height() && watchedTabBar->tabAt(me->pos()) == -1)
			emit addTab();
	} else if(watched == watchedTabBar && event->type() == QEvent::MouseButtonPress) {
		QMouseEvent* me = static_cast<QMouseEvent*>(event);
		int tabIndex = watchedTabBar->tabAt(me->pos());
		if(me->button() == Qt::MiddleButton && tabIndex != -1)
			emit removeTab(tabIndex);
	}

	return result;
}
