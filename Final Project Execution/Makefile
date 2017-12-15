main: main.cpp engine clause spine toks intmap imap prog
	g++ -o main main.cpp engine.o clause.o Spine.o toks.o intmap.o imap.o prog.o

engine: Engine.cpp 
	g++ -c -o engine.o Engine.cpp

clause: clause.cpp clause.h
	g++ -c -o clause.o clause.cpp

spine: Spine.cpp Spine.h
	g++ -c -o Spine.o Spine.cpp

toks: toks.cpp toks.h
	g++ -c -o toks.o toks.cpp

intmap: IntMap.cpp IntMap.h
	g++ -c -o intmap.o IntMap.cpp

imap: IMap.cpp IMap.h intmap.o
	g++ -c -o imap.o IMap.cpp

imap_exe: IMap.cpp  intmap.o
	g++ -o imap6 IMap.cpp intmap.o

prog: prog.cpp
	g++ -c -o prog.o prog.cpp
