//
// Created by sad0p on 9/28/23.
//

#include "ParseELF.h"

const std::vector<Elf64_Phdr> &ParseELF::getPhdrs() const {
	return _phdrs;
}

const std::vector<Elf64_Shdr> &ParseELF::getShdrs() const {
	return _shdrs;
}

const std::shared_ptr<Elf64_Ehdr> &ParseELF::getHdr() const {
	return _hdr;
}

size_t ParseELF::getFilesz() const {
	return _filesz;
}

const std::string &ParseELF::getPath() const {
	return _path;
}

void ParseELF::mapNameToSection() {
	auto strTable = _shdrs.at(_hdr->e_shstrndx);
	std::shared_ptr<uint8_t[]> strTableMem(_mem, reinterpret_cast<uint8_t*>(_mem.get() + strTable.sh_offset));
	for(auto section : _shdrs)
		_nameSection[reinterpret_cast<const char*>(&strTableMem[section.sh_name])] = section;
}