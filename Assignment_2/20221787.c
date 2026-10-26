#include "20221787.h"

// Check that the range [offset, offset + size) is inside the file
// (written this way to avoid integer overflow on broken files)
static int in_file(Elf64_Off offset, Elf64_Xword size, off_t length)
{
	return offset <= (Elf64_Off)length && size <= (Elf64_Off)length - offset;
}

// Read the whole file into a newly allocated buffer
char* read_file(int fd, off_t* length)
{
	char* buffer;
	off_t total = 0;
	ssize_t n;

	// Get the file size and go back to the beginning
	*length = lseek(fd, 0, SEEK_END);
	if (*length == -1 || lseek(fd, 0, SEEK_SET) == -1)
	{
		perror("lseek");
		return NULL;
	}

	buffer = (char*)malloc(*length > 0 ? *length : 1);
	if (buffer == NULL)
	{
		perror("malloc");
		return NULL;
	}

	// read() may return less than requested, so loop until all bytes are read
	while (total < *length)
	{
		n = read(fd, buffer + total, *length - total);
		if (n <= 0)
		{
			if (n == -1)
			{
				perror("read");
			}
			free(buffer);
			return NULL;
		}
		total += n;
	}

	return buffer;
}

// Check whether the buffer really contains an ELF file
// Returns 1 if it is a 64-bit ELF file, otherwise 0
int is_elf(const char* buffer, off_t length)
{
	const Elf64_Ehdr* hdr = (const Elf64_Ehdr*)buffer;

	// The file must be large enough to hold an ELF header
	if (length < (off_t)sizeof(Elf64_Ehdr))
	{
		fprintf(stderr, "Not an ELF file: file is too small\n");
		return 0;
	}

	// The first 4 bytes must be the magic number 0x7F 'E' 'L' 'F'
	if (memcmp(hdr->e_ident, ELFMAG, SELFMAG) != 0)
	{
		fprintf(stderr, "Not an ELF file: wrong magic number "
			"(%02X %02X %02X %02X)\n",
			(unsigned char)buffer[0], (unsigned char)buffer[1],
			(unsigned char)buffer[2], (unsigned char)buffer[3]);
		return 0;
	}

	// This program parses the file with 64-bit ELF structures
	if (hdr->e_ident[EI_CLASS] != ELFCLASS64)
	{
		fprintf(stderr, "Not supported: only ELF64 files are supported\n");
		return 0;
	}

	return 1;
}

// Print basic information of the ELF file (same as editro)
void print_elf_info(const char* buffer)
{
	const Elf64_Ehdr* hdr = (const Elf64_Ehdr*)buffer;

	// Magic
	printf("ELF Magic     : %02X %02X %02X %02X\n",
		hdr->e_ident[0], hdr->e_ident[1], hdr->e_ident[2], hdr->e_ident[3]);

	// Class (is_elf() already guarantees ELF64)
	printf("Class         : ELF64\n");

	// Data
	printf("Data          : ");
	switch (hdr->e_ident[EI_DATA])
	{
		case ELFDATANONE:
			printf("None\n");
			break;

		case ELFDATA2LSB:
			printf("2's complement, little endian\n");
			break;

		case ELFDATA2MSB:
			printf("2's complement, big endian\n");
			break;

		default:
			printf("Unknown\n");
			break;
	}

	// Machine
	if (hdr->e_machine == EM_X86_64)
	{
		printf("Machine       : AMD X86-64\n");
	}
	else
	{
		printf("Machine       : Other Machines\n");
	}

	printf("Entry point   : 0x%lX\n", (unsigned long)hdr->e_entry);
	printf("Section count : %d\n", hdr->e_shnum);
	printf("\n");
}

