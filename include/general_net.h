/*

Copyright (C) 2011 Thomas MAURICE

This program is free software; you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation; either version 2 of the License, or
(at your option) any later version.

This program is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
GNU General Public License for more details.

You should have received a copy of the GNU General Public License along
with this program; if not, write to the Free Software Foundation, Inc.,
51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.

*/

extern "C"
{
	#if defined (WIN32)
		#include <winsock2.h>

	    	typedef int socklen_t;

	#elif defined (linux)

	    	#include <sys/types.h>
	    	#include <sys/socket.h>
	    	#include <netinet/in.h>
	    	#include <arpa/inet.h>
	    	#include <netdb.h>
	    	#include <unistd.h>

	    	#define INVALID_SOCKET -1
	    	#define SOCKET_ERROR -1
	    	#define closesocket(s) close (s)

	    	typedef int SOCKET;
	    	typedef struct sockaddr_in SOCKADDR_IN;
	    	typedef struct sockaddr SOCKADDR;

	#endif

	#include <stdio.h>
	#include <stdlib.h>
}
