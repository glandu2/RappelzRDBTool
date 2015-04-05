#include "LogWindow.h"
#include "ui_LogWindow.h"
#include <QDateTime>
#include "Settings.h"

LogWindow* LogWindow::instance = 0;

LogWindow::LogWindow(QWidget *parent) :
	QDialog(parent),
    ui(new Ui::LogWindow)
{
	ui->setupUi(this);
	instance = this;
	getLogger()->setCallback(&onLogMessage);

	int level = Settings::getSettings()->value("LogWindow/logLevel", 3).toInt();
	ui->logLevelCombo->setCurrentIndex(level);
	onLogLevelChange(level);

	connect(ui->clearButton, SIGNAL(clicked()), this, SLOT(onClear()));
	connect(ui->logLevelCombo, SIGNAL(currentIndexChanged(int)), this, SLOT(onLogLevelChange(int)));

}

LogWindow::~LogWindow()
{
	Settings::getSettings()->setValue("LogWindow/logLevel", ui->logLevelCombo->currentIndex());
	delete ui;
}

void LogWindow::onClear() {
	ui->logEdit->clear();
}

void LogWindow::onLogLevelChange(int newLevel) {
	switch(newLevel) {
		case 0: getLogger()->setMaxLevel(ILog::LL_Fatal); break;
		case 1: getLogger()->setMaxLevel(ILog::LL_Error); break;
		case 2: getLogger()->setMaxLevel(ILog::LL_Warning); break;
		case 3: getLogger()->setMaxLevel(ILog::LL_Info); break;
		case 4: getLogger()->setMaxLevel(ILog::LL_Debug); break;
		case 5: getLogger()->setMaxLevel(ILog::LL_Trace); break;
		default:
			getLogger()->log(ILog::LL_Debug, "LogWindow: Invalid log level: %d\n", newLevel);
	}
}

void LogWindow::onLogMessage(ILog *logger, const char* message) {
	logger = logger;
	instance->ui->logEdit->appendPlainText(QDateTime::currentDateTime().toString("dd/MM/yyyy hh:mm:ss:zzz ") +   QString::fromLatin1(message, strlen(message)-1));
}
