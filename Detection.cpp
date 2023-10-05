//
// Created by sad0p on 10/4/23.
//

#include "Detection.h"

void Detection::scan() const {
	embeddedPayload();
}
/*
 * The entry-point for execution should be at the start of the text section.
 */

int Detection::textSegmentPadding() const{
	return 0;
}

/*
 * Calculate the size the elf binary should be based on ELF metadata with the following:
 * elf_file_size = elf_header.shoff + (elf_header.shentsize * elf_header.shnum)
 */

int Detection::embeddedPayload() const {
	auto hdr = _parser->getHdr();
	auto filesz = _parser->getFilesz();
	auto calcFilesz = hdr->e_shoff + (hdr->e_shentsize * hdr->e_shnum);
	if(calcFilesz < filesz) {
		std::cout << "Possible embedded payload alert (" << _parser->getPath() << ")" << std::endl;
		std::cout << "filesize (disk) = " << filesz << std::endl;
		std::cout << "filesize (calculated from ELF metadata) = " << calcFilesz << std::endl;
		return -1;
	}
	return 0;
}