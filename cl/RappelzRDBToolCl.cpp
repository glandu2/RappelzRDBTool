#include "IDatabase.h"
#include "IRowManipulator.h"
#include "IDatabaseDescription.h"

#include <string.h>
#include <stdio.h>
#include <malloc.h>
#include <errno.h>
#include "stdlib.h"

#ifdef __unix__
#	include <sys/ioctl.h>
#else
#	include <windows.h>
#endif

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
#define APPNAME "RappelzRDBToolCl"
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
			fputc('\xDB', stdout);
		lastPercentage = newPercentage;
	}
	/*if(rowProceed == totalRows) {
		HANDLE consoleHandle = GetStdHandle(STD_OUTPUT_HANDLE);
		CONSOLE_SCREEN_BUFFER_INFO conInfo;
		GetConsoleScreenBufferInfo(consoleHandle, &conInfo);
		if(conInfo.dwCursorPosition.X != 0)
			fputc('\n', stdout);
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
			   "sql_options\tSQL Options, one of: --sql-server <server ip>, --sql-port <port>\n"
			   "           \t --sql-user <username>, --sql-pwd <password>\n"
			   "\n"
			   "Notes:\n"
			   " -CSV is read on standard input and written on standard output (screen), use pipes to redirect.\n"
			   " - For more information about --src-<sqltype> and --dst-<sqltype>, type \""APPNAME" --help sql_opt\"\n"
			   " -SQL ODBC Server are configurable with odbcad32, type \""APPNAME" --help  sql_opt\" to show all recognized sql servers\n"
			   " -If the file name or table name after --src-<type> or --dst-<type> is omitted, the default name is used\n");
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
		printf("%d\t", row->getDataInt32("id"));
		for(j=1; j<9; j++) {
			sprintf(buffer, "stat_%d_present", j);
			if(row->getDataInt16(buffer) == 96) {
				int flag;
				int data;
				sprintf(buffer, "stat_flag_%d", j);
				flag = (int)(row->getDataFloat32(buffer));
				sprintf(buffer, "stat_%d", j);
				data = (int)(row->getDataFloat32(buffer));
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

int main(int argc, char *argv[])
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
	const char *sqlServer = "127.0.0.1";
	int sqlPort = 0;
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
				dbDescription = createExternDescriptedDatabase();
				switch(dbDescription->open(argv[i+1], 0)) {
					case 0:
						break;

					case ENOENT:
						throw "Db description file cannot be loaded";

					case EINVAL:
						throw "File is not a db description file";

					default:
						throw "Unknown error";
				}

				database = createDatabase(dbDescription);
			} catch(const char *errorStr) {
				fprintf(stderr, "Error Db type %s: %s\n", argv[i+1], errorStr);
				return 5;
			}
			i++;
		}
		//Source
		else if(!strcmp(argv[i], "--src-csv")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				source = NULL;
			else {
				source = argv[i+1];
				i++;
			}
			sType = mDF_CSV;
		} else if(!strcmp(argv[i], "--src-rdb")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				source = NULL;
			else {
				source = argv[i+1];
				i++;
			}
			sType = mDF_RDB;
		} else if(!strcmp(argv[i], "--src-sqlserver")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				source = NULL;
			else {
				source = argv[i+1];
				i++;
			}
			sType = mDF_SQLServer;
		} else if(!strcmp(argv[i], "--src-sqlpostgres")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				source = NULL;
			else {
				source = argv[i+1];
				i++;
			}
			sType = mDF_SQLPostgres;
		}
		//Destination
		else if(!strcmp(argv[i], "--dst-csv")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				dest = NULL;
			else {
				dest = argv[i+1];
				i++;
			}
			dType = mDF_CSV;
		} else if(!strcmp(argv[i], "--dst-rdb")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				dest = NULL;
			else {
				dest = argv[i+1];
				i++;
			}
			dType = mDF_RDB;
		} else if(!strcmp(argv[i], "--dst-sqlserver")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				dest = NULL;
			else {
				dest = argv[i+1];
				i++;
			}
			dType = mDF_SQLServer;
		} else if(!strcmp(argv[i], "--dst-sqlpostgres")) {
			if((i+1) >= argc || argv[i+1][0] == '-')
				dest = NULL;
			else {
				dest = argv[i+1];
				i++;
			}
			dType = mDF_SQLPostgres;
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
		} else if(!strcmp(argv[i], "--sql-port") && (i+1) < argc) {
			sqlPort = atoi(argv[i+1]);
			i++;
		} else {
			fprintf(stderr, "Unknown switch \"%s\"\n", argv[i]);
			return 1;
		}
	}

	char buffer[512];
	if(source == NULL)
		source = buffer;
	if(dest == NULL)
		dest = buffer;

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
			sprintf(buffer, "%s.csv", database->getDatabaseDescription()->getDefaultFileName());
			result = database->readData(DST_CSV, source, progressPercentage);
			break;

		case mDF_RDB:
			sprintf(buffer, "%s.rdb", database->getDatabaseDescription()->getDefaultFileName());
			result = database->readData(DST_RDB, source, progressPercentage);
			break;

		case mDF_SQLServer: {
			char location[512];
			sprintf(buffer, "Arcadia.dbo.%s", database->getDatabaseDescription()->getDefaultTableName());
#ifdef _WIN32
			sprintf(location, "Driver={SQL Server};Server=%s,%d;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 1433, sqlUser, sqlPassword);
#else
			sprintf(location, "Driver=FreeTDS;TDS_Version=7.2;ClientCharset=UTF-8;Server=%s,%d;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 1433, sqlUser, sqlPassword);
#endif
			result = database->readData(DST_SQLServer, source, progressPercentage, NULL, location);
			break;
		}

		case mDF_SQLPostgres: {
			char location[512];
			sprintf(buffer, "%s", database->getDatabaseDescription()->getDefaultTableName());
#ifdef _WIN64
			sprintf(location, "Driver={PostgreSQL Unicode(x64)};Server=%s;Port=%d;Database=Arcadia;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 5432, sqlUser, sqlPassword);
#else
			sprintf(location, "Driver={PostgreSQL Unicode};Server=%s;Port=%d;Database=Arcadia;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 5432, sqlUser, sqlPassword);
#endif
			result = database->readData(DST_SQLPostgres, source, progressPercentage, NULL, location);
			break;
		}
	}
	if(result) {
		fprintf(stderr, "Failed to load data, error %d\n", result);
		return 2;
	}
	switch(dType) {
		case mDF_CSV:
			sprintf(buffer, "%s.csv", database->getDatabaseDescription()->getDefaultFileName());
			if(!strcmp(dest, "con"))
				printWritePercentage = false;
			result = database->writeData(DST_CSV, dest, progressPercentage);
			break;

		case mDF_RDB:
			sprintf(buffer, "%s.rdb", database->getDatabaseDescription()->getDefaultFileName());
			result = database->writeData(DST_RDB, dest, progressPercentage);
			break;

		case mDF_SQLServer: {
			char location[512];
			sprintf(buffer, "Arcadia.dbo.%s", database->getDatabaseDescription()->getDefaultTableName());
#ifdef _WIN32
			sprintf(location, "Driver={SQL Server};Server=%s,%d;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 1433, sqlUser, sqlPassword);
#else
			sprintf(location, "Driver=FreeTDS;TDS_Version=7.2;ClientCharset=UTF-8;Server=%s,%d;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 1433, sqlUser, sqlPassword);
#endif
			result = database->writeData(DST_SQLServer, source, progressPercentage, NULL, location);
			break;
		}

		case mDF_SQLPostgres: {
			char location[512];
			sprintf(buffer, "%s", database->getDatabaseDescription()->getDefaultTableName());
#ifdef _WIN64
			sprintf(location, "Driver={PostgreSQL Unicode(x64)};Server=%s;Port=%d;Database=Arcadia;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 5432, sqlUser, sqlPassword);
#else
			sprintf(location, "Driver={PostgreSQL Unicode};Server=%s;Port=%d;Database=Arcadia;Uid=%s;Pwd=%s;", sqlServer, sqlPort ? sqlPort : 5432, sqlUser, sqlPassword);
#endif
			result = database->writeData(DST_SQLPostgres, source, progressPercentage, NULL, location);
			break;
		}
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
