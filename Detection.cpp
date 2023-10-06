//
// Created by sad0p on 10/4/23.
//

#include "Detection.h"


//TODO: Switch from void. We want to propagate the return values to the terminal.

void Detection::scan() const {

	embeddedPayload();

	auto brokenShdr =_parser->isBrokenShdr();
	if(brokenShdr) {
		std::cout << "Broken section header table or section header offset." << std::endl;
		std::cout << INDICATIVE_OF_MOD_POST_COMPILATION << std::endl;
	}

	auto brokenPhdr= _parser->isBrokenPhdr();
	if(brokenPhdr) {
		std::cout << "Broken program header offset (exceeds size of binary)." << std::endl;
		std::cout << INDICATIVE_OF_MOD_POST_COMPILATION << std::endl;
	}

	if(!(brokenShdr || brokenPhdr)) {
		textSegmentPadding();
	}
}

/*
 * The entry-point for execution should be at the start of the text section.
 */

int Detection::textSegmentPadding() const{
	auto nameSectionMap = _parser->getSectionNameMapping();
	auto textSection = nameSectionMap[".text"];
	uint64_t e_entry = _parser->getHdr()->e_entry;

	auto sectionEnd = textSection.sh_addr + textSection.sh_size;
	auto sectionstart = textSection.sh_addr;

	auto entryInTextSection = (e_entry >= sectionstart) && (e_entry < sectionEnd);
	if(!entryInTextSection) {
		std::cout << "Entry point for " << _parser->getPath() << " is outside .text section" << std::endl;
		std::cout << "Indicative of entry-point modification." << std::endl;
	}

	return entryInTextSection ? 0 : -1;
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