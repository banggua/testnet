CXX = g++
CXXFLAGS =
LDFLAGS =
LDLIBS = -Iutil -luv

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
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBUTS) -o $(LIBUTSOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o uts $(LIBUTSOBJ) $(LDLIBS)
utc: $(RANDPKG) $(LIBUTC)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBUTC) -o $(LIBUTCOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o utc $(LIBUTCOBJ) $(RANDPKGOBJ) $(LDLIBS)
uus: $(LIBUUS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBUUS) -o $(LIBUUSOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o uus $(LIBUUSOBJ) $(LDLIBS)
uuc: $(LIBUUC) $(RANDPKG)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBUUC) -o $(LIBUUCOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(LDLIBS)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o uuc $(LIBUUCOBJ) $(RANDPKGOBJ) $(LDLIBS)
# sts: $(LIBSTS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBSTS) -o $(LIBSTSOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o sts $(LIBSTSOBJ) $(LDLIBS)
# stc: $(LIBSTC) $(RANDPKG)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBSTC) -o $(LIBSTCOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o stc $(LIBSTCOBJ) $(RANDPKGOBJ) $(LDLIBS)
# sus: $(LIBSUS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBSUS) -o $(LIBSUSOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o sus $(LIBSUSOBJ) $(LDLIBS)
# suc: $(LIBSUC) $(RANDPKG)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(LIBSUC) -o $(LIBSUCOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -c $(RANDPKG) -o $(RANDPKGOBJ) $(LDLIBS)
# 	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o suc $(LIBSUCOBJ) $(RANDPKGOBJ) $(LDLIBS)

clean:
	rm -rf $(OBJ) $(EXEC)
