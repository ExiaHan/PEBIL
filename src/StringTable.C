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

#include <StringTable.h>

#include <BinaryFile.h>
#include <ElfFile.h>
#include <SectionHeader.h>

void StringTable::wedge(uint32_t shamt){
    // don't need to do anything
}

char* StringTable::getString(uint32_t offset){
    ASSERT(offset >= 0 && offset < sizeInBytes && "Cannot look up a string outside the string table"); 
    return strings + offset; 
}

uint32_t StringTable::addString(const char* name){

    uint32_t stringSize = strlen(name);
    uint32_t currentSize = sizeInBytes;

    ASSERT(strings && "strings array should be initialized");

    char* newstrings = new char[sizeInBytes + strlen(name) + 1];
    memcpy(newstrings, strings, sizeInBytes);
    memcpy(newstrings + sizeInBytes, name, strlen(name));
    newstrings[sizeInBytes + strlen(name)] = '\0';

    delete[] strings;
    strings = newstrings;
    sizeInBytes += strlen(name) + 1;

    return currentSize;
}

void StringTable::print() { 

    PRINT_INFOR("StringTable : %d aka sect %d %uB",index,getSectionIndex(),sizeInBytes);
    for (uint32_t currByte = 0; currByte < sizeInBytes; currByte++){
        char* ptr = getString(currByte);
        PRINT_INFOR("\t%7d : %s",currByte,ptr);
        currByte += strlen(ptr);
    }

}

void StringTable::dump(BinaryOutputFile* binaryOutputFile, uint32_t offset){
    binaryOutputFile->copyBytes(strings, sizeInBytes, offset);
}

uint32_t StringTable::read(BinaryInputFile* binaryInputFile){
    binaryInputFile->setInPointer(getFilePointer());
    setFileOffset(binaryInputFile->currentOffset());

    strings = new char[sizeInBytes];
    binaryInputFile->copyBytesIterate(strings, sizeInBytes);

    return sizeInBytes;
}

StringTable::~StringTable(){
    if (strings){
        delete[] strings;
    }
}
