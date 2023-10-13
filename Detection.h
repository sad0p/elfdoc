//
// Created by sad0p on 10/4/23.
//

#ifndef ELFDOC_DETECTION_H
#define ELFDOC_DETECTION_H

#include <elf.h>
#include "ParseELF.h"

#define INDICATIVE_OF_MOD_POST_COMPILATION "Indicative of modification post compilation."
class Detection {
public:
	Detection(std::shared_ptr<ParseELF> parser) {
		_parser = parser;
	}

	void scan() const;

private:
	std::shared_ptr<ParseELF> _parser;
	int embeddedPayload() const;
	int entryPointModification() const;
};


#endif //ELFDOC_DETECTION_H
