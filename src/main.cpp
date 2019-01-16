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
	Vulkan vk(window.window);
	while(!UX::run(&window)) {
		int i = 0;
	}
	cleanUp();
}
