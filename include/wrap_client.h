/*

Copyright (C) 2013 Thomas MAURICE

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

#include <client_socket.h>
#include <libluawrap.h>
#include <macros.h>

struct WrapSocket {
	Client_socket* sock;
};

CHECK_UDATA_H(WrapSocket, luanet_check_socket)

int new_client_socket(lua_State* L);
int luanet_client_getPort(lua_State * L);
int luanet_client_getHost(lua_State * L);
int luanet_client_connectSocket(lua_State* L);
int luanet_client_closeSocket(lua_State* L);
int luanet_client_canGetLine(lua_State* L);
int luanet_client_getLine(lua_State* L);
int luanet_client_isConnected(lua_State* L);
int luanet_client_writeLine(lua_State* L);
int luanet_client_write(lua_State* L);
