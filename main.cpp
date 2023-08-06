#include <iostream>
#include "window.h"


// Physics sim 1 of 5: Projectile motion

/*  User input :
    E: turn gravity on
    SPACE: throw block with gravity
    R: reset block (currently stops block doesn't reset)

*/

int main() {
        
    int x = 1;
	bool displayState = true;
    while (x) {
        int result = CreateWindow(displayState);
        x -= 1;
    }


	std::cin.get();
}