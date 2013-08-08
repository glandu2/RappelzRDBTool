#include "Base/IDatabase.h"
#include "Base/IRowManipulator.h"
#include "Base/IDatabaseDescription.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>

#ifdef __unix__
#	include <sys/ioctl.h>
#else
#	include <windows.h>
#endif

/*
 * TODO:
 * - Continuer changement mode d'exportation + nouvelle utilisation de Database avec ExternDescriptedDatabase
 * - Faire un acces public pour les données (RowManipulator + choix colonne)
 */

/*
 * Tests fait:
 * - RDB -> RDB	OK
 * - RDB -> CSV	OK
 * - CSV -> CSV Ok (except string fields with space)
 * - CSV -> RDB Ok (except string fields with space)
 * - SQL -> CSV Ok
 */

enum eDF { mDF_None, mDF_CSV, mDF_RDB, mDF_SQLServer, mDF_SQLPostgres };

#ifdef __GNUC__
#pragma GCC diagnostic ignored "-Wswitch"
#endif

#ifndef APPNAME
#define APPNAME "RappelzDBTool"
#endif

//used to hide percentage info when printing csv
bool printWritePercentage;

void DLLCALLCONV progressPercentage(void *arg, int rowProceed, int totalRows) {
	static int lastPercentage = 0;
	static int consoleWidth = 0;

	if(!printWritePercentage) return;

	if(!consoleWidth) {
#ifdef __unix__
		struct winsize terminalSize;
		ioctl(0, TIOCGWINSZ, &terminalSize);
		consoleWidth = terminalSize.ws_col;
#else
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &conInfo);
		consoleWidth = conInfo.dwSize.X;
#endif
	}

	if(!totalRows) return;

	int newPercentage = rowProceed*consoleWidth/totalRows;

	if(totalRows && newPercentage != lastPercentage) {
		int i;
		for(i=0; i<(newPercentage - lastPercentage); i++)
			fputc('\xDB', stderr);
		lastPercentage = newPercentage;
	}
	/*if(rowProceed == totalRows) {
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &conInfo);
		if(conInfo.dwCursorPosition.X != 0)
			fputc('\n', stderr);
	}*/
}

void help(const char *arg) {
	printf(APPNAME" - Glandu2 (%s %s)\n"
		   "Convert or show a rappelz database, support sql, rdb, csv\n\n", __DATE__, __TIME__);

	if(!strcmp(arg, "sql_opt"))
		printf("Supported SQL Servers: (switch, server name, ODBC driver name)\n"
			   "sqlserver MS SQL Server (with SQL Server Native Client)\n"
			   "sqlpostgres PostgreSQL (with pgsqlODBC)\n"
			   "\n"
			   "To use a sql type type --src-<switch> or --dst-<switch>\n"
			   "For Example, if you want to use a SQL Server source, use --src-sqlserver\n");
	else
		printf("Usage "APPNAME" [--help | (--dbtype <descdll> src_type dst_type [sql_options])]\n"
			   "\n"
			   "--help     \tShow this help screen\n"
			   "<descdll>  \tDB type, <descdll> is the database description dll\n"
			   "           \t   (for example: --dbtype StringDatabase.dll for StringDatabase.dll)\n"
			   "src_type   \tSource type, one of: --src-csv, --src-rdb <rdb file name>,\n"
			   "           \t --src-<sqltype> <table name>\n"
			   "dst_type   \tDestination type, one of: --dst-csv, --dst-rdb <rdb file name>,\n"
			   "           \t --dst-<sqltype> <table name>\n"
			   "\n"
			   "sql_options\tSQL Options, one of: --sql-server <odbc sql source>,\n"
			   "           \t --sql-user <username>, --sql-pwd <password>\n"
			   "\n"
			   "Notes:\n"
			   " -CSV is read on standard input and written on standard output (screen), use pipes to redirect.\n"
			   " - For more information about --src-<sqltype> and --dst-<sqltype>, type \""APPNAME" --help sql_opt\"\n"
			   " -SQL ODBC Server are configurable with odbcad32, type \""APPNAME" --help  sql_opt\" to show all recognized sql servers\n");
}

void tempFunction(IDatabase *database) {
	IRowManipulator *row = database->getRowManipulator();
	int rowCount = database->getRowCount();
	int i, j;
	char buffer[50];
	int str, vit, agi, dex, intel, wis, lck;

	printf("id\tstr\tvit\tagi\tdex\tintel\twis\tlck\n");

	for(i=0; i<rowCount; i++) {
		str = vit = agi = dex = intel = wis = lck = 0;
		row->setCurrentRow(database->getRowAt(i));
		printf("%d\t", *static_cast<int*>(row->getValuePtr("id")));
		for(j=1; j<9; j++) {
			sprintf(buffer, "stat_%d_present", j);
			if(*static_cast<short*>(row->getValuePtr(buffer)) == 96) {
				int flag;
				int data;
				sprintf(buffer, "stat_flag_%d", j);
				flag = (int)(*static_cast<float*>(row->getValuePtr(buffer)));
				sprintf(buffer, "stat_%d", j);
				data = (int)(*static_cast<float*>(row->getValuePtr(buffer)));
				if(flag & 0x01)
					str = data;
				if(flag & 0x02)
					vit = data;
				if(flag & 0x04)
					agi = data;
				if(flag & 0x08)
					dex = data;
				if(flag & 0x10)
					intel = data;
				if(flag & 0x20)
					wis = data;
				if(flag & 0x40)
					lck = data;
			} else break;
		}
		printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\n", str, vit, agi, dex, intel, wis, lck);
	}
}

