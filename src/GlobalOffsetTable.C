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

#include <GlobalOffsetTable.h>

#include <ElfFile.h>
#include <SectionHeader.h>

void GlobalOffsetTable::wedge(uint32_t shamt){
    for (uint32_t i = 0; i < numberOfEntries; i++){
        if (elfFile->isWedgeAddress(entries[i])){
            entries[i] += shamt;
        }
    }
    baseAddress += shamt;
}

void GlobalOffsetTable::dump(BinaryOutputFile* binaryOutputFile, uint32_t offset){
    uint32_t currByte = 0;

    for (uint32_t i = 0; i < numberOfEntries; i++){
        if (elfFile->is64Bit()){
            binaryOutputFile->copyBytes((char*)&entries[i],entrySize,offset+currByte);
        } else {
            uint32_t tmpEntry = (uint32_t)entries[i];
            binaryOutputFile->copyBytes((char*)&tmpEntry,entrySize,offset+currByte);
        }
        currByte += entrySize;
    }
    
}


uint32_t GlobalOffsetTable::read(BinaryInputFile* binaryInputFile){

    binaryInputFile->setInPointer(rawDataPtr);
    setFileOffset(binaryInputFile->currentOffset());
    
    for (uint32_t i = 0; i < numberOfEntries; i++){
        if (elfFile->is64Bit()){
            if (!binaryInputFile->copyBytesIterate(&entries[i],entrySize)){
                PRINT_ERROR("Global Offset Table entry %d (64) cannot be read", i);
            }
        } else {
            uint32_t tmpEntry;
            if (!binaryInputFile->copyBytesIterate(&tmpEntry,entrySize)){
                PRINT_ERROR("Global Offset Table entry %d (32) cannot be read", i);
            } 
            entries[i] = (uint64_t)tmpEntry;
        }
    }

    return sizeInBytes;
}


void GlobalOffsetTable::print(){
    PRINT_INFOR("GOT : with %d x %dB",numberOfEntries,entrySize);
    PRINT_INFOR("\tsect : %d",sectionIndex);
    PRINT_INFOR("\tbase : %d",tableBaseIdx);
    PRINT_INFOR("\taddr : %#llx",baseAddress);

    char tmpStr[__MAX_STRING_SIZE+1];
    for (int32_t i = minIndex(); i < maxIndex(); i++){
        uint64_t val = getEntry(i);
        char* namestr = NULL;
        Symbol* foundsymbols[3];
        getElfFile()->findSymbol4Addr(val, foundsymbols, 3, &namestr);
        PRINT_INFOR("\t%5d : %#12llx -- %s",i,val,namestr);
        delete[] namestr;
    }
}

uint64_t GlobalOffsetTable::getEntry(uint32_t idx){
    ASSERT(idx >= minIndex() && idx < maxIndex() && "index into Global Offset Table is out of bounds");
    ASSERT(entries && "Entries array should be initialized");

    return entries[idx];
}


GlobalOffsetTable::GlobalOffsetTable(char* rawPtr, uint32_t size, uint16_t scnIdx, uint64_t gotSymAddr, ElfFile* elf)
    : RawSection(PebilClassType_GlobalOffsetTable,rawPtr,size,scnIdx,elf)
{

    entrySize = elfFile->getSectionHeader(sectionIndex)->GET(sh_entsize);
 
    if (elfFile->is64Bit()){
        if (entrySize != sizeof(uint64_t)){
            PRINT_WARN(10, "Global offset table entry size is %d... setting it to a sane value", entrySize);
            entrySize = sizeof(uint64_t);
        }
        ASSERT(entrySize == sizeof(uint64_t) && "GOT entry size is incorrect");
    } else {
        if (entrySize != sizeof(uint32_t)){
            PRINT_WARN(10, "Global offset table entry size is %d... setting it to a sane value", entrySize);
            entrySize = sizeof(uint32_t);
        }
        ASSERT(entrySize == sizeof(uint32_t) && "GOT entry size is incorrect"); 
    }


    baseAddress = gotSymAddr;

    uint64_t addrOffset = baseAddress - elfFile->getSectionHeader(sectionIndex)->GET(sh_addr);
    ASSERT(addrOffset % entrySize == 0 &&
           "The byte offset in the Global Offset Table must be divisible by the entry size");
    tableBaseIdx = addrOffset / entrySize;


    ASSERT(sizeInBytes % entrySize == 0 &&
           "The number of bytes in the Global Offset Table must be divisible by the entry size");
    numberOfEntries = sizeInBytes / entrySize;

    entries = new uint64_t[numberOfEntries];
}

GlobalOffsetTable::~GlobalOffsetTable(){
    if (entries){
        delete[] entries;
    }
}

