#ifndef UTILS_H
#define UTILS_H

#include <fstream>
#include <vector>

namespace utils {

	class Uncopyable {
	protected:
		Uncopyable() {}
		~Uncopyable() {}
	private:
		Uncopyable(const Uncopyable&);
		Uncopyable& operator=(const Uncopyable&);
	};

	static std::vector<char> readFile(const std::string& filename) {
		std::ifstream file(filename, std::ios::ate | std::ios::binary);
		if(!file.is_open()) {
			throw std::runtime_error("Failed opening file.");
		}
		size_t fileSize = (size_t) file.tellg();
		std::vector<char> buffer(fileSize);
		file.seekg(0);
		file.read(buffer.data(), fileSize);
		file.close();
		return buffer;
	}

};
#endif
