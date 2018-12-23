SHELL = powershell.exe
CXX = g++
CXXFLAGS = -Wall -O2 

score: score.cc
	$(CXX) -o $@ $^ $(CXXFLAGS)

clean:
	rm -Force *.exe
	rm -Force *.tmp
