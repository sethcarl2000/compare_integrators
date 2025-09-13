CPPFLAGS=

test_integration: integrators.o test_integration.cpp
	g++ ${CPPFLAGS} integrators.o test_integration.cpp -o test_integration
# t5rrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrrtghyuion: test_integrators.hpp integrators.cpp

integrators.o: integrators.hpp integrators.cpp  
	g++ ${CPPFLAGS} -c integrators.cpp -o integrators.o

