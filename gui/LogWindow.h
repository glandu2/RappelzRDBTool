#ifndef LOGWINDOW_H
#define LOGWINDOW_H

#include "ILog.h"
#include <QDialog>

namespace Ui {
class LogWindow;
}

class LogWindow : public QDialog {
	Q_OBJECT

public:
	explicit LogWindow(QWidget* parent = 0);
	~LogWindow();

protected slots:
	void onClear();
	void onLogLevelChange(int newLevel);

signals:
	void logMessageCountUpdated(int count);

private:
	static void onLogMessageStatic(ILog* logger, const char* message);
	void onLogMessage(const char* message);

private:
	Ui::LogWindow* ui;
	int logLineCount;
	static LogWindow* instance;
};

#endif  // LOGWINDOW_H
