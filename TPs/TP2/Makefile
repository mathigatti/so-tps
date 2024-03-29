.POSIX:
.SUFFIXES:
.SUFFIXES: .o .cpp

CXX = g++
CXXFLAGS = -std=c++11
LDLIBS = -lpthread

BIN = test-1 test-2 test-2-mini test-3 test-3-mini test-4 test-4-mini test-5-mini test-5 test-6 test-z tiempos tiempos1 tiempos2 tiempos3
OBJ = ConcurrentHashMap.o locks/Mutex.o

all: $(BIN)

$(BIN): ListaAtomica.hpp 

ConcurrentHashMap.o: ConcurrentHashMap.cpp ConcurrentHashMap.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

locks/Mutex.o: locks/Mutex.cpp locks/Mutex.h
	$(CXX) $(CXXFLAGS) -o $@ -c $<

test-1: $(OBJ) test-1.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-1.cpp $(LDLIBS)
	
test-1-run: test-1
			./test-1

test-2-mini: $(OBJ) test-2-mini.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-2-mini.cpp $(LDLIBS)
	
test-2-mini-run: test-2-mini
			./test-2-mini

test-2: $(OBJ) test-2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-2.cpp $(LDLIBS)
	
test-2-run: test-2
	awk -f corpus.awk corpus | sort >corpus-post
	./test-2 | sort | diff -u - corpus-post
	rm -f corpus-post

test-3-mini: $(OBJ) test-3-mini.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-3-mini.cpp $(LDLIBS)
	
test-3-mini-run: test-3-mini
			./test-3-mini

test-3: $(OBJ) test-3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-3.cpp $(OBJ) $(LDLIBS)

test-3-run: test-3
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-3 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-4-mini: $(OBJ) test-4-mini.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-4-mini.cpp $(LDLIBS)
	
test-4-mini-run: test-4-mini
			./test-4-mini 4
			./test-4-mini 3
			./test-4-mini 2

test-4: $(OBJ) test-4.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-4.cpp $(OBJ) $(LDLIBS)

test-4-run: test-4
	awk -f corpus.awk corpus | sort >corpus-post
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do ./test-4 $$((i + 1)) | sort | diff -u - corpus-post; done
	rm -f corpus-post corpus-[0-4]

test-5-mini: $(OBJ) test-5-mini.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ test-5-mini.cpp $(LDLIBS)
	
test-5-mini-run: test-5-mini
			./test-5-mini 4 4
			./test-5-mini 6 2
			./test-5-mini 5 1
			./test-5-mini 1 1
			./test-5-mini 8 8
			./test-5-mini 2 2


test-5: $(OBJ) test-5.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-5.cpp $(OBJ) $(LDLIBS)

test-5-run: test-5
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	cat corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-5 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

test-6: $(OBJ) test-6.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) -o $@ test-6.cpp $(OBJ) $(LDLIBS)

test-6-run: test-6
	awk -f corpus.awk corpus | sort -nk 2 | tail -n 1 >corpus-max
	cat corpus-max
	for i in 0 1 2 3 4; do sed -n "$$((i * 500 + 1)),$$(((i + 1) * 500))p" corpus >corpus-"$$i"; done
	for i in 0 1 2 3 4; do for j in 0 1 2 3 4; do \
		./test-6 $$((i + 1)) $$((j + 1)) | diff -u - corpus-max; \
	done; done
	rm -f corpus-max corpus-[0-4]

tiempos1: $(OBJ) tiempos1.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ tiempos1.cpp $(LDLIBS)
	
tiempos1-run: tiempos1
			./tiempos1 26 26

tiempos2: $(OBJ) tiempos2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ tiempos2.cpp $(LDLIBS)
	
tiempos2-run: tiempos2
			./tiempos2 26 26

tiempos3: $(OBJ) tiempos3.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ tiempos3.cpp $(LDLIBS)
	
tiempos3-run: tiempos3
			./tiempos3 5 5

generar-palabras1: $(OBJ) generarPalabras1.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ generarPalabras1.cpp $(LDLIBS)
	
generar-palabras-1-run: ./generarPalabras1
			./generarPalabras1 100000

generar-palabras2: $(OBJ) generarPalabras2.cpp
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $(OBJ) -o $@ generarPalabras2.cpp $(LDLIBS)
	
generar-palabras-2-run: ./generarPalabras2
			./generarPalabras2 1000

clean:
	rm -f $(BIN) $(OBJ)
	rm -f corpus-*
	rm -f resultado*

