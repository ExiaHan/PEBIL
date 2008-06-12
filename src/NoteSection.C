#include <NoteSection.h>


Note::Note(uint32_t index, uint32_t namsz, uint32_t dessz, uint32_t typ, char* nam, char* des){
    namesz = namsz;
    descsz = dessz;
    type = typ;
    name = nam;
    desc = des;
}

uint32_t Note::getNumberOfDescriptors(){
    return nextAlignAddress(descsz, Size__32_bit_Note_Section_Entry)/Size__32_bit_Note_Section_Entry;
}


uint32_t Note::getDescriptor(uint32_t idx){
    ASSERT(idx >= 0 && idx < getNumberOfDescriptors() && "Descriptor table index out of bounds");
    uint32_t descriptor = 0;
    memcpy(&descriptor, &desc[idx*Size__32_bit_Note_Section_Entry], Size__32_bit_Note_Section_Entry);
    return descriptor;
}

bool Note::verify(){
    if (namesz == 0){
        PRINT_ERROR("Note with namesz=0 is reserved for system use");
        return false;
    }
    if (!name || namesz != strlen(name) + 1){
        PRINT_ERROR("Actual name size does not match claimed name size");
        return false;
    }
    return true;
}

void Note::print(){
    PRINT_INFOR("\tNOTE[%d]:\t%d\t%d\t%d\t%s", index, type, namesz, descsz, name);    

    // print the list of descriptors
    fprintf(stdout, "\t\tDESC(0-%d):", getNumberOfDescriptors());
    for (uint32_t i = 0; i < getNumberOfDescriptors(); i++){
        fprintf(stdout, "\t%d", getDescriptor(i));
    }
    fprintf(stdout, "\n");
}

bool NoteSection::verify(){
    for (uint32_t i = 0; i < numberOfNotes; i++){
        if (!notes[i]->verify()){
            return false;
        }
    }
    return true;
}


NoteSection::NoteSection(char* rawPtr, uint32_t size, uint16_t scnIdx, uint32_t idx, ElfFile* elf)
    : RawSection(ElfClassTypes_hash_table,rawPtr,size,scnIdx,elf)
{
    index = idx;

    numberOfNotes = 0;
    notes = NULL;
}

NoteSection::~NoteSection(){
    if (notes){
        for (uint32_t i = 0; i < numberOfNotes; i++){
            if (notes[i]){
                delete notes[i];
            }
        }
    }
}

void NoteSection::print(){
    PRINT_INFOR("NoteSection %d: section idx %d, %d notes", index, sectionIndex, numberOfNotes);
    printBytes(0,0);

    PRINT_INFOR("\t\t\tTYPE\tNAMESZ\tDESCSZ\tNAME");

    for (uint32_t i = 0; i < numberOfNotes; i++){
        ASSERT(notes[i] && "numberOfNotes should indicate the number of elements in the notes array");
        notes[i]->print();
    }
}

