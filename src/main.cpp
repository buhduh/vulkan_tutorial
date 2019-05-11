#include <cstdio>
#include <cstdlib>

#include "window.hpp"
#include "graphics.hpp"

Window& window = Window::getWindow();
int main() {
	Vulkan vk(window);
	while(window.run()) {
		int i = 0;
	}
}
