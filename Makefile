SOURCES=Ui.cpp Block.cpp Well.cpp BlockPosition.cpp Config.cpp BlockChooser.cpp BastetBlockChooser.cpp
MAIN=main.cpp
TESTS=Test.cpp
PROGNAME=bastet
BOOST_PO?=-lboost_program_options
LDFLAGS+=-lncurses $(BOOST_PO)
#CXXFLAGS+=-ggdb -Wall
CXXFLAGS+=-DNDEBUG -Wall
#CXXFLAGS+=-pg
#LDFLAGS+=-pg

all: $(PROGNAME) $(TESTS:.cpp=)

Test: $(SOURCES:.cpp=.o) $(TESTS:.cpp=.o)
	$(CXX) -ggdb -o $(TESTS:.cpp=) $(SOURCES:.cpp=.o) $(TESTS:.cpp=.o) $(LDFLAGS) 

depend: *.hpp $(SOURCES) $(MAIN) $(TESTS)
	$(CXX) -MM $(SOURCES) $(MAIN) $(TESTS)> depend

include depend

$(PROGNAME): $(SOURCES:.cpp=.o) $(MAIN:.cpp=.o)
	$(CXX) -ggdb -o $(PROGNAME) $(SOURCES:.cpp=.o) $(MAIN:.cpp=.o) $(LDFLAGS) 

clean:
	rm -f $(SOURCES:.cpp=.o) $(TESTS:.cpp=.o) $(MAIN:.cpp=.o) $(PROGNAME)

mrproper: clean
	rm -f *~
