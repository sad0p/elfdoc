//
// Created by sad0p on 10/4/23.
//

#include "Detection.h"

void Detection::scan() const {
	embeddedPayload();
}

/*
 * Calculate the size the elf binary should be based on ELF metadata with the following:
 * elf_file_size = elf_header.shoff + (elf_header.shentsize * elf_header.shnum)
 */

int Detection::embeddedPayload() const {
	auto calcFilesz = _hdr->e_shoff + (_hdr->e_shentsize * _hdr->e_shnum);
	if(calcFilesz < _filesz) {
		std::cout << "Possible embedded payload alert (" << _elfPath << ")" << std::endl;
		std::cout << "filesize (disk) = " << _filesz << std::endl;
		std::cout << "filesize (calculated from ELF metadata) = " << calcFilesz << std::endl;
		return -1;
	}
	return 0;
}