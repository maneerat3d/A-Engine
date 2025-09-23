#pragma once
#include "core/memory/allocator.h"

namespace AEngine::os {

struct FileInfo {
    bool is_directory;
    char filename[260]; // MAX_PATH
};

// Opaque pointer for the iterator state
struct FileIterator;

FileIterator* createFileIterator(const char* path, IAllocator& allocator);
bool getNextFile(FileIterator* iterator, FileInfo* info);
void destroyFileIterator(FileIterator* iterator);

} // namespace AEngine::os