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
#include <ctime>
#include <cstdlib>
#include <dirent.h>
#include <libluawrap.h>
#include <sha1.h>

extern "C" {
	int luaopen_luanet_misc(lua_State* L);
}

using namespace std;

std::string uppercase(std::string input)
{
    char maj[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char min[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    string ret = "";
    for(unsigned int i = 0 ; i < input.length() ; i++)
    {
        bool found = false;
        int index = -1;
        for(unsigned int j = 0 ; j < 26 ; j++)
        {
            if(input.at(i) == min[j])
            {
                found = true;
                index = j;
                break;
            }
        }
        if(found)
            ret += maj[index];
        else 
            ret += input.at(i);
    }
    
    return ret;
    
}

std::string lowercase(std::string input)
{
    char maj[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
    char min[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    string ret = "";
    for(unsigned int i = 0 ; i < input.length() ; i++)
    {
        bool found = false;
        int index = -1;
        for(unsigned int j = 0 ; j < 26 ; j++)
        {
            if(input.at(i) == maj[j])
            {
                found = true;
                index = j;
                break;
            }
        }
        if(found)
            ret += min[index];
        else 
            ret += input.at(i);
    }
    
    return ret;
    
}

int random_number(int min, int max)
{
    srand(time(NULL));
    int result = 0;
    result = (rand() % (max - min + 1)) + min;
    
    return result;
}

std::string random_string(int len, bool majs, bool num)
{
	srand(time(NULL));
  string min = "azertyuiopqsdfghjklmwxcvbn";
	string maj = "AZERTYUIOPQSDFGHJKLMWXCVBN";
	string cf = "0123456789";
	string res = "";
	
	if(majs)
		min += maj;
	
	if(num)
		min += cf;
	
    const int MAX = min.length() - 1, MIN = 0;

    for(int i = 0; i < len ; i++)
	{
		int in = (rand() % (MAX - MIN + 1)) + MIN;
		res += min.at(in);
	}
	
	return res;
}

std::string sha1(std::string input)
{
	string res = "";
	unsigned message_digest[5];
	SHA1 sha;
	sha.Reset();
	
	for(unsigned int i = 0; i < input.length(); i++)
	{
		sha.Input(input.at(i));
	}
	
	if(!sha.Result(message_digest))
	{
		return "";
	}
	else
	{
		char buf[100];
		sprintf(buf, "%08X%08X%08X%08X%08X",
					message_digest[0],
                    message_digest[1],
                    message_digest[2],
                    message_digest[3],
                    message_digest[4]);
		
		res = buf;
	}
	
	return lowercase(res);
}

int luanet_sha1(lua_State* L)
{
  string s = luaL_checkstring(L, 1);
  
  LuaString hash(sha1(s));
  
  hash.push(L);
  
  return 1;
}

int luanet_listdir(lua_State* L) {
	
	string direc = luaL_checkstring(L, 1);
	LuaTable res("resultat");
		
	DIR* dir = opendir(direc.c_str());
  int count = 1;
  
  if(dir != NULL)
  {
    dirent *fic = NULL;
		while((fic = readdir(dir)) != NULL)
		{
			if(string(fic->d_name) != ".." && string(fic->d_name) != ".")
			{
				res.addIndex_String(count, fic->d_name);
				count++;
			}
		}
  }
  else
  {
     LuaNil nil;
     nil.push(L);
     return 1;
  }
  
  res.push(L);
  
  return 1;
}

int luanet_random(lua_State *L){
	int min,max;
	min = luaL_checknumber(L, 1);
	max = luaL_checknumber(L, 2);
	
	LuaNumber n(random_number(min,max));
	
	n.push(L);
	
	return 1;
}

int luanet_randstring(lua_State *L) {
	int size = luaL_checknumber(L, 1);
	bool majs = true;
	bool nums = true;
	
	if(lua_isboolean(L, 2)) majs = lua_toboolean(L, 2);
	if(lua_isboolean(L, 3)) nums = lua_toboolean(L, 3);

  LuaString r(random_string(size, majs, nums));

	r.push(L);
	
	return 1;
}

int luaopen_luanet_misc(lua_State *L) {
  LuaTable module("luanet");
  LuaTable misc("misc");
  module.addField_Table("misc", &misc);
  
  LuaFunction listdir("listdir");
  listdir.setFunction(&luanet_listdir);
  
  LuaFunction rstring("randomString");
  rstring.setFunction(&luanet_randstring);
  
  LuaFunction rd("randomString");
  rd.setFunction(&luanet_random);
  
  misc.addField_Function("listdir", &listdir);
  misc.addField_Function("randomString", &rstring);
  misc.addField_Function("random", &rd);
  
  LuaFunction sha("sha1");
  sha.setFunction(&luanet_sha1);
  
  misc.addField_Function("sha1", &sha);
  
  module.globalize(L, "luanet");
    
  return 0;
}
