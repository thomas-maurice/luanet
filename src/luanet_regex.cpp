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

extern "C" {
#include <regex.h>
	int luaopen_luanet_regex(lua_State* L);
}

using namespace std;

bool regex_match(std::string chaine, std::string expression)
{
    int err;
    regex_t preg;
    int match = 0;
    
    err = regcomp (&preg, expression.c_str(), REG_NOSUB | REG_EXTENDED);
    if(err == 0)
    {
        match = regexec(&preg, chaine.c_str(), 0, NULL, 0);
        regfree(&preg);
		
		if(match == REG_NOMATCH)
			return false;
		else
			return true;
    }
    else
    {
        regfree(&preg);
        return false;
    }
	return false;
}

int luanet_match(lua_State* L)
{
  string s = luaL_checkstring(L, 1);
  string p = luaL_checkstring(L, 2);
  
  LuaBoolean b(regex_match(s, p));
  
  b.push(L);
  
  return 1;
}

int luaopen_luanet_regex(lua_State *L) {
  LuaTable module("luanet");
  LuaTable regex("regex");
  module.addField_Table("regex", &regex);
  
  LuaFunction match("match");
  match.setFunction(&luanet_match);
  
  regex.addField_Function("match", &match);
  
  module.globalize(L, "luanet");
    
  return 0;
}