uint32_t NoteSection::read(BinaryInputFile* binaryInputFile){

    uint32_t currWord = 0;

    uint32_t tmpNameSize;
    uint32_t tmpDescSize;
    uint32_t tmpType;

    binaryInputFile->setInPointer(rawDataPtr);
    setFileOffset(binaryInputFile->currentOffset());

    ASSERT(sizeInBytes % Size__32_bit_Note_Section_Entry == 0 && "Notes section should be word-aligned");
    uint32_t* rawData = new uint32_t[sizeInBytes/Size__32_bit_Note_Section_Entry];
    if (!binaryInputFile->copyBytes((char*)rawData, sizeInBytes)){
        PRINT_ERROR("Cannot read notes section from file");
    }


    // go over the section once to determine how many note structures are in it
    numberOfNotes = 0;
    while (currWord * Size__32_bit_Note_Section_Entry < sizeInBytes){
        tmpNameSize = rawData[currWord++];
        tmpDescSize = rawData[currWord++];
        tmpType = rawData[currWord++];
        
        currWord += nextAlignAddress(tmpNameSize,Size__32_bit_Note_Section_Entry)/Size__32_bit_Note_Section_Entry;
        currWord += nextAlignAddress(tmpDescSize,Size__32_bit_Note_Section_Entry)/Size__32_bit_Note_Section_Entry;
        numberOfNotes++;

        PRINT_INFOR("Found a note with %d %d %d", tmpNameSize, tmpDescSize, tmpType);
    }

    PRINT_INFOR("Found %d notes", numberOfNotes);
    PRINT_INFOR("%d %d", currWord*Size__32_bit_Note_Section_Entry, sizeInBytes);
    ASSERT(currWord*Size__32_bit_Note_Section_Entry == sizeInBytes && "Number of bytes read from note section is not the same as section size");

    notes = new Note*[numberOfNotes];

    currWord = 0;
    numberOfNotes = 0;
    while (currWord * Size__32_bit_Note_Section_Entry < sizeInBytes){
        tmpNameSize = rawData[currWord++];
        tmpDescSize = rawData[currWord++];
        tmpType = rawData[currWord++];

        char* nameptr = (char*)(rawDataPtr + currWord * Size__32_bit_Note_Section_Entry);
        currWord += nextAlignAddress(tmpNameSize,Size__32_bit_Note_Section_Entry)/Size__32_bit_Note_Section_Entry;
        char* descptr = (char*)(rawDataPtr + currWord * Size__32_bit_Note_Section_Entry);
        currWord += nextAlignAddress(tmpDescSize,Size__32_bit_Note_Section_Entry)/Size__32_bit_Note_Section_Entry;

        notes[numberOfNotes] = new Note(numberOfNotes, tmpNameSize, tmpDescSize, tmpType, nameptr, descptr);
        numberOfNotes++;
    }

    delete[] rawData;
    return sizeInBytes;
}

Note* NoteSection::getNote(uint32_t idx){
    ASSERT(idx >= 0 && idx < numberOfNotes && "Note table index out of bounds");
    ASSERT(notes && "Note table should be initialized");

    return notes[idx];
}

void NoteSection::dump(BinaryOutputFile* binaryOutputFile, uint32_t offset){
    uint32_t currByte = 0;
    Note* currNote;
    uint32_t tmpEntry;
    char* tmpName;

    for (uint32_t i = 0; i < numberOfNotes; i++){
        currNote = getNote(i);
        ASSERT(currNote && "Note table should be initialized");

        tmpEntry = currNote->getNameSize();
        binaryOutputFile->copyBytes((char*)&tmpEntry,Size__32_bit_Note_Section_Entry,offset+currByte);
        currByte += Size__32_bit_Note_Section_Entry;

        tmpEntry = currNote->getDescriptorSize();
        binaryOutputFile->copyBytes((char*)&tmpEntry,Size__32_bit_Note_Section_Entry,offset+currByte);
        currByte += Size__32_bit_Note_Section_Entry;

        tmpEntry = currNote->getType();
        binaryOutputFile->copyBytes((char*)&tmpEntry,Size__32_bit_Note_Section_Entry,offset+currByte);
        currByte += Size__32_bit_Note_Section_Entry;

        tmpName = currNote->getName();
        binaryOutputFile->copyBytes(tmpName,nextAlignAddress(currNote->getNameSize(),Size__32_bit_Note_Section_Entry),offset+currByte);
        currByte += nextAlignAddress(currNote->getNameSize(),Size__32_bit_Note_Section_Entry);

        for (uint32_t j = 0; j < currNote->getNumberOfDescriptors(); j++){
            tmpEntry = currNote->getDescriptor(j);
            binaryOutputFile->copyBytes((char*)&tmpEntry,Size__32_bit_Note_Section_Entry,offset+currByte);
            currByte += Size__32_bit_Note_Section_Entry;
        }
    }
    ASSERT(currByte == sizeInBytes && "Dumped an incorrect number of bytes for NoteSection");
}





