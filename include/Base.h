/* 
 * This file is part of the pebil project.
 * 
 * Copyright (c) 2010, University of California Regents
 * All rights reserved.
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef _Base_h_
#define _Base_h_

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <assert.h>
#include <strings.h>
#include <string.h>
#include <unistd.h>
#include <regex.h>
#include <errno.h>
#include <sys/stat.h>
#include <execinfo.h>
#include <dlfcn.h>
#include <CStructuresElf.h>
#include <CStructuresDwarf.h>
#include <Debug.h>
#include <Vector.h>

#ifdef HAVE_UNORDERED_MAP
#define pebil_map_type tr1::unordered_map
#include <tr1/unordered_map>
#else
#define pebil_map_type map
#include <map>
#endif

typedef void (*fprintf_ftype)(FILE*, const char*, ...);
extern FILE* pebilOutp;
extern uint64_t warnCount;

#define __MAX_STRING_SIZE 1024
#define __SHOULD_NOT_ARRIVE ASSERT(0 && "Control should not reach this point")
#define __FUNCTION_NOT_IMPLEMENTED PRINT_ERROR("Function %s in file %s not implemented", __FUNCTION__, __FILE__); __SHOULD_NOT_ARRIVE;

#define GET_FIELD_BASIS(__type,__field) virtual __type get_ ## __field() \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define GET_FIELD_CLASS(__type,__field) inline __type get_ ## __field() \
    { return (__type)(entry.__field); }
#define GET_FIELD_BASIS_A(__type,__field,__union) virtual __type get_ ## __union ## _ ## __field() \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define GET_FIELD_CLASS_A(__type,__field,__union) inline __type get_ ## __union ## _ ## __field() \
    { return (__type)(entry.__union.__field); }
#define GET(__field) get_ ## __field()
#define GET_A(__field,__union) get_ ## __union ## _ ## __field()


#define SET_FIELD_BASIS(__type,__field) virtual __type set_ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define SET_FIELD_CLASS(__type,__field) inline __type set_ ## __field(__type __value) \
    { entry.__field = __value; return (__type)(entry.__field); }
#define SET_FIELD_BASIS_A(__type,__field,__union) virtual __type set_ ## __union ## _ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define SET_FIELD_CLASS_A(__type,__field,__union) inline __type set_ ## __union ## _ ## __field(__type __value) \
    { entry.__union.__field = __value; return (__type)(entry.__union.__field); }
#define SET(__field,__value) set_ ## __field(__value)
#define SET_A(__field,__union,__value) set_ ## __union ## _ ## __field(__value)


#define INCREMENT_FIELD_BASIS(__type,__field) virtual __type increment_ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define INCREMENT_FIELD_CLASS(__type,__field) inline __type increment_ ## __field(__type __value) \
    { entry.__field += __value; return (__type)(entry.__field); }
#define INCREMENT_FIELD_BASIS_A(__type,__field,__union) virtual __type increment_ ## __union ## _ ## __field(__type __value) \
    { __SHOULD_NOT_ARRIVE; return ( __type )0; }
#define INCREMENT_FIELD_CLASS_A(__type,__field,__union) inline __type increment_ ## __union ## _ ## __field(__type __value) \
    { entry.__union.__field += __value; return (__type)(entry.__union.__field); }
#define INCREMENT(__field,__value) increment_ ## __field(__value)
#define INCREMENT_A(__field,__union,__value) increment_ ## __union ## _ ## __field(__value)


#define PRINT_ERROR(...) fprintf(stderr,"*********** ERROR : "); \
    fprintf(stderr,## __VA_ARGS__);                              \
    fprintf(stderr,"\n");                                \
    ASSERT(0); \
    exit(-1);

#define PRINT_INFOR(...) fprintf(pebilOutp,"[pebil-instr] "); \
    fprintf(pebilOutp,## __VA_ARGS__);                        \
    fprintf(pebilOutp,"\n");                                  \
    fflush(pebilOutp);

#define PRINT_INFO() fprintf(pebilOutp,"[pebil-instr] "); \
    fflush(pebilOutp);

#define PRINT_OUT(...) fprintf(pebilOutp,## __VA_ARGS__); \
    fflush(pebilOutp);

#ifdef WARNING_SEVERITY
#define WARN_FILE stdout
#define PRINT_WARN(__severity,...)  if (__severity >= WARNING_SEVERITY){ \
    fprintf(WARN_FILE,"*** WARNING : ");                            \
    fprintf(WARN_FILE,## __VA_ARGS__);                              \
    fprintf(WARN_FILE,"\n");                                        \
    fflush(WARN_FILE);                                              \
    warnCount++; }
#else
#define PRINT_WARN(...)
#endif

#define PRINT_PROGRESS(__inc, __tot, __break) \
    if (__inc % ((__tot > __break) ? (__tot / __break) : 1) == 0){ fprintf(pebilOutp, "."); fflush(pebilOutp); }


#define __bit_shift(__v) (1 << __v)

#define Invalid_UInteger_ID                 ((uint32_t)-1)
#define Size__32_bit_File_Header            sizeof(Elf32_Ehdr)
#define Size__64_bit_File_Header            sizeof(Elf64_Ehdr)
#define Size__32_bit_Program_Header         sizeof(Elf32_Phdr)
#define Size__64_bit_Program_Header         sizeof(Elf64_Phdr)
#define Size__32_bit_Section_Header         sizeof(Elf32_Shdr)
#define Size__64_bit_Section_Header         sizeof(Elf64_Shdr)
#define Size__32_bit_Symbol                 sizeof(Elf32_Sym)
#define Size__64_bit_Symbol                 sizeof(Elf64_Sym)
#define Size__32_bit_Relocation             sizeof(Elf32_Rel)
#define Size__64_bit_Relocation             sizeof(Elf64_Rel)
#define Size__32_bit_Relocation_Addend      sizeof(Elf32_Rela)
#define Size__64_bit_Relocation_Addend      sizeof(Elf64_Rela)
#define Size__32_bit_Global_Offset_Table_Entry sizeof(uint32_t)
#define Size__64_bit_Global_Offset_Table_Entry sizeof(uint64_t)
#define Size__32_bit_Dynamic_Entry          sizeof(Elf32_Dyn)
#define Size__64_bit_Dynamic_Entry          sizeof(Elf64_Dyn)
#define Size__32_bit_Hash_Entry             sizeof(uint32_t)
#define Size__64_bit_Hash_Entry             sizeof(uint64_t)
#define Size__32_bit_GNU_Hash_Bloom_Entry   sizeof(uint32_t)
#define Size__64_bit_GNU_Hash_Bloom_Entry   sizeof(uint64_t)
#define Size__32_bit_Note_Section_Entry     sizeof(uint32_t)
#define Size__64_bit_Note_Section_Entry     sizeof(uint32_t)
#define Size__32_bit_Gnu_Verneed            sizeof(Elf32_Verneed)
#define Size__64_bit_Gnu_Verneed            sizeof(Elf64_Verneed)
#define Size__32_bit_Gnu_Vernaux            sizeof(Elf32_Vernaux)
#define Size__64_bit_Gnu_Vernaux            sizeof(Elf64_Vernaux)
#define Size__32_bit_Gnu_Versym             sizeof(uint16_t)
#define Size__64_bit_Gnu_Versym             sizeof(uint16_t)
#define Size__Dwarf_LineInfo_Header         sizeof(DWARF4_Internal_LineInfo)
#define Size__Dwarf_LineInfo                sizeof(DWARF4_LineInfo_Registers)


#define Print_Code_All                      0x00000001
#define Print_Code_FileHeader               0x00000002
#define Print_Code_SectionHeader            0x00000004
#define Print_Code_ProgramHeader            0x00000008
#define Print_Code_StringTable              0x00000010
#define Print_Code_SymbolTable              0x00000020
#define Print_Code_NoteSection              0x00000040
#define Print_Code_RelocationTable          0x00000080
#define Print_Code_GlobalOffsetTable        0x00000100
#define Print_Code_HashTable                0x00000200
#define Print_Code_DynamicTable             0x00000400
#define Print_Code_GnuVersymTable           0x00000800
#define Print_Code_GnuVerneedTable          0x00001000
#define Print_Code_Disassemble              0x00002000
#define Print_Code_Instruction              0x00004000
#define Print_Code_Instrumentation          0x00008000
#define Print_Code_DwarfSection             0x00010000
#define Print_Code_Loops                    0x00020000

#define HAS_PRINT_CODE(__value,__Print_Code) ((__value & __Print_Code) || (__value & Print_Code_All))
#define SET_PRINT_CODE(__value,__Print_Code) (__value |= __Print_Code)

typedef enum {
    FlagsProtectionMethod_undefined = 0,
    FlagsProtectionMethod_none,
    FlagsProtectionMethod_light,
    FlagsProtectionMethod_full,
    FlagsProtectionMethod_Total_Types
} FlagsProtectionMethods;

static const char* FlagsProtectionMethodNames[FlagsProtectionMethod_Total_Types] = {
    "undefined",
    "none",
    "light",
    "full"
};

typedef enum {
    InstrumentationMode_undefined = 0,
    InstrumentationMode_inline,
    InstrumentationMode_trampinline,
    InstrumentationMode_tramp,
    InstrumentationMode_Total_Types
} InstrumentationModes;

static const char* InstrumentationModeNames[InstrumentationMode_Total_Types] = {
    "undefined",
    "inline",
    "trampinline",
    "tramp"
};

typedef enum {
    TableMode_undefined = 0,
    TableMode_direct,
    TableMode_indirect,
    TableMode_instructions,
    TableMode_Total_Types
} TableModes;

typedef enum {
    InstLocation_undefined = 0,
    InstLocation_prior,
    InstLocation_after,
    InstLocation_replace,
    InstLocation_Total_Types
} InstLocations;

static const char* InstLocationNames[InstLocation_Total_Types] = {
    "undefined",
    "prior",
    "after",
    "replace"
};

typedef enum {
    DebugFormat_undefined = 0,
    DebugFormat_DWARF4_32bit,
    DebugFormat_DWARF4_64bit,
    DebugFormat_Total_Types
} DebugFormats;

typedef enum {
    ElfRelType_undefined = 0,
    ElfRelType_rel,
    ElfRelType_rela,
    ElfRelType_Total_Types
} ElfRelTypes;

typedef enum {
    PebilClassType_no_type = 0, // 0
    PebilClassType_BasicBlock,
    PebilClassType_CodeBlock,
    PebilClassType_DataReference,
    PebilClassType_DataSection,
    PebilClassType_DwarfSection,
    PebilClassType_DwarfLineInfoSection,
    PebilClassType_Dynamic,
    PebilClassType_DynamicTable,
    PebilClassType_FileHeader,
    PebilClassType_FreeText, // 10
    PebilClassType_Function,
    PebilClassType_GlobalOffsetTable,
    PebilClassType_GnuHashTable,
    PebilClassType_GnuVerneed,
    PebilClassType_GnuVerneedTable,
    PebilClassType_GnuVersym,
    PebilClassType_GnuVersymTable,
    PebilClassType_X86Instruction,
    PebilClassType_InstrumentationFunction,
    PebilClassType_InstrumentationPoint, // 20
    PebilClassType_InstrumentationSnippet,
    PebilClassType_Note,
    PebilClassType_NoteSection,
    PebilClassType_ProgramHeader,
    PebilClassType_RawBlock,
    PebilClassType_RawSection,
    PebilClassType_RelocationTable,
    PebilClassType_Relocation,
    PebilClassType_SectionHeader,
    PebilClassType_StringTable, // 30
    PebilClassType_Symbol,
    PebilClassType_SymbolTable,
    PebilClassType_SysvHashTable,
    PebilClassType_TextSection, 
    PebilClassType_Total_Types
} PebilClassTypes;

static const char* PebilClassTypeNames[PebilClassType_Total_Types] = {
    "no_type", // 0
    "BasicBlock",
    "CodeBlock",
    "DataReference",
    "DataSection",
    "DwarfSection",
    "DwarfLineInfoSection",
    "Dynamic",
    "DynamicTable",
    "FileHeader",
    "FreeText", // 10
    "Function",
    "GlobalOffsetTable",
    "GnuHashTable",
    "GnuVerneed",
    "GnuVerneedTable",
    "GnuVersym",
    "GnuVersymTable",
    "X86Instruction",
    "InstrumentationFunction",
    "InstrumentationPoint", // 20
    "InstrumentationSnippet",
    "Note",
    "NoteSection",
    "ProgramHeader",
    "RawBlock",
    "RawSection",
    "RelocationTable",
    "Relocation",
    "SectionHeader",
    "StringTable", // 30
    "Symbol",
    "SymbolTable",
    "SysvHashTable",
    "TextSection"
};

#define GET_PEBIL_CLASS_NAME(__typ) (PebilClassTypeNames[__typ])

typedef enum {
    ByteSource_no_source = 0,
    ByteSource_Application,
    ByteSource_Application_FreeText,
    ByteSource_Application_Function,
    ByteSource_Instrumentation,
    ByteSource_Total_Types
} ByteSources;

#define IS_BYTE_SOURCE_APPLICATION(__src) \
    ((__src == ByteSource_Application) || \
     (__src == ByteSource_Application_FreeText) || \
     (__src == ByteSource_Application_Function))

class BinaryInputFile;
class BinaryOutputFile;
class X86Instruction;
template <class anonymous> class Vector;

class Base {
protected:
    const static uint32_t invalidOffset = 0xffffffff;

    PebilClassTypes type;
    uint32_t sizeInBytes;
    uint32_t fileOffset;

    Base() : type(PebilClassType_no_type),sizeInBytes(0),fileOffset(invalidOffset),baseAddress(0) {}
    Base(PebilClassTypes t) : type(t),sizeInBytes(0),fileOffset(invalidOffset),baseAddress(0) {}
    virtual ~Base() {}

public:
    uint64_t baseAddress;

    PebilClassTypes getType() { return type; }
    const char* getTypeName() { return GET_PEBIL_CLASS_NAME(type); }
    uint32_t getSizeInBytes() { return sizeInBytes; }

    virtual void print() { __SHOULD_NOT_ARRIVE; }
    virtual uint32_t read(BinaryInputFile* b) { __SHOULD_NOT_ARRIVE; return 0; }


    uint32_t getFileOffset() { return fileOffset; }
    void setFileOffset(uint32_t offset) { fileOffset = offset; }
    bool hasInvalidFileOffset() { return (invalidOffset == fileOffset); }

    bool includesFileOffset(uint32_t offset);


    bool containsProgramBits() { return (type == PebilClassType_X86Instruction          || 
                                         type == PebilClassType_BasicBlock              || 
                                         type == PebilClassType_Function                || 
                                         type == PebilClassType_TextSection             ||
                                         type == PebilClassType_InstrumentationSnippet  ||
                                         type == PebilClassType_InstrumentationFunction ||
                                         type == PebilClassType_DataReference
                                         ); }
    virtual Vector<X86Instruction*>* swapInstructions(uint64_t addr, Vector<X86Instruction*>* replacements) { __SHOULD_NOT_ARRIVE; return NULL; }
    virtual uint64_t findInstrumentationPoint(uint32_t size, InstLocations loc) { __SHOULD_NOT_ARRIVE; return 0; }
    virtual uint64_t getBaseAddress() { __SHOULD_NOT_ARRIVE; }
};

class FileList {
private:
    uint32_t width;
    char sep;
    const char* fname;

protected:
    Vector<Vector<char*>*> fileTokens;
    void init(const char* filename, uint32_t width, char sep, char comm);

public:
    FileList(const char* filename, uint32_t width, char sep);
    FileList(const char* filename);
    FileList() { width = 0; }
    ~FileList();

    virtual bool matches(char* str, uint32_t tok);
    char* getToken(uint32_t idx, uint32_t tok);
    void print();

    void appendLine(Vector<char*>*);
    void setSeparator(char);
    void setFileName(const char*);

    bool verify();
};

class HashCode {
private:
    typedef union {
        struct {
            uint32_t instruction : 16;
            uint32_t block       : 16;
            uint32_t function    : 16;
            uint32_t section     : 8;
            uint32_t res         : 8;
        } fields;
        uint64_t bits;
    } HashCodeEntry;

    const static uint64_t INVALID_FIELD = 0;

    HashCodeEntry entry;

    inline bool hasSection()       { return (entry.fields.section     != INVALID_FIELD); }
    inline bool hasFunction()      { return (entry.fields.function    != INVALID_FIELD); }
    inline bool hasBlock()         { return (entry.fields.block       != INVALID_FIELD); }
    inline bool hasInstruction()   { return (entry.fields.instruction != INVALID_FIELD); }

    inline static bool validSection(uint32_t s)        { return ((0 <= s) && (s < (0x1 << 8))); }
    inline static bool validFunction(uint32_t f)       { return ((0 <= f) && (f < ((0x1 << 16) - 1))); }
    inline static bool validBlock(uint32_t b)          { return ((0 <= b) && (b < ((0x1 << 16) - 1))); }
    inline static bool validInstruction(uint32_t i)    { return ((0 <= i) && (i < ((0x1 << 16) - 1))); }
public:

    inline uint64_t getValue(){ return entry.bits; }
    inline uint64_t getBlockCode() { return entry.bits & 0xffffffffffffff00; }

    inline HashCode() { entry.bits = INVALID_FIELD; }
    inline HashCode(uint64_t a) { entry.bits = a; }

    HashCode(uint32_t s);
    HashCode(uint32_t s,uint32_t f);
    HashCode(uint32_t s,uint32_t f,uint32_t b);
    HashCode(uint32_t s,uint32_t f,uint32_t b,uint32_t i);

    inline bool isSection()     { return (hasSection() && !hasFunction() && !hasBlock() && !hasInstruction()); }
    inline bool isFunction()    { return (hasSection() &&  hasFunction() && !hasBlock() && !hasInstruction()); }
    inline bool isBlock()       { return (hasSection() &&  hasFunction() &&  hasBlock() && !hasInstruction()); }
    inline bool isInstruction() { return (hasSection() &&  hasFunction() &&  hasBlock() &&  hasInstruction()); }
    inline bool isValid()       { return (isSection() || isFunction() || isBlock() || isInstruction()); }

    inline uint32_t getSection()     { return (hasSection() ? (entry.fields.section - 1) : INVALID_FIELD); }
    inline uint32_t getFunction()    { return (hasFunction() ? (entry.fields.function - 1) : INVALID_FIELD); }
    inline uint32_t getBlock()       { return (hasBlock() ? (entry.fields.block - 1) : INVALID_FIELD); }
    inline uint32_t getInstruction() { return (hasInstruction() ? (entry.fields.instruction - 1) : INVALID_FIELD); }

};

extern bool allSpace(char* str);

extern bool isAddressAligned(uint64_t addr, uint32_t align);
extern bool isPowerOfTwo(uint32_t n);
extern uint32_t logBase2(uint32_t n);
extern uint64_t nextAlignAddress(uint64_t addr, uint32_t align);
extern uint64_t nextAlignAddressHalfWord(uint64_t addr);
extern uint64_t nextAlignAddressWord(uint64_t addr);
extern uint64_t nextAlignAddressDouble(uint64_t addr);

extern int compareHashCode(const void* arg1,const void* arg2);
extern int searchHashCode(const void* arg1, const void* arg2);
extern int compareBaseAddress(const void* arg1,const void* arg2);
extern int searchBasicBlockAddress(const void* arg1, const void* arg2);
extern int searchBaseAddressExact(const void* arg1, const void* arg2);
extern int searchBaseAddress(const void* arg1, const void* arg2);

extern uint64_t getUInt64(char* buf);
extern uint32_t getUInt32(char* buf);
extern uint16_t getUInt16(char* buf);
extern int64_t absoluteValue(uint64_t d);

extern int32_t scmp(const void *a, const void *b);

extern char mapCharsToByte(char c1, char c2);
extern void printBufferPretty(char* buff, uint32_t sizeInBytes, uint64_t baseAddress, uint32_t bytesPerWord, uint32_t bytesPerLine);

extern char getHexValue(char c1);

class X86Instruction;
class ElfFileInst;

extern uint32_t searchFileList(Vector<char*>* list, char* name);
extern uint32_t initializeFileList(char* fileName, Vector<char*>* list);
extern bool regexMatch(char* string, char* reg);

#define FIRST_HALFWORD(__n) ((__n) & 0xffff)
#define SECOND_HALFWORD(__n) (((__n) >> 16) & 0xffff)

#define rotateleft(x,n) ((x<<n) | (x>>(32-n)))
#define rotateright(x,n) ((x>>n) | (x<<(32-n)))

extern FILE* GetTempFile(char** sfn);

extern void SHA1(unsigned char * str1);

//sha1 functions                                                                                                                                             
void calc(const void *src, const int bytelength, unsigned char *hash);
void toHexString(const unsigned char *hash, char *hexstring);
char* sha1sum(char* buffer, uint32_t size, uint64_t* first64);

extern double timer();

#define WEDGE_SHAMT 0x400000

#endif
