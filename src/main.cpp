#include <cstdio>
#include <cstdlib>

#include "window.h"
#include "graphics.h"

Window& window = Window::getWindow();
int main() {
	Vulkan vk(window);
	while(window.run()) {
		int i = 0;
	}
}
