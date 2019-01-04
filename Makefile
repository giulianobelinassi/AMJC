CXX       = g++-6
CXXFLAGS  = -g -std=c++11 -flto -Wall -pedantic
LDFLAGS   = -lfl -lgvc -lcgraph

LEX       = flex
LEXFLAGS  = 
YACC      = bison
YACCFLAGS = -v -d

main: lex.yy.o minijava.tab.o tree.o statement.o expression.o types.o declaration.o symboltable.o x86.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

declaration.o: declaration.cpp declaration.h tree.h expression.h statement.h
	$(CXX) $(CXXFLAGS) -c $<

expression.o: expression.cpp x86.cpp x86.h interp.h compiler.h expression.h tree.h macros.h
	$(CXX) $(CXXFLAGS) -c $<

types.o: types.cpp macros.h
	$(CXX) $(CXXFLAGS) -c $<

statement.o: statement.cpp statement.h tree.h macros.h
	$(CXX) $(CXXFLAGS) -c $<

tree.o: tree.cpp types.cpp tree.h types.h macros.h
	$(CXX) $(CXXFLAGS) -c $<

symboltable.o: symboltable.cpp symboltable.h types.cpp
	$(CXX) $(CXXFLAGS) -c $<

x86.o: x86.cpp x86.h
	$(CXX) $(CXXFLAGS) -c $<

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
	rm -f *.o *.output lex.yy.c *.dot minijava.tab.c minijava.tab.h graph.pdf main
