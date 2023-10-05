//
// Created by sad0p on 9/28/23.
//

#ifndef ELFDOC_EXCEPTIONHANDLER_H
#define ELFDOC_EXCEPTIONHANDLER_H

#include <iostream>

class ExceptionHandler {
public:
	ExceptionHandler(int errNum, std::string customMsg) {
		_errNum = errNum;
		_customMsg = customMsg;

	}

	ExceptionHandler(int errNum) {
		_errNum = errNum;
	}

	ExceptionHandler(std::string customMsg) {
		_customMsg = customMsg;
	}

	~ExceptionHandler(){

	};

	void report() const;

private:
	int _errNum = 0;
	std::string _customMsg;
};


#endif //ELFDOC_EXCEPTIONHANDLER_H
