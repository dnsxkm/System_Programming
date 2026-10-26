#ifndef _20221787_H_
#define _20221787_H_

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <elf.h>
#include <errno.h>
#include <string.h>

// String to find in .rodata and the string to replace it with
// (both are 8 bytes, so the section layout does not change)
#define TARGET_STR  "software"
#define REPLACE_STR "hackers!"

// Read the whole file into a newly allocated buffer
char* read_file(int fd, off_t* length);

// Check whether the buffer really contains an ELF file
int is_elf(const char* buffer, off_t length);

// Print basic information of the ELF file (same as editro)
void print_elf_info(const char* buffer);

// Find the section header of the given section name
Elf64_Shdr* find_section(char* buffer, off_t length, const char* name);

// Replace TARGET_STR with REPLACE_STR in .rodata and write it to the file
int patch_rodata(int fd, char* buffer, Elf64_Shdr* rodata);

#endif
