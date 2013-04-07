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

#ifndef MACROS_HEADER
#define MACROS_HEADER

	#if defined (linux)
		#define DllExport
	#elif defined (WIN32)
		#define DllExport __declspec(dllexport)
	#endif
	
	// Utilisé pour convertir l'objet "obj" de type quelconque en
	// un pointeur sur void "out"
	#define OBJ_TO_VOID(obj, out) void *out = (void*)obj;
	
	// Utilisé pour convertir le pointeur sur void "vd" en un objet
	// "obj" de type "type"
	#define VOID_TO_OBJ(vd, type, out) type *out = (type*)vd;
	
	#define CHECK_UDATA(Type,Func,Meta) \
	Type * Func(lua_State * L) \
	{\
    void *ud = luaL_checkudata(L, 1, Meta);\
    luaL_argcheck(L, ud != NULL, 1, std::string(std::string(Meta)+ "expected").c_str());\
    return (Type*)ud;\
  }
  
  #define CHECK_UDATA_H(Type,Func) \
	Type * Func(lua_State * L);
	
#endif
