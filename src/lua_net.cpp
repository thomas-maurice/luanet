#define LUA_LIB

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

#include <iostream>

#include <libluawrap.h>
#include <client_socket.h>
#include <macros.h>

extern "C" {
	int luaopen_luanet(lua_State* L);
}

struct Test { Client_socket* sock;};

int new_client_socket(lua_State* L)
{
  Test* vd = (Test*)lua_newuserdata(L, sizeof(Test*)); // Alloue un bloc de
  // mémoire de taille qui va bien pour stocker l'objet.

  // On assigne a tout les objets la même metatable
  luaL_getmetatable(L, "socket");
  lua_setmetatable(L, -2); // Puisque l'objet et en position -2 sur la stack

  Client_socket *c = new Client_socket("irc.epiknet.org", 6667);
 	vd->sock = c;

  return 1; // Userdatum on the stack
}


using namespace std;

Test* array_checkvoid(lua_State *L)
{
  void *ud = luaL_checkudata(L, 1, "socket");
  luaL_argcheck(L, ud != NULL, 1, "`socket' expected");
  return (Test*)ud;
}

int test(lua_State* L)
{
	cout << "test" << endl;
	return 0;
}

int echoport(lua_State * L)
{
	Test* vd = array_checkvoid(L);

	cout << vd->sock->get_host() << endl;

	return 0;

}

int __declspec(dllexport) luaopen_luanet(lua_State *L) {
    LuaTable module("net");

    LuaTable metas("socket");

    LuaFunction newSocket("newSocket");
    newSocket.setFunction(new_client_socket);
    module.addField_Function("newSocket", &newSocket);

    LuaTable socket("socket");
    socket.setMetatable(socket);

    LuaFunction testf("test");
    testf.setFunction(test);

    LuaFunction ep("ep");
    ep.setFunction(echoport);
    metas.addField_Function("port", &ep);

    metas.globalize(L, "socket");
    socket.globalize(L, "socket");

    module.globalize(L, "net");

    return 0;
}