// Find the section header of the given section name
// Returns NULL if the section does not exist or the headers are broken
Elf64_Shdr* find_section(char* buffer, off_t length, const char* name)
{
	Elf64_Ehdr* hdr = (Elf64_Ehdr*)buffer;
	Elf64_Shdr* sht;
	Elf64_Shdr* strsec;
	char* shstrtab;
	int i;

	// The section header table must be inside the file
	if (hdr->e_shoff == 0 || hdr->e_shnum == 0 ||
		!in_file(hdr->e_shoff, (Elf64_Xword)hdr->e_shnum * sizeof(Elf64_Shdr), length))
	{
		fprintf(stderr, "Invalid section header table\n");
		return NULL;
	}
	sht = (Elf64_Shdr*)&buffer[hdr->e_shoff];

	// The section header string table must be inside the file
	if (hdr->e_shstrndx >= hdr->e_shnum)
	{
		fprintf(stderr, "Invalid section header string table index\n");
		return NULL;
	}
	strsec = &sht[hdr->e_shstrndx];
	if (!in_file(strsec->sh_offset, strsec->sh_size, length))
	{
		fprintf(stderr, "Invalid section header string table\n");
		return NULL;
	}
	shstrtab = &buffer[strsec->sh_offset];

	// Compare the name of each section with the given name
	for (i = 0; i < hdr->e_shnum; i++)
	{
		if (sht[i].sh_name >= strsec->sh_size)
		{
			continue;
		}
		if (strcmp(&shstrtab[sht[i].sh_name], name) == 0)
		{
			return &sht[i];
		}
	}

	return NULL;
}

// Replace TARGET_STR with REPLACE_STR in .rodata and write it to the file
// Returns the number of replaced strings, or -1 on error
int patch_rodata(int fd, char* buffer, Elf64_Shdr* rodata)
{
	char* section = &buffer[rodata->sh_offset];
	size_t len = strlen(TARGET_STR);
	size_t i;
	int count = 0;

	if (rodata->sh_size < len)
	{
		return 0;
	}

	// Scan the whole .rodata section byte by byte
	for (i = 0; i + len <= rodata->sh_size; i++)
	{
		if (memcmp(&section[i], TARGET_STR, len) != 0)
		{
			continue;
		}

		// Modify the string in the buffer
		memcpy(&section[i], REPLACE_STR, len);

		// Write the modified bytes to the same position of the file
		if (pwrite(fd, &section[i], len, rodata->sh_offset + i) != (ssize_t)len)
		{
			perror("pwrite");
			return -1;
		}

		printf("   \"%s\" -> \"%s\" at file offset 0x%08lX\n",
			TARGET_STR, REPLACE_STR, (unsigned long)(rodata->sh_offset + i));
		count++;
		i += len - 1;
	}

	return count;
}

// Main
int main(int argc, char* argv[])
{
	int fd;
	off_t length;
	char* buffer;
	Elf64_Shdr* rodata;
	int count;
	int ret = -1;

	if (argc < 2)
	{
		printf("ex) %s <elf_file_name>\n", argv[0]);
		return -1;
	}

	// Open the file with read/write permission to modify it
	fd = open(argv[1], O_RDWR);
	if (fd == -1)
	{
		perror("open");
		return -1;
	}

	// Read the whole file into memory
	buffer = read_file(fd, &length);
	if (buffer == NULL)
	{
		close(fd);
		return -1;
	}

	// 1. Check whether the file is really an ELF file
	if (!is_elf(buffer, length))
	{
		goto out;
	}

	print_elf_info(buffer);

	// 2. Find the .rodata section
	rodata = find_section(buffer, length, ".rodata");
	if (rodata == NULL)
	{
		fprintf(stderr, ".rodata section not found\n");
		goto out;
	}
	if (!in_file(rodata->sh_offset, rodata->sh_size, length))
	{
		fprintf(stderr, "Invalid .rodata section\n");
		goto out;
	}

	printf(".rodata section info\n");
	printf("   File Offset = 0x%08lX\n", (unsigned long)rodata->sh_offset);
	printf("          Size = 0x%08lX\n", (unsigned long)rodata->sh_size);
	printf("\n");

	// 3. Replace "software" with "hackers!" in .rodata
	count = patch_rodata(fd, buffer, rodata);
	if (count < 0)
	{
		goto out;
	}

	if (count == 0)
	{
		printf("\"%s\" not found in .rodata\n", TARGET_STR);
	}
	else
	{
		printf("%d string(s) modified\n", count);
	}
	ret = 0;

out:
	// Close the file and free the buffer
	free(buffer);
	close(fd);

	return ret;
}
