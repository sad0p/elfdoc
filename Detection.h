//
// Created by sad0p on 10/4/23.
//

#ifndef ELFDOC_DETECTION_H
#define ELFDOC_DETECTION_H

#include <elf.h>
#include "ParseELF.h"

class Detection {
public:
	Detection(const ParseELF &parseElf) {
		_filesz = parseElf.getFilesz();
		_hdr = parseElf.getHdr();
		_elfPath = parseElf.getPath();

	}

	void scan() const;
private:
	size_t _filesz;
	std::string _elfPath;
	std::shared_ptr<Elf64_Ehdr> _hdr;
	int embeddedPayload() const;
};


#endif //ELFDOC_DETECTION_H
