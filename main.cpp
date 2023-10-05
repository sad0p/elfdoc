#include <iostream>
#include "ParseELF.h"
#include "Detection.h"

constexpr int ERROR = -1;
constexpr int INFECTED = 1;
constexpr int NO_INFECTION = 0;

int main(int argc, char **argv)
{
	if(argc < 2) {
		std::cout << argv[0] << " <path-to-elf>" << std::endl;
		return 1;
	}
	std::string elfPath = argv[1];
	std::unique_ptr<ParseELF> parser;
	try {
		parser = std::make_unique<ParseELF>(elfPath);
	}catch(ExceptionHandler e) {
		e.report();
		exit(ERROR);
	}

	Detection detect(const_cast<const ParseELF&>(*parser));
	detect.scan();

	return 0;
}
