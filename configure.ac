#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Can probably go earlier than 2.69
AC_PREREQ([2.69])

AC_INIT([ghetto], [0.0.1], [oldtopman@gmail.com])
AC_CONFIG_SRCDIR([src/ghetto.cpp])
AC_CONFIG_HEADERS([src/config.h])

AM_INIT_AUTOMAKE

# Checks for programs.
AC_PROG_CXX

# Checks for libraries.

PKG_CHECK_MODULES([CURL], [libcurl])
#Don't use pkg-config for ncurses, it's not reliable

# Checks for header files.
AC_CHECK_HEADERS([ncurses.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