int __cdecl main(int argc, char *argv[])
{
	int i, result;
	IDatabase *database = 0;
	IDatabaseDescription *dbDescription = 0;
	eDF sType = mDF_None;
	eDF dType = mDF_None;
	const char *source = NULL;
	const char *dest = NULL;
	const char *sqlUser = "sa";
	const char *sqlPassword = "";
	const char *sqlServer = "SQLEXPRESS";
#if !defined(NDEBUG) && !defined(__unix__)
	LARGE_INTEGER beginTime, endTime, freq;
#endif

	printWritePercentage = true;

	if(argc <= 1) {
		help("");
		return 0;
	}

#if !defined(NDEBUG) && !defined(__unix__)
	QueryPerformanceFrequency(&freq);
	QueryPerformanceCounter(&beginTime);
#endif

	for(i=1; i<argc; i++) {
		if(!strcmp(argv[i], "--help")) {
			if((i+1) < argc)
				help(argv[i+1]);
			else help("");
			return 0;
		} else if(!strcmp(argv[i], "--dbtype") && i+1 < argc) {
			try {
				dbDescription = createExternDescriptedDatabase(argv[i+1]);
				database = createDatabase(dbDescription);
			} catch(const char *errorStr) {
				fprintf(stderr, "Error Db type %s: %s\n", argv[i+1], errorStr);
				return 5;
			}
			i++;
		}
		//Source
		else if(!strcmp(argv[i], "--src-csv") && (i+1) < argc) {
			source = argv[i+1];
			sType = mDF_CSV;
			i++;
		} else if(!strcmp(argv[i], "--src-rdb") && (i+1) < argc) {
			source = argv[i+1];
			sType = mDF_RDB;
			i++;
		} else if(!strcmp(argv[i], "--src-sqlserver") && (i+1) < argc) {
			source = argv[i+1];
			sType = mDF_SQLServer;
			i++;
		} else if(!strcmp(argv[i], "--src-sqlpostgres") && (i+1) < argc) {
			source = argv[i+1];
			sType = mDF_SQLPostgres;
			i++;
		}
		//Destination
		else if(!strcmp(argv[i], "--dst-csv") && (i+1) < argc) {
			dest = argv[i+1];
			dType = mDF_CSV;
			i++;
		} else if(!strcmp(argv[i], "--dst-rdb") && (i+1) < argc) {
			dest = argv[i+1];
			dType = mDF_RDB;
			i++;
		} else if(!strcmp(argv[i], "--dst-sqlserver") && (i+1) < argc) {
			dest = argv[i+1];
			dType = mDF_SQLServer;
			i++;
		} else if(!strcmp(argv[i], "--dst-sqlpostgres") && (i+1) < argc) {
			dest = argv[i+1];
			dType = mDF_SQLPostgres;
			i++;
		}
		//SQL options
		else if(!strcmp(argv[i], "--sql-user") && (i+1) < argc) {
			sqlUser = argv[i+1];
			i++;
		} else if(!strcmp(argv[i], "--sql-pwd") && (i+1) < argc) {
			sqlPassword = argv[i+1];
			i++;
		} else if(!strcmp(argv[i], "--sql-server") && (i+1) < argc) {
			sqlServer = argv[i+1];
			i++;
		} else {
			fprintf(stderr, "Unknown switch \"%s\"\n", argv[i]);
			return 1;
		}
	}

	if(!database) {
		fprintf(stderr, "You must specify the database structure DLL to use with --dbtype\n");
		return 6;
	}

	if(sType == mDF_None || dType == mDF_None) {
		fprintf(stderr, "You must choose the data format for the source and destination\n");
		return 4;
	}

	switch(sType) {
		case mDF_CSV:
			result = database->readData(DST_CSV, source, progressPercentage);
			break;

		case mDF_RDB:
			result = database->readData(DST_RDB, source, progressPercentage);
			break;

		case mDF_SQLServer:
			result = database->readData(DST_SQLServer, source, progressPercentage, NULL, sqlServer, sqlUser, sqlPassword);
			break;

		case mDF_SQLPostgres:
			result = database->readData(DST_SQLPostgres, source, progressPercentage, NULL, sqlServer, sqlUser, sqlPassword);
			break;
	}
	if(result) {
		fprintf(stderr, "Failed to load data, error %d\n", result);
		return 2;
	}
	switch(dType) {
		case mDF_CSV:
			if(!strcmp(dest, "con"))
				printWritePercentage = false;
			result = database->writeData(DST_CSV, dest, progressPercentage);
			break;

		case mDF_RDB:
			result = database->writeData(DST_RDB, dest, progressPercentage);
			break;

		case mDF_SQLServer:
			result = database->writeData(DST_SQLServer, dest, progressPercentage, NULL, sqlServer, sqlUser, sqlPassword);
			break;

		case mDF_SQLPostgres:
			result = database->writeData(DST_SQLPostgres, dest, progressPercentage, NULL, sqlServer, sqlUser, sqlPassword);
			break;
	}
	if(result) {
		fprintf(stderr, "Failed to write data, error %d\n", result);
		return 3;
	}

#if !defined(NDEBUG) && !defined(__unix__)
	QueryPerformanceCounter(&endTime);
	fprintf(stderr, "Execution time: %fms\n", (endTime.QuadPart - beginTime.QuadPart)*1000.0/freq.QuadPart);
#endif

	//tempFunction(database);

	database->close(NULL);
	delete database;
	//system("pause");

    return 0;
}
