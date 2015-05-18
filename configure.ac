#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Can probably go earlier than 2.69
AC_PREREQ([2.69])

AC_INIT([ghetto], [0.0.1], [oldtopman@gmail.com])
AC_CONFIG_SRCDIR([src/ghetto.cpp])
AC_CONFIG_HEADERS([src/config.h])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

#Checks for compiler capabilities.
AC_PROG_CXX

#We need a way to get CXX11 going here. This isn't working in 2.69 for some reason.
AX_CXX_COMPILE_STDCXX_11

# Checks for programs.

# Checks for libraries.

PKG_CHECK_MODULES([CURL], [libcurl])
#Don't use pkg-config for ncurses, it's not reliable
PKG_CHECK_MODULES([MHD], [libmicrohttpd])

# Checks for header files.
AC_CHECK_HEADERS([ncurses.h])

# Checks for typedefs, structures, and compiler characteristics.

# Checks for library functions.

AC_CONFIG_FILES([Makefile
                 src/Makefile])
AC_OUTPUT
