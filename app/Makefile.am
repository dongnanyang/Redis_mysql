AUTOMAKE_OPTIONS = foreign
bin_PROGRAMS = mr
INCLUDES = -I../_inc -I/usr/include/mysql -I/usr/local/include/hiredis
mr_SOURCES = main.cxx
mr_LDADD = ../src/libsrc.a -lmysqlclient -lhiredis
