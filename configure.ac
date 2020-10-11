#                                               -*- Autoconf -*-
# Process this file with autoconf to produce a configure script.

AC_PREREQ([2.69])
AC_INIT(mr, 1.0, 3192818988@qq.com)
AC_CONFIG_SRCDIR([app/main.cxx])
AC_CONFIG_HEADERS([config.h])
:$(CXXFLAGS="-g -gdb -std=C++14")
AM_INIT_AUTOMAKE
# Checks for programs.
AC_PROG_CXX
AC_PROG_CC

# Checks for libraries.
AC_PROG_RANLIB
# FIXME: Replace `main' with a function in `-lmysqlclient':
AC_CHECK_LIB([mysqlclient],[hiredis], [main])

# Checks for header files.
AC_CHECK_HEADERS([string.h])

# Checks for typedefs, structures, and compiler characteristics.
AC_CHECK_HEADER_STDBOOL
AC_TYPE_SIZE_T

# Checks for library functions.
CXXFLAGS="$CXXFLAGS -std=c++0x"
AC_CONFIG_FILES([Makefile
                 _inc/Makefile
                 app/Makefile
                 src/Makefile])
AC_OUTPUT
