#include "LogWindow.h"
#include "Settings.h"
#include "ui_LogWindow.h"
#include <QDateTime>

LogWindow* LogWindow::instance = 0;

LogWindow::LogWindow(QWidget* parent) : QDialog(parent), ui(new Ui::LogWindow), logLineCount(0) {
	ui->setupUi(this);
	instance = this;
	getLogger()->setCallback(&onLogMessageStatic);

	int level = Settings::getSettings()->value("LogWindow/logLevel", (int) ILog::LL_Warning).toInt();
	ui->logLevelCombo->setCurrentIndex(level);
	onLogLevelChange(level);

	connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(ui->logLevelCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onLogLevelChange(int)));
}

LogWindow::~LogWindow() {
	Settings::getSettings()->setValue("LogWindow/logLevel", ui->logLevelCombo->currentIndex());
	delete ui;
}

void LogWindow::onClear() {
	ui->logEdit->clear();
	logLineCount = 0;
	emit logMessageCountUpdated(logLineCount);
}

void LogWindow::onLogLevelChange(int newLevel) {
	switch(newLevel) {
		case 0:
			getLogger()->setMaxLevel(ILog::LL_Fatal);
			break;
		case 1:
			getLogger()->setMaxLevel(ILog::LL_Error);
			break;
		case 2:
			getLogger()->setMaxLevel(ILog::LL_Warning);
			break;
		case 3:
			getLogger()->setMaxLevel(ILog::LL_Info);
			break;
		case 4:
			getLogger()->setMaxLevel(ILog::LL_Debug);
			break;
		case 5:
			getLogger()->setMaxLevel(ILog::LL_Trace);
			break;
		default:
			getLogger()->log(ILog::LL_Debug, "LogWindow: Invalid log level: %d\n", newLevel);
	}
}

void LogWindow::onLogMessageStatic(ILog* logger, const char* message) {
	logger = logger;
	instance->onLogMessage(message);
}

void LogWindow::onLogMessage(const char* message) {
	ui->logEdit->appendPlainText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss:zzz ") +
	                             QString::fromLatin1(message, strlen(message) - 1));
	logLineCount++;
	emit logMessageCountUpdated(logLineCount);
}
