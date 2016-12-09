#include "SQLiteHelper.h"
#include "common.h"

SQLite::Database* SQLiteHelper::db = NULL;
SQLiteHelper::SQLiteHelper(void)
{
	//path = getAppPath();
	path.append("/home/chen/gsiot.db");
	//path.append("/root/gsiot.db");
	//printf("path=%s\n",path.c_str()); //jyc2016823 test

	try
	{
		if( !db )
		{
			db = new SQLite::Database( path.c_str(), SQLITE_OPEN_READWRITE );
		}
	}
	catch(...)
	{
		db = NULL;
		//LOGMSGEX( defLOGNAME, defLOG_ERROR, "Open DB failed!" );
		printf("Open DB failed!\n");
	}
}

SQLiteHelper::~SQLiteHelper(void)
{
}

void SQLiteHelper::FinalRelease()
{
	if(db)
	{
		delete db;
		db = NULL;
	}
}
