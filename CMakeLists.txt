cmake_minimum_required(VERSION 2.6)

INCLUDE(cmake/FindGZIP.cmake)

SET(CMAKE_INSTALL_PREFIX "/usr/local/share")

# PRoject name
PROJECT(luanet)

INCLUDE(FindPkgConfig)

IF(NOT PKG_CONFIG_FOUND)
	message(FATAL_ERROR "You need to install pkg-config. Exiting.")
ENDIF()

pkg_check_modules(LUA REQUIRED lua5.2)

IF(NOT LUA_FOUND)
	message(FATAL_ERROR "You need to install lua5.2. Exiting.")
ENDIF()

pkg_check_modules(LIBLUAWRAP REQUIRED libluawrap)

IF(NOT LIBLUAWRAP_FOUND)
	message(FATAL_ERROR "You need to install libluawrap. Exiting.")
ENDIF()

SET(CMAKE_C_FLAGS "-O2 -s -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DHAVE_ALLOCA_H=1")
SET(CMAKE_CXX_FLAGS "-Wall -O2 -s")

# Paths
SET(BINARY_OUTPUT_PATH ${PROJECT_BINARY_DIR})
SET(LIBRARY_OUTPUT_PATH ${PROJECT_BINARY_DIR}/luanet)

# Includes
INCLUDE_DIRECTORIES("include" ${LIBLUAWRAP_INCLUDE_DIRS} ${LUA_INCLUDE_DIRS})
# Libs directories
LINK_DIRECTORIES(${LUA_LIBRARY_DIRS} ${LIBLUANET_LIBRARY_DIRS})

# Manpages
SET(MAN_PAGES "man/luanet.3")

INCLUDE(cmake/BuildMan.cmake)

# Configuration des fichiers sources
# Lecture de tout les .cpp et .h
FILE(
	GLOB_RECURSE
	headers
	include/*.h
)

# Cible socket
ADD_LIBRARY(        
	socket
	SHARED
	${headers}
	src/client_socket.cpp
	src/luanet_socket.cpp
	src/wrap_client.cpp
)

# Cible misc
ADD_LIBRARY(        
	misc
	SHARED
	${headers}
	src/luanet_misc.cpp
	src/sha1.cpp
)

# Cible sleep
ADD_LIBRARY(        
	sleep
	SHARED
	${headers}
	src/luanet_sleep.cpp
)

# Cible regex
ADD_LIBRARY(        
	regex
	SHARED
	${headers}
	src/luanet_regex.cpp
	src/regex.c
)

# Propriétés des cibles
SET_TARGET_PROPERTIES(
	socket
	PROPERTIES
	PREFIX ""
)
SET_TARGET_PROPERTIES(
	misc
	PROPERTIES
	PREFIX ""
)
SET_TARGET_PROPERTIES(
	sleep
	PROPERTIES
	PREFIX ""
)
SET_TARGET_PROPERTIES(
	regex
	PROPERTIES
	PREFIX ""
)

# Link les libs aux odules
TARGET_LINK_LIBRARIES(
	socket
	luawrap
	${LUA_LIBRARIES}
)
TARGET_LINK_LIBRARIES(
	misc
	luawrap
	${LUA_LIBRARIES}
)
TARGET_LINK_LIBRARIES(
	sleep
	luawrap
	${LUA_LIBRARIES}
)
TARGET_LINK_LIBRARIES(
	regex
	luawrap
	${LUA_LIBRARIES}
)

INSTALL(TARGETS socket misc sleep regex DESTINATION "lua/5.2/luanet")
INSTALL(FILES ${PROJECT_SOURCE_DIR}/luanet/init.lua
             ${PROJECT_SOURCE_DIR}/luanet/irc.lua
             DESTINATION
             "lua/5.2/luanet")
