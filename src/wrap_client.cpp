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

#include <wrap_client.h>

using namespace std;

CHECK_UDATA(WrapSocket, luanet_check_socket, "luanet.socket")

int new_client_socket(lua_State* L)
{
  string host = luaL_checkstring(L, 1);
  int port = luaL_checkint(L, 2);
  WrapSocket* s = (WrapSocket*)lua_newuserdata(L, sizeof(WrapSocket*)); // Alloue un bloc de
  // mémoire de taille qui va bien pour stocker l'objet.

  // On assigne a tout les objets la même metatable
  luaL_getmetatable(L, "luanet.socket");
  lua_setmetatable(L, -2); // Puisque l'objet et en position -2 sur la stack

  Client_socket *c = new Client_socket(host, port);
 	s->sock = c;

  return 1; // Userdatum on the stack
}

int luanet_client_getPort(lua_State * L)
{
	WrapSocket* s = luanet_check_socket(L);

	LuaNumber n(s->sock->get_port());
	n.push(L);

	return 1;
}

int luanet_client_getHost(lua_State * L)
{
	WrapSocket* s = luanet_check_socket(L);

	LuaString str(s->sock->get_host());
	str.push(L);

	return 1;
}

int luanet_client_connectSocket(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);
  s->sock->connect_socket();

  return 0;
}

int luanet_client_isConnected(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);
  LuaBoolean b(s->sock->is_connected());
  b.push(L);

  return 1;
}

int luanet_client_closeSocket(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);

  s->sock->close_socket();

  return 0;
}

int luanet_client_writeLine(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);

  string str = luaL_checkstring(L, 2);

  LuaNumber ret(s->sock->write_line(str));
  ret.push(L);

  return 1;
}

int luanet_client_write(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);
  string str = luaL_checkstring(L, 2);

  LuaNumber ret(s->sock->write(str));
  ret.push(L);

  return 1;
}

int luanet_client_canGetLine(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);
  LuaBoolean b(s->sock->can_getline());
  b.push(L);

  return 1;
}

int luanet_client_getLine(lua_State* L)
{
  WrapSocket* s = luanet_check_socket(L);
  LuaString str(s->sock->getline());
  str.push(L);

  return 1;
}

