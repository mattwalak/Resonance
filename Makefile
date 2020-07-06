CXXFLAGS= -std=c++14 -O2 -W -Wall -Wextra -pedantic 

reson: resonance.o Util.h
	$(CXX) $(CXXFLAGS) resonance.o -o $@

resonance.o: resonance.cpp
	$(CXX) $(CXXFLAGS) -Ipng++-0.2.9 -c resonance.cpp -o $@

clean:
	rm -f resonance.o reson

.PHONY: clean
