bin_PROGRAMS = ghetto ghettod

ghetto_SOURCES = ghetto.cpp submodules/ncurses-menu/dialogBox.cpp submodules/ncurses-menu/menu.cpp
ghetto_LDADD = $(CURL_LIBS) -lncurses

ghettod_SOURCES = ghettod.cpp
ghettod_LDADD = $(MHD_LIBS)