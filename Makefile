# Makefile
# Date : Mercredi 18 janvier 2012
# 
# Cibles : 
#         - all     : Créé le programme executable
#         - strip   : Strip l'executable
#         - clean   : Nettoie les fichiers objets
#         - nuke    : Nettoie les fichiers objets
#                     et l'éxécutable
#         - deb     : Crée le paquet .deb (nécessite
#                     le programme makedeb)
#         - rebuild : nuke et reconstruit le projet
#         - dist    : Archive les sources en tgz
#         - manps   : Crée la page man en .ps
#         - mangz   : Crée la page man en .gz

# Réglages ###########################################

# Nom du compilateur
COMPILER=g++
# Répertoire des fichiers sources
SRCDIR=./src
INCLUDEDIR=./include
# Flags à passer au compilo
FLAGS=-Wall -I $(INCLUDEDIR) -I include/libluawrap -Os -fexpensive-optimizations $(shell pkg-config --cflags lua5.2 libluawrap)
# Flags à passer à l'édition de liens
LDFLAGS=$(shell pkg-config --libs lua5.2) -lluawrap
# Extention des fichiers source (ex 'cpp' pour le C++)
SRCEXT=cpp
# Nom du package
EXEC=luanet
# Nom de la lib
# Socket
SOCKET=luanet/socket.so
SOCKET_SRC=src/luanet_socket.cpp src/client_socket.cpp src/wrap_client.cpp
# Sleep
SLEEP=luanet/sleep.so
SLEEP_SRC=src/luanet_sleep.cpp
# Misc
MISC=luanet/misc.so
MISC_SRC=src/luanet_misc.cpp src/sha1.cpp
# Regex
REGEX=luanet/regex.so
REGEX_SRC=src/regex.c src/luanet_regex.cpp
# Man configuration
# Program's category, for instance 1 is a program
# And 3 a library.
CATEGORY=3
VERSION=0.1
# The manpage filename
MANDEPS=man/luanet.$(CATEGORY)
MANGZ=man/luanet.$(CATEGORY).gz
MANPS=man/luanet.$(CATEGORY).ps

# Makedeb configuration
ifneq ("$(shell which makedeb)","")
USE_MAKEDEB=1
MAKEDEB_SUFFIX=$(shell makedeb --suffix)
PACKAGE_NAME=$(shell basename `pwd`)
else
USE_MAKEDEB=0
endif

# Installation
INSTALL_LUALN=/usr/share/lua/5.2/luanet
INSTALL_LIBLN=/usr/lib/lua/5.2/luanet
INSTALL_MAN=/usr/local/man/man3

INSTALL= install -p
INSTALL_EXEC= $(INSTALL) -m 0755
INSTALL_DATA= $(INSTALL) -m 0644
INSTALL_DIR= cp -r 

MKDIR= mkdir -p
RM= rm -f
RMD= rm -fr

# Fin des réglages ###################################

# Auto-options #######################################

# Répertoire temporaire pour l'archivage
DISTDIR=$(shell basename `pwd`)-src-$(VERSION)
# Fichiers objet
SOCKET_OBJ=$(SOCKET_SRC:.$(SRCEXT)=.o)
SLEEP_OBJ=$(SLEEP_SRC:.$(SRCEXT)=.o)
MISC_OBJ=$(MISC_SRC:.$(SRCEXT)=.o)
REGEX_OBJ1=$(REGEX_SRC:.cpp=.o)
REGEX_OBJ=$(REGEX_OBJ1:.c=.o)

# Fin auto-options ###################################

# Cibles spéciales

all: $(SOCKET) $(SLEEP) $(MISC) $(REGEX) $(MANGZ)

strip: all
	strip -s $(SOCKET)
	strip -s $(SLEEP)
	strip -s $(MISC)
	strip -s $(REGEX)

rebuild: nuke all

install: strip all
	$(MKDIR) $(INSTALL_LUALN) $(INSTALL_LIBLN) $(INSTALL_MAN)
	$(INSTALL) luanet/init.lua $(INSTALL_LUALN)/init.lua
	$(INSTALL) luanet/irc.lua $(INSTALL_LUALN)/irc.lua
	$(INSTALL) luanet/socket.so $(INSTALL_LIBLN)/socket.so
	$(INSTALL) luanet/sleep.so $(INSTALL_LIBLN)/sleep.so
	$(INSTALL) luanet/regex.so $(INSTALL_LIBLN)/regex.so
	$(INSTALL) luanet/misc.so $(INSTALL_LIBLN)/misc.so
	$(INSTALL) man/luanet.3.gz $(INSTALL_MAN)/luanet.3.gz

