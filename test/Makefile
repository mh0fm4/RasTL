
RTL_INCLUDE_DIR:=../include

EXEC:=poly

CPPFLAGS:=-I$(RTL_INCLUDE_DIR)
CXX:=g++
CXXFLAGS:=-Wall -ggdb
LDFLAGS:=-Wall -ggdb

HXXFILES:=$(wildcard *.hpp $(RTL_INCLUDE_DIR)/*.hpp)
CXXFILES:=$(wildcard *.cpp)
OFILES:=$(addsuffix .o,$(basename $(CXXFILES)))

all: $(EXEC)

%.o: %.cpp $(HXXFILES) Makefile
	$(CXX) $(CPPFLAGS) $(CXXFLAGS) -c -o $@ $<

poly: $(OFILES)
	g++ $(LDFLAGS) -o $@ $(OFILES)

clean:
	rm -f $(OFILES) $(EXEC)
