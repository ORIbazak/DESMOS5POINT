#pragma once
#include "imports.h"
#include "DATA.h"
class CLI
{
private:
	string IP; //holds ip
	int port;//holds port


public:
	SOCKET conn; //holds connection to client

	CLI();
	bool connectToServer(void);
	
};

