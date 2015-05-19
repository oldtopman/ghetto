#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

# Can probably go earlier than 2.69
AC_PREREQ([2.69])

AC_INIT([ghetto], [0.0.1], [oldtopman@gmail.com])
AC_CONFIG_SRCDIR([src/ghetto/ghetto.cpp])
AC_CONFIG_HEADERS([src/config.h])

AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE

#Checks for compiler capabilities.
AX_CXX_COMPILE_STDCXX_11

# Checks for programs.
AC_PROG_CC
AC_PROG_CXX

# Checks for libraries.
AC_CHECK_LIB([ncurses], [initscr])

PKG_CHECK_MODULES([CURL], [libcurl])
#Don't use pkg-config for ncurses, it's not reliable
PKG_CHECK_MODULES([MHD], [libmicrohttpd])

# Checks for header files.
AC_CHECK_HEADERS([stdlib.h string.h sys/socket.h unistd.h])
AC_CHECK_HEADERS([ncurses.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_C_INLINE
AC_TYPE_SIZE_T
AC_TYPE_UINT16_T
AC_TYPE_UINT32_T
AC_CHECK_TYPES([ptrdiff_t])

# Checks for library functions.
AC_FUNC_ERROR_AT_LINE
AC_CHECK_FUNCS([gethostname mkdir])

AC_CONFIG_FILES([Makefile
                 src/Makefile
                 src/ghetto/Makefile
                 src/ghettod/Makefile])
AC_OUTPUT
