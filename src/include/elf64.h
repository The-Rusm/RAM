#ifndef __RAM_elf64__
#define __RAM_elf64__

#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>

/*ELF 64 main header*/
typedef struct {
  uint8_t e_ident[16]; /* magic bytes, for ELF is \x7F ELF*/
  uint16_t e_type; /* type of file, 1 - Relocatable object, 2 - Executable */
  uint16_t e_mach; /* target arch, 62 - x86_64, 183 - AArch64, 175 - EM_ELBRUS */
  uint32_t e_ver; /* version, always 1*/
  uint64_t e_entry; /* entry point, 0 for object */
  uint64_t e_phoff; /* Program Header Table Offset */
  uint64_t e_shoff; /* Section Header Table Offset */
  uint32_t e_flags; /* Proccessor flags */
  uint16_t e_ehsize; /* size of this header */
  uint16_t e_phentsize; /* Program table entry size */
  uint16_t e_phnum; /* Number of program table entries */
  uint16_t e_shentsize; /* Section Table entry size*/
  uint16_t e_shnum; /* Section count */
  uint16_t e_shstrndx; /* Section index with string names */
} _RAM_elf64Ehead;

typedef struct {
  uint32_t sh_name; /* Section name offset in string table */
  uint32_t sh_type; /* Section type 1 - PROGBITS, 3 - STRTAB */
  uint64_t sh_flags; /* flags 1 - WRITE, 2 - ALLOC, 4 - EXEC */
  uint64_t sh_addr; /* Virtual Memory Address */
  uint64_t sh_offset; /* Section offset */
  uint64_t sh_size; /* Section size in bytes */
  uint32_t sh_link; /* Link on other section */
  uint32_t sh_info; /* Info */
  uint64_t sh_addral; /* Align */
  uint64_t sh_entsize; /* Record size if the section contains a table */
} _RAM_elf64Shead;

typedef struct {
  uint32_t st_name; /* Name of symbol offset in strtab*/
  uint8_t st_info; /* symbol type, func or val, and global or local */
  uint8_t st_other; /* visibility */
  uint16_t st_shndx; /* Index of the section containing this symbol*/
  uint64_t st_value; /* Symbol offset inside his section */
  uint64_t st_size; /* symbol size in bytes */
} _RAM_elf64sym;

typedef struct {
  const char *name; /*name of label*/
  uint64_t offset;  /*Offset in bytes inside .text section*/
  bool is_global;   /*is global?*/ 
} _RAMSymbol;


void elf64_pack_object(
  const uint8_t *raw_code, uint64_t code_size, 
  const _RAMSymbol *symbols, int symcount,
  uint16_t mach, FILE *output);

#endif /*__RAM_elf64__*/
