SOURCES=Ui.cpp main.cpp Block.cpp
PROGNAME=bastet
LDFLAGS+=-lncurses
CXXFLAGS+=-ggdb -Wall

all: $(PROGNAME)

depend: *.hpp $(SOURCES)
	$(CXX) -MM $(SOURCES) > depend

include depend

$(PROGNAME): $(SOURCES:.cpp=.o)
	$(CXX) -ggdb -o $(PROGNAME) $(SOURCES:.cpp=.o) $(LDFLAGS)

clean:
	rm -f $(SOURCES:.cpp=.o) $(PROGNAME)

mrproper: clean
	rm -f *~
