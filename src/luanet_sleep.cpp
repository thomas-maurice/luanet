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
#include <macros.h>

#include <libluawrap.h>
#ifdef _WIN32
	#include <windows.h>
#else
	#include <unistd.h>
#endif

extern "C" {
	int DllExport luaopen_luanet_sleep(lua_State* L);
}

#ifdef _WIN32
int luanet_sleep(lua_State *L)
{
  int milliseconds = luaL_checkint(L, 1);
	Sleep(milliseconds);
	return 0;
}
#else
int luanet_sleep(lua_State* L)
{
  int milliseconds = luaL_checkint(L, 1);
	usleep(milliseconds * 1000); // takes microseconds
	return 0;
}
#endif

using namespace std;

int luaopen_luanet_sleep(lua_State *L) {
  LuaTable module("luanet");
  LuaFunction sleep("sleep");
  sleep.setFunction(&luanet_sleep);

  module.addField_Function("sleep", &sleep);

  module.globalize(L, "luanet");

  return 0;
}