uninstall: all
	$(RMD) $(INSTALL_LIBLN) $(INSTALL_LUALN)
	$(RM) $(INSTALL_MAN)/luanet.3.gz

# Makedeb configuration
ifneq ("$(shell which makedeb)","")
USE_MAKEDEB=1
MAKEDEB_SUFFIX=$(shell makedeb --suffix)
endif

$(SOCKET): $(SOCKET_OBJ)
	if ! [ -d "luanet" ]; then mkdir luanet; fi;
	g++ -shared -fPIC $(SOCKET_OBJ)  $(LDFLAGS) -o $(SOCKET)

$(SLEEP): $(SLEEP_OBJ)
	if ! [ -d "luanet" ]; then mkdir luanet; fi;
	g++ -shared -fPIC $(SLEEP_OBJ)  $(LDFLAGS) -o $(SLEEP)

$(MISC): $(MISC_OBJ)
	if ! [ -d "luanet" ]; then mkdir luanet; fi;
	g++ -shared -fPIC $(MISC_OBJ)  $(LDFLAGS) -o $(MISC)

$(REGEX): $(REGEX_OBJ)
	if ! [ -d "luanet" ]; then mkdir luanet; fi;
	g++ -shared -fPIC $(REGEX_OBJ)  $(LDFLAGS) -o $(REGEX)

# Fichiers objets

%.o: %.$(SRCEXT)
	$(COMPILER) $(FLAGS) -c $< -o $@

%.o: %.c
	gcc -I include -Os -fexpensive-optimizations -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DHAVE_ALLOCA_H=1 -c $< -o $@

# Package deb

ifeq ($(USE_MAKEDEB),1)
deb: all strip
	makedeb
	if [ "`makedeb --autoexec`" = "0" ]; then \
		./$(PACKAGE_NAME)$(MAKEDEB_SUFFIX); \
	fi;
endif

# Pages de man
mangz: $(MANGZ)
$(MANGZ): $(MANDEPS)
	gzip -c $(MANDEPS) > $(MANGZ)

ifneq ($(shell which groff),)
# Alias :
manps: $(MANPS)
$(MANPS): $(MANDEPS)
	groff -K utf8 -t -e -mandoc -Tps $(MANDEPS) > $(MANPS)
endif

# Clean & nuke

clean:
	rm -f $(SRCDIR)/*.o
	rm -f man/*.gz man/*.ps
ifeq ($(USE_MAKEDEB),1)
	if [ -e ./$(PACKAGE_NAME)$(MAKEDEB_SUFFIX) ]; then \
		 ./$(PACKAGE_NAME)$(MAKEDEB_SUFFIX) --clean; \
	fi;
endif

nuke: clean
	rm -f $(SOCKET)
	rm -f $(SLEEP)
	rm -f $(MISC)
	rm -f $(REGEX)
	rm -f $(DISTDIR).tgz

# Archivage

dist: nuke
	if ! [ -d $(DISTDIR) ]; then \
		mkdir $(DISTDIR); \
	fi;\
	if ! [ -d $(DISTDIR)/$(SRCDIR) ]; then \
		mkdir $(DISTDIR)/$(SRCDIR); \
	fi;\
	if ! [ -d $(DISTDIR)/$(INCLUDEDIR) ]; then \
		mkdir $(DISTDIR)/$(INCLUDEDIR); \
	fi;\
	if [ -e "INSTALL" ]; then cp INSTALL $(DISTDIR); fi;
	if [ -e "LICENSE" ]; then cp LICENSE $(DISTDIR); fi;
	if [ -e "AUTHORS" ]; then cp AUTHORS $(DISTDIR); fi;
	if [ -e "COPYING" ]; then cp COPYING $(DISTDIR); fi;
	if [ -d "man"     ]; then cp -r man  $(DISTDIR); fi;
ifeq ($(USE_MAKEDEB),1)
	if [ -e "$(PACKAGE_NAME).xml"   ]; then cp *.xml   $(DISTDIR); fi;
endif
	cp $(SRCDIR)/*.$(SRCEXT) $(DISTDIR)/$(SRCDIR)
	cp -r luanet $(DISTDIR)/luanet
	cp exemple.lua $(DISTDIR)
	cp -r $(INCLUDEDIR) $(DISTDIR)
	cp Makefile $(DISTDIR)
	tar zcvf $(DISTDIR).tgz $(DISTDIR)/
	rm -r $(DISTDIR)
