#include "testobj.h"
#include <iostream>
testobj::testobj(int something) {
	privateVar = something;
}

void testobj::printSomething() {
	std::cout << privateVar << std::endl;
}