CXX = g++
CXXFLAGS =
INCLUDES = -I$(VCPKG_INCLUDE) -Iutil
LIBRARYS = -L$(VCPKG_LIB) -llibuv -lpthread

LIBUT = libuv/tcp
LIBUTS = $(LIBUT)/server.cc
LIBUTSOBJ = $(LIBUT)/server.o
LIBUTC = $(LIBUT)/client.cc
LIBUTCOBJ = $(LIBUT)/client.o
LIBUU = libuv/udp
LIBUUS = $(LIBUU)/server.cc
LIBUUSOBJ = $(LIBUU)/server.o
LIBUUC = $(LIBUU)/client.cc
LIBUUCOBJ = $(LIBUU)/client.o
LIBST = std/tcp
LIBSTS = $(LIBST)/server.cc
LIBSTSOBJ = $(LIBST)/server.o
LIBSTC = $(LIBST)/client.cc
LIBSTCOBJ = $(LIBST)/client.o
LIBSU = std/udp
LIBSUS = $(LIBSU)/server.cc
LIBSUSOBJ = $(LIBSU)/server.o
LIBSUC = $(LIBSU)/client.cc
LIBSUCOBJ = $(LIBSU)/client.o
UTIL = util
RANDPKG = $(UTIL)/randpkg.cc
RANDPKGOBJ = $(UTIL)/randpkg.o

SRC = $(RANDPKG) $(LIBUTS) $(LIBUTC) $(LIBUUS) $(LIBUUC)
# $(LIBSTS) $(LIBSTC) $(LIBSUS) $(LIBSUC)
OBJ = $(SRC:.cc=.o)
EXEC = uts utc uus uuc
# sts stc sus suc

all: $(EXEC)

uts: $(LIBUTS)
	$(CXX) $(CXXFLAGS) -c $(LIBUTS) -o $(LIBUTSOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o uts $(LIBUTSOBJ)  $(INCLUDES) $(LIBRARYS)
utc: $(RANDPKG) $(LIBUTC)
	$(CXX) $(CXXFLAGS) -c $(LIBUTC) -o $(LIBUTCOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o utc $(LIBUTCOBJ) $(RANDPKGOBJ) $(INCLUDES) $(LIBRARYS)
uus: $(LIBUUS)
	$(CXX) $(CXXFLAGS) -c $(LIBUUS) -o $(LIBUUSOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o uus $(LIBUUSOBJ) $(INCLUDES) $(LIBRARYS)
uuc: $(LIBUUC) $(RANDPKG)
	$(CXX) $(CXXFLAGS) -c $(LIBUUC) -o $(LIBUUCOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(INCLUDES)
	$(CXX) $(CXXFLAGS) -o uuc $(LIBUUCOBJ) $(RANDPKGOBJ) $(INCLUDES) $(LIBRARYS)
# sts: $(LIBSTS)
# 	$(CXX) $(CXXFLAGS) -c $(LIBSTS) -o $(LIBSTSOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -o sts $(LIBSTSOBJ) $(INCLUDES) $(LIBRARYS)
# stc: $(LIBSTC) $(RANDPKG)
# 	$(CXX) $(CXXFLAGS) -c $(LIBSTC) -o $(LIBSTCOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -o stc $(LIBSTCOBJ) $(RANDPKGOBJ) $(INCLUDES) $(LIBRARYS)
# sus: $(LIBSUS)
# 	$(CXX) $(CXXFLAGS) -c $(LIBSUS) -o $(LIBSUSOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -o sus $(LIBSUSOBJ) $(INCLUDES) $(LIBRARYS)
# suc: $(LIBSUC) $(RANDPKG)
# 	$(CXX) $(CXXFLAGS) -c $(LIBSUC) -o $(LIBSUCOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(INCLUDES)
# 	$(CXX) $(CXXFLAGS) -o suc $(LIBSUCOBJ) $(RANDPKGOBJ) $(INCLUDES) $(LIBRARYS)

clean:
	rm -rf $(OBJ) $(EXEC)
