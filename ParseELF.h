//
// Created by sad0p on 9/28/23.
//

#ifndef ELFDOC_PARSEELF_H
#define ELFDOC_PARSEELF_H
#include <iostream>
#include <memory>
#include <vector>
#include <sys/stat.h>
#include <sys/mman.h>
#include <elf.h>
#include <fcntl.h>
#include <errno.h>
#include "ExceptionHandler.h"

extern int errno;

class ParseELF {
	std::vector<Elf64_Shdr> _shdrs;
	std::vector<Elf64_Phdr> _phdrs;
	std::shared_ptr<uint8_t[]> _mem;
	std::shared_ptr<Elf64_Ehdr> _hdr;
public:
	const std::string &getPath() const;

private:
	std::string _path;
	int _fd;
	size_t _filesz;

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

		std::shared_ptr<Elf64_Phdr[]> p(_mem, reinterpret_cast<Elf64_Phdr *>(_mem.get() + _hdr->e_phoff));
		for (int i = 0; i < _hdr->e_phnum; i++)
			_phdrs.push_back(p[i]);

		std::shared_ptr<Elf64_Shdr[]> s(_mem, reinterpret_cast<Elf64_Shdr *>(_mem.get() + _hdr->e_shoff));
		for (int i = 0; i < _hdr->e_shnum; i++)
			_shdrs.push_back(s[i]);
	}

	const std::vector<Elf64_Phdr> &getPhdrs() const;
	const std::vector<Elf64_Shdr> &getShdrs() const;
	const std::shared_ptr<Elf64_Ehdr> &getHdr() const;
	size_t getFilesz() const;
};


#endif //ELFDOC_PARSEELF_H
