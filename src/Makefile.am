bin_PROGRAMS = ghetto ghettod

NCURSES_MENU_CPP = submodules/ncurses-menu/dialogBox.cpp submodules/ncurses-menu/menu.cpp
JSONCONS_HPP = src/jsoncons/json.hpp \
	src/jsoncons/json1.hpp \
	src/jsoncons/json2.hpp \
	src/jsoncons/json_deserializer.hpp \
	src/jsoncons/json_exception.hpp \
	src/jsoncons/json_filter.hpp \
	src/jsoncons/json_input_handler.hpp \
	src/jsoncons/json_output_handler.hpp \
	src/jsoncons/json_reader.hpp \
	src/jsoncons/json_serializer.hpp \
	src/jsoncons/json_structures.hpp \
	src/jsoncons/json_type_traits.hpp \
	src/jsoncons/jsoncons.hpp \
	src/jsoncons/jsoncons_config.hpp \
	src/jsoncons/output_format.hpp \
	src/jsoncons/parse_error_handler.hpp


ghetto_SOURCES = ghetto.cpp $(NCURSES_MENU_CPP) $(JSONCONS_HPP)
#ghetto_HEADERS = $(JSONCONS_HPP)
ghetto_LDADD = $(CURL_LIBS) -lncurses

ghettod_SOURCES = ghettod.cpp  $(JSONCONS_HPP)
#ghettod_HEADERS = $(JSONCONS_HPP)
ghettod_LDADD = $(MHD_LIBS)