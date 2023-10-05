//
// Created by sad0p on 10/4/23.
//

#ifndef ELFDOC_DETECTION_H
#define ELFDOC_DETECTION_H

#include <elf.h>
#include "ParseELF.h"

class Detection {
public:
	Detection(std::shared_ptr<ParseELF> parser) {
		_parser = parser;
	}

	void scan() const;

private:
	std::shared_ptr<ParseELF> _parser;
	int embeddedPayload() const;
	int textSegmentPadding() const;
};


#endif //ELFDOC_DETECTION_H
