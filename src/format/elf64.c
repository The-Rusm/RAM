#include "../include/elf64.h"
#include "../include/tce.h"
#include <imm/IntMemoryManager.h>
#include <string.h>

#define ELF64_ST_INFO(bind, type) (((bind) << 4) + ((type) & 0xf))

void elf64_pack_object(
  const uint8_t *raw_code, uint64_t code_size, 
  const _RAMSymbol *symbols, int symcount,
  uint16_t mach, FILE *output)
{
  if (!output) throw("RAM Format NULL Error: output file is NULL!");
   /* Forming string tables for section names
      Strings are separated by a null byte \0
      Indexes:
      0:"", 1:".text", 7:".shstrtab"
   */
  const char *shstrtab = "\0.text\0.symtab\0.strtab\0.shstrtab\0";
  uint64_t shstrtabsz = 33;

  char *strtab = MemoryAllocate(4096);
  if (!strtab) throw("RAM Format Alloc Error: Failed to allocate memory for strtab!");
  strtab[0] = '\0';
  uint64_t strtab_size = 1;

  int total_elf_symbols = symcount + 1;
  _RAM_elf64sym *elf_syms = MemoryAllocateAndFillZero(total_elf_symbols, sizeof(_RAM_elf64sym));
  if (!elf_syms) throw("RAM Format Alloc Error: Failed to allocate memory for symtab");

  for (int i = 0; i < symcount; i++)
  {
    _RAM_elf64sym *sym = &elf_syms[i + 1];

    sym->st_name = strtab_size;
    strcpy(&strtab[strtab_size], symbols[i].name);
    strtab_size += strlen(symbols[i].name) + 1;

    uint8_t bind = symbols[i].is_global ? 1 : 0; /* 1 - STB_GLOBAL, 0 - STB_LOCAL*/
    sym->st_info = ELF64_ST_INFO(bind, 2); /* 2 - STT_FUNC */
    sym->st_shndx = 1;
    sym->st_value = symbols[i].offset;
    sym->st_size = 0;
  }

  _RAM_elf64Ehead header;
  memset(&header, 0, sizeof(_RAM_elf64Ehead));

  /* ELF magic */
  header.e_ident[0] = 0x7F;
  header.e_ident[1] = 'E';
  header.e_ident[2] = 'L';
  header.e_ident[3] = 'F';
  header.e_ident[4] = 2; /*64-bit*/
  header.e_ident[5] = 1; /*LE*/
  header.e_ident[6] = 1; /*Version*/
  
  header.e_type = 1; /*ET_REL, object file by default, i don't think that RAM does need to generate an EXECUTABLE, for it exist a Rusm*/
  header.e_mach = mach;
  header.e_ver = 1;
  header.e_ehsize = sizeof(_RAM_elf64Ehead);
  header.e_shentsize = sizeof(_RAM_elf64Shead);
  header.e_shnum = 3; /* 3 Sections: NULL, .text, .shstrtab */
  header.e_shstrndx = 2; /* Section .shstrtab has index 2 */
  
  /* Offsets */
  uint64_t offset = sizeof(_RAM_elf64Ehead);
  uint64_t text_offset = offset;
  offset += code_size;
  uint64_t symtab_offset = offset;
  offset += total_elf_symbols * sizeof(_RAM_elf64sym);
  uint64_t strtab_offset = offset;
  offset += strtab_size;
  uint64_t shstrtab_offset = offset;
  offset += shstrtabsz;

  header.e_shoff = offset;

  /* writing to the file */
  fwrite(&header, sizeof(_RAM_elf64Ehead), 1, output); /*ELF header*/
  fwrite(raw_code, 1, code_size, output); /*.text section*/
  fwrite(elf_syms, sizeof(_RAM_elf64sym), total_elf_symbols, output); /* .symtab */
  fwrite(strtab, 1, strtab_size, output); /* .strtab */
  fwrite(shstrtab, 1, shstrtabsz, output); /*string table of section names*/

  _RAM_elf64Shead sections[5];
  memset(sections, 0, sizeof(sections));

  /* Section 0: always null*/
  /* Section 1: .text*/
  sections[1].sh_name = 1; /* offset ".text" in shstrtab*/
  sections[1].sh_type = 1; // SHT_PROGBITS
  sections[1].sh_flags = 6; /* SHF_ALLOC - 2 | SHF_EXECINSTR - 4*/
  sections[1].sh_offset = text_offset;
  sections[1].sh_size = code_size;
  sections[1].sh_addral = 16; /* align by 16 bytes */
  /* second section: .symtab */
  sections[2].sh_name = 7; 
  sections[2].sh_type = 2; // SHT_SYMTAB
  sections[2].sh_offset = symtab_offset;
  sections[2].sh_size = total_elf_symbols * sizeof(_RAM_elf64sym);
  sections[2].sh_link = 3;
  sections[2].sh_info = 1;
  sections[2].sh_addral = 8;
  sections[2].sh_entsize = sizeof(_RAM_elf64sym);
  /* third section: .strtab */
  sections[3].sh_name = 15;
  sections[3].sh_type = 3; // SHT_STRTAB
  sections[3].sh_offset = strtab_offset;
  sections[3].sh_size = strtab_size;
  sections[3].sh_addral = 1;
  /* fourth section: .shstrtab */
  sections[4].sh_name = 7; /* offset ".shstrtab" in shstrtab*/
  sections[4].sh_type = 3; // SHT_STRTAB
  sections[4].sh_offset = shstrtab_offset;
  sections[4].sh_size = shstrtabsz;
  sections[4].sh_addral = 1;

  if (fwrite(sections, sizeof(_RAM_elf64Shead), 3, output) != 3)
    throw("RAM Format Error: Failed to write ELF64 section table!");

  cleanbit(strtab);
  cleanbit(elf_syms);
}
