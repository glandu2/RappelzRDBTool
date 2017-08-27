#include "FileDialog.h"

#ifdef _WIN32
#ifndef _UNICODE
#define _UNICODE
#endif
#ifndef UNICODE
#define UNICODE
#endif
#define WINVER 0x0400
#include <windows.h>
#include <QDir>
#include <QFileInfo>
#endif

#ifdef _WIN32

QString FileDialog::getSaveFileName(QWidget* parent, const QString &caption, const QString& dir, const QString& filter, QString *selectedFilter, QFileDialog::Options options) {
	OPENFILENAME ofn;
	WCHAR filename[1024];
	QFileInfo initialFileInfo(dir);
	QString initialDir = QDir::toNativeSeparators(initialFileInfo.absolutePath());
	QString initialFileName = initialFileInfo.fileName();
	QString nullSeparatedFilter;
	QStringList filters = filter.split(";;", QString::SkipEmptyParts);

	QString oneFilter;
	int initialFilterIndex = 0;
	int i = 1;
	foreach(oneFilter, filters) {
		int beginIdx, endIdx;

		if(selectedFilter && oneFilter == *selectedFilter)
			initialFilterIndex = i;

		beginIdx = oneFilter.indexOf('(');
		if(beginIdx < 0)
			beginIdx = 0;
		else
			beginIdx++;

		endIdx = oneFilter.lastIndexOf(')');
		if(endIdx > beginIdx)
			endIdx -= beginIdx;
		else
			endIdx = -1;

		nullSeparatedFilter.append(oneFilter);
		nullSeparatedFilter += QChar();
		nullSeparatedFilter.append(oneFilter.mid(beginIdx, endIdx).replace(' ', ';'));
		nullSeparatedFilter += QChar();

		i++;
	}

	memcpy(filename, initialFileName.utf16(), (initialFileName.size()+1)*sizeof(QChar));

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent ? parent->winId() : 0;
	ofn.lpstrFilter = (LPCWSTR) nullSeparatedFilter.utf16();
	if(selectedFilter)
		ofn.nFilterIndex = initialFilterIndex;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrInitialDir = (LPCWSTR) initialDir.utf16();
	ofn.lpstrTitle = (LPCWSTR) caption.utf16();

	ofn.Flags = OFN_HIDEREADONLY | OFN_NOCHANGEDIR | OFN_PATHMUSTEXIST | OFN_NONETWORKBUTTON;
	if(options & QFileDialog::DontResolveSymlinks)
		ofn.Flags |= OFN_NODEREFERENCELINKS;
	if(!(options & QFileDialog::DontConfirmOverwrite))
		ofn.Flags |= OFN_OVERWRITEPROMPT;

	if(GetSaveFileName(&ofn)) {
		if(selectedFilter && ((int)ofn.nFilterIndex - 1) >= 0 && ((int)ofn.nFilterIndex - 1) < filters.size()) {
			*selectedFilter = filters[ofn.nFilterIndex - 1];
		}
		return QDir::fromNativeSeparators(QString::fromWCharArray(filename));
	}

	return QString();
}

QString FileDialog::getOpenFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) {
	OPENFILENAME ofn;
	WCHAR filename[1024];
	QFileInfo initialFileInfo(dir);
	QString initialDir = QDir::toNativeSeparators(initialFileInfo.absolutePath());
	QString initialFileName = initialFileInfo.fileName();
	QString nullSeparatedFilter;
	QStringList filters = filter.split(";;", QString::SkipEmptyParts);

	QString oneFilter;
	int initialFilterIndex = 0;
	int i = 1;
	foreach(oneFilter, filters) {
		int beginIdx, endIdx;

		if(selectedFilter && oneFilter == *selectedFilter)
			initialFilterIndex = i;

		beginIdx = oneFilter.indexOf('(');
		if(beginIdx < 0)
			beginIdx = 0;
		else
			beginIdx++;

		endIdx = oneFilter.lastIndexOf(')');
		if(endIdx > beginIdx)
			endIdx -= beginIdx;
		else
			endIdx = -1;

		nullSeparatedFilter.append(oneFilter);
		nullSeparatedFilter += QChar();
		nullSeparatedFilter.append(oneFilter.mid(beginIdx, endIdx).replace(' ', ';'));
		nullSeparatedFilter += QChar();

		i++;
	}

	memcpy(filename, initialFileName.utf16(), (initialFileName.size()+1)*sizeof(QChar));

	memset(&ofn, 0, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = parent ? parent->winId() : 0;
	ofn.lpstrFilter = (LPCWSTR) nullSeparatedFilter.utf16();
	if(selectedFilter)
		ofn.nFilterIndex = initialFilterIndex;
	ofn.lpstrFile = filename;
	ofn.nMaxFile = sizeof(filename);
	ofn.lpstrInitialDir = (LPCWSTR) initialDir.utf16();
	ofn.lpstrTitle = (LPCWSTR) caption.utf16();

	ofn.Flags = OFN_NOCHANGEDIR | OFN_FILEMUSTEXIST | OFN_NONETWORKBUTTON | OFN_HIDEREADONLY;
	if(options & QFileDialog::DontResolveSymlinks)
		ofn.Flags |= OFN_NODEREFERENCELINKS;

	if(GetOpenFileName(&ofn)) {
		if(selectedFilter && ((int)ofn.nFilterIndex - 1) >= 0 && ((int)ofn.nFilterIndex - 1) < filters.size()) {
			*selectedFilter = filters[ofn.nFilterIndex - 1];
		}
		return QDir::fromNativeSeparators(QString::fromWCharArray(filename));
	}

	return QString();
}


#else

QString FileDialog::getSaveFileName(QWidget* parent, const QString &caption, const QString& dir, const QString& filter, QString *selectedFilter, QFileDialog::Options options) {
	return QFileDialog::getSaveFileName(parent, caption, dir, filter, selectedFilter, options);
}

QString FileDialog::getOpenFileName(QWidget *parent, const QString &caption, const QString &dir, const QString &filter, QString *selectedFilter, QFileDialog::Options options) {
	return QFileDialog::getOpenFileName(parent, caption, dir, filter, selectedFilter, options);
}

#endif
