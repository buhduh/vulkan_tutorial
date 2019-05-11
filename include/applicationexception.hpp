#ifndef APPLICATIONEXCEPTION_H
#define APPLICATIONEXCEPTION_H

#include <stdexcept>
#include <string>
#include <string.h>

class ApplicationException : public std::runtime_error {
	public:
	ApplicationException(std::string msg): runtime_error(msg), msg(msg) {}
	virtual const char * what() const throw() {
		return this->msg.c_str();	
	}
	protected:
	std::string msg;
};

class ApplicationVulkanException: public ApplicationException {
	public:
	ApplicationVulkanException(std::string msg)
	: ApplicationException(msg) {}
};

#endif
