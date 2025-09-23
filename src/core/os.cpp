#include "os.h"
#include "core/string/string.h" // For copyString
#include "core/memory/core.h"
#include <filesystem> // C++17 filesystem library
#include <vector>
#include <iostream>

namespace AEngine::os {

namespace fs = std::filesystem;

struct FileIterator {
    // --- ส่วนที่แก้ไข ---
    FileIterator(IAllocator& allocator) : allocator(allocator) {}
    // --------------------
    IAllocator& allocator;
    // std::vector does not work with our custom allocator directly
    // For simplicity, we will use std::vector with the default allocator here.
    // A more robust solution would be a custom vector class that uses our IAllocator.
    std::vector<fs::path> std_files;
    int current_index = -1;
};

FileIterator* createFileIterator(const char* path, IAllocator& allocator) {
    FileIterator* iter = AENGINE_NEW(allocator, FileIterator)(allocator);
    try {
        // Ensure the path exists and is a directory before iterating
        if (fs::exists(path) && fs::is_directory(path)) {
            for (const auto& entry : fs::directory_iterator(path)) {
                iter->std_files.push_back(entry.path());
            }
        }
    } catch (const fs::filesystem_error& e) {
        // Log the error but don't crash
        std::cerr << "Filesystem error: " << e.what() << std::endl;
    }
    iter->current_index = -1;
    return iter;
}

bool getNextFile(FileIterator* iterator, FileInfo* info) {
    if (!iterator) return false;
    iterator->current_index++;
    if (iterator->current_index >= iterator->std_files.size()) {
        return false;
    }

    const auto& path = iterator->std_files[iterator->current_index];
    info->is_directory = fs::is_directory(path);
    
    std::string filename_str = path.filename().string();
    AEngine::copyString(AEngine::Span(info->filename), AEngine::StringView(filename_str.c_str()));

    return true;
}

void destroyFileIterator(FileIterator* iterator) {
    if (iterator) {
        AENGINE_DELETE(iterator->allocator, iterator);
    }
}

} // namespace AEngine::os