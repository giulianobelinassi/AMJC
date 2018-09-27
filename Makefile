CXX       = g++
CXXFLAGS  = -g -std=c++11 -flto -Wall -pedantic
LDFLAGS   = -lfl

LEX       = flex
LEXFLAGS  = 
YACC      = bison
YACCFLAGS = -v -d

main: lex.yy.o minijava.tab.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

lex.yy.o: lex.yy.c minijava.tab.h
	$(CXX) $(CXXFLAGS) -c $<

minijava.tab.o: minijava.tab.c minijava.tab.h
	$(CXX) $(CXXFLAGS) -c $<

minijava.tab.c minijava.tab.h: minijava.y
	$(YACC) $(YACCFLAGS) $<

lex.yy.c: minijava.l minijava.tab.h
	$(LEX) $(LEXFLAGS) $<

.PHONY: clean
clean:
	rm -f *.o lex.yy.c minijava.tab.c minijava.tab.h main 
