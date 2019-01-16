#include <cstdio>
#include <cstdlib>

#include "window.h"
#include "graphics.h"

UX::Window window;

void cleanUp() {
	UX::cleanUp(&window);
}

int main() {
	window = UX::Window{};
	UX::initWindow(&window);
	//vk = new Graphics::Vulkan(error, window.window);
	//if(error != OK) {
		//TODO what?
		//exit(1);
	//}
	while(!UX::run(&window)) {
		int i = 0;
	}
	cleanUp();
}
