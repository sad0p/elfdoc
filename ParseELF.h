//
// Created by sad0p on 9/28/23.
//

#ifndef ELFDOC_PARSEELF_H
#define ELFDOC_PARSEELF_H
#include <iostream>
#include <memory>
#include <vector>
#include <map>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <errno.h>
#include "ExceptionHandler.h"

extern int errno;

class ParseELF {
	void mapNameToSection();
	std::map<std::string, Elf64_Shdr> _nameSection;
	std::vector<Elf64_Shdr> _shdrs;
	std::vector<Elf64_Phdr> _phdrs;
	std::shared_ptr<uint8_t[]> _mem;
	std::shared_ptr<Elf64_Ehdr> _hdr;
	std::string _path;
	int _fd;
	size_t _filesz;
	uint64_t _memStart;
	uint64_t _memEnd;
	bool _brokenPhdr;
	bool _brokenShdr;

public:
	ParseELF(std::string& path) {
		_path = path;
		auto statBuf = std::make_unique<struct stat>();
		if ((_fd = open(path.c_str(), O_RDONLY)) < 0)
			throw ExceptionHandler(errno, std::string("while opening opening " + _path));

		if (fstat(_fd, static_cast<struct stat *>(statBuf.get())) < 0)
			throw ExceptionHandler(errno, "while performing fstat on" + _path);

		_filesz = statBuf->st_size;
		_mem = std::make_shared<uint8_t[]>(_filesz);

		if (read(_fd, _mem.get(), _filesz) != _filesz)
			throw ExceptionHandler(errno, "while reading " + _path);

		if (_mem[0] != 0x7f && _mem[1] != 0x45 && _mem[2] != 0x4c && _mem[3] != 0x46)
			throw ExceptionHandler( "Not an ELF binary (" + _path + ")");

		_hdr = std::reinterpret_pointer_cast<Elf64_Ehdr>(_mem);
		_memStart = reinterpret_cast<uint64_t>(_mem.get());
		_memEnd = reinterpret_cast<uint64_t>(_mem.get()) + _filesz;

		auto offPhdrTableEnd = _hdr->e_phoff + (_hdr->e_phentsize * _hdr->e_phnum);
		_brokenPhdr = _memEnd < (_memStart + offPhdrTableEnd);
		if(!_brokenPhdr) {
			std::shared_ptr<Elf64_Phdr[]> p(_mem, reinterpret_cast<Elf64_Phdr *>(_mem.get() + _hdr->e_phoff));
			for (int i = 0; i < _hdr->e_phnum; i++)
				_phdrs.push_back(p[i]);
		}

		auto offShdrTableEnd = _hdr->e_shoff + (_hdr->e_shentsize * _hdr->e_shnum);
		_brokenShdr = _memEnd < (_memStart + offShdrTableEnd) ;
		if(!_brokenShdr) {
			std::shared_ptr<Elf64_Shdr[]> s(_mem, reinterpret_cast<Elf64_Shdr *>(_mem.get() + _hdr->e_shoff));
			for (int i = 0; i < _hdr->e_shnum; i++)
				_shdrs.push_back(s[i]);
			mapNameToSection();
		}
	}

	const std::vector<Elf64_Phdr> &getPhdrs() const;
	const std::vector<Elf64_Shdr> &getShdrs() const;
	const std::shared_ptr<Elf64_Ehdr> &getHdr() const;
	const std::string &getPath() const;
	size_t getFilesz() const;
	std::map<std::string, Elf64_Shdr> getSectionNameMapping() const;
	uint64_t getMemStart() const;
	bool isBrokenPhdr() const;
	bool isBrokenShdr() const;
};


#endif //ELFDOC_PARSEELF_H
