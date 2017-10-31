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

private:
	static void onLogMessage(ILog* logger, const char* message);

private:
	Ui::LogWindow* ui;
	static LogWindow* instance;
};

#endif  // LOGWINDOW_H
