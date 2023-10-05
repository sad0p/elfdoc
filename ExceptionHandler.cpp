//
// Created by sad0p on 9/28/23.
//
#include <cstring>
//#include <errno.h>
#include <iostream>
#include <memory>
#include "ExceptionHandler.h"

void ExceptionHandler::report() const {
	if (_errNum)
		std::cerr << "Error: " << strerror(_errNum) << std::endl;

	if (!_customMsg.empty())
		std::cerr << "Custom error msg: " << _customMsg << std::endl;
}