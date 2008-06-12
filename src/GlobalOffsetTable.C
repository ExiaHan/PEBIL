#include <GlobalOffsetTable.h>

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
    PRINT_INFOR("Global Offset Table -- section %d at address 0x%016llx, %d entries at %d bytes each, base entry is %d",
                sectionIndex, baseAddress, numberOfEntries, entrySize, tableBaseIdx);

    printBytes(0,0);

    for (int32_t i = minIndex(); i < maxIndex(); i++){
        PRINT_INFOR("GOT[%d]: 0x%016llx", i, getEntry(i));
    }
}

uint64_t GlobalOffsetTable::getEntry(uint32_t idx){
    ASSERT(idx >= minIndex() && idx < maxIndex() && "index into Global Offset Table is out of bounds");
    ASSERT(entries && "Entries array should be initialized");

    return entries[idx];
}


GlobalOffsetTable::GlobalOffsetTable(char* rawPtr, uint32_t size, uint16_t scnIdx, uint64_t gotSymAddr, ElfFile* elf)
    : RawSection(ElfClassTypes_global_offset_table,rawPtr,size,scnIdx,elf)
{

    entrySize = elfFile->getSectionHeader(sectionIndex)->GET(sh_entsize);

    if (elfFile->is64Bit()){
        ASSERT(entrySize == sizeof(uint64_t) && "GOT entry size is incorrect");
    } else {
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
        delete entries;
    }
}
