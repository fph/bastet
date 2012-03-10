SOURCES=Ui.cpp main.cpp Block.cpp Well.cpp BlockPosition.cpp Config.cpp BlockChooser.cpp BastetBlockChooser.cpp
CC=g++
TESTS=Test.cpp
PROGNAME=bastet
LDFLAGS+=-lncurses -lboost_program_options
#CXXFLAGS+=-ggdb -Wall
CXXFLAGS+=-DNDEBUG -Wall
#CXXFLAGS+=-pg
#LDFLAGS+=-pg

all: $(PROGNAME) $(TESTS:.cpp=)

Test: Well.o BlockPosition.o Block.o

depend: *.hpp $(SOURCES) $(TESTS)
	$(CXX) -MM $(SOURCES) $(TESTS)> depend

include depend

$(PROGNAME): $(SOURCES:.cpp=.o)
	$(CXX) -ggdb -o $(PROGNAME) $(SOURCES:.cpp=.o) $(LDFLAGS) 

clean:
	rm -f $(SOURCES:.cpp=.o) $(PROGNAME)

mrproper: clean
	rm -f *~
