/*
 *  Copyright (c) 2004-2015 by Jakob Schr√∂ter <js@camaya.net>
 *  This file is part of the gloox library. http://camaya.net/gloox
 *
 *  This software is distributed under a license. The full license
 *  agreement can be found in the file LICENSE in this distribution.
 *  This software may not be copied, modified, sold or distributed
 *  other than expressed in the named license agreement.
 *
 *  This software is distributed without any warranty.
 */

#include "gloox/client.h"
#include "gloox/connectionlistener.h"
#include "gloox/adhoccommandprovider.h"
#include "gloox/disco.h"
#include "gloox/adhoc.h"
#include "gloox/tag.h"
#include "gloox/loghandler.h"
#include "gloox/logsink.h"

using namespace gloox;

#include <stdio.h>
#include <locale.h>
#include <string>

#include <cstdio> // [s]print[f]
#include <iostream>

#include <unistd.h>

#include "libiot/GSIOTClient.h"


int main( int /*argc*/, char** /*argv*/ )
{
	//LOGMSG( ">>>>> GSIOT ≥Ã–Ú∆Ù∂Ø <<<<<\r\n\r\n" );
	//LOGMSG( "GSIOT v%s\r\nbuild %s\r\n\r\n", g_IOTGetVersion().c_str(), g_IOTGetBuildInfo().c_str() );
	printf("gsiot start\n");

	GSIOTClient *client = new GSIOTClient( "" );

	if( !client->GetPreInitState() )
	{
		//LOGMSG( "PreInit Failed!" );
		//Sleep( 2000 );
		printf("PreInit Failed!\n");
		sleep(2000);
		return -1;
	}

	client->Run();
	client->Connect();
}
