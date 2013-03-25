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
#include <wrap_client.h>

extern "C" {
	int DllExport luaopen_luanet_socket(lua_State* L);
}

using namespace std;

int luaopen_luanet_socket(lua_State *L) {
  #if defined (WIN32)
    WSADATA WSAData;
    int erreur = WSAStartup(MAKEWORD(2,2), &WSAData);
    if(erreur)
    {
        perror("WSAStartup");
        WSACleanup();
        return 0;
    }
  #endif
  // Création des tables du module
  LuaTable module("luanet");
  LuaTable socket("luanet.socket");
  socket.setMetatable(socket);

  // Création des fonctions de module
  LuaFunction newSocket("newSocket");

  // Assignation des fonctions du module
  newSocket.setFunction(new_client_socket);

  // Enregistrement dans le module
  module.addField_Function("newSocket", &newSocket);


  // Création des fonctions de la classe
  LuaFunction getPort("getPort");
  LuaFunction getHost("getHost");
  LuaFunction connectSocket("connectSocket");
  LuaFunction closeSocket("closeSocket");
  LuaFunction isConnected("isConnected");
  LuaFunction canGetLine("canGetLine");
  LuaFunction getLine("getLine");
  LuaFunction writeLine("writeLine");
  LuaFunction write("write");

  // Assignation des fonctions de la classe
  getPort.setFunction(luanet_client_getPort);
  getHost.setFunction(luanet_client_getHost);
  connectSocket.setFunction(luanet_client_connectSocket);
  closeSocket.setFunction(luanet_client_closeSocket);
  isConnected.setFunction(luanet_client_isConnected);
  canGetLine.setFunction(luanet_client_canGetLine);
  getLine.setFunction(luanet_client_getLine);
  writeLine.setFunction(luanet_client_writeLine);
  write.setFunction(luanet_client_write);

  // Enregistrement dans la classe
  socket.addField_Function("getHost", &getHost);
  socket.addField_Function("getPort", &getPort);
  socket.addField_Function("connectSocket", &connectSocket);
  socket.addField_Function("closeSocket", &closeSocket);
  socket.addField_Function("isConnected", &isConnected);
  socket.addField_Function("canGetLine", &canGetLine);
  socket.addField_Function("getLine", &getLine);
  socket.addField_Function("writeLine", &writeLine);
  socket.addField_Function("write", &write);

  // Globalisation des tables
  socket.globalize(L, "luanet.socket");
  module.globalize(L, "luanet");

  return 0;
}
