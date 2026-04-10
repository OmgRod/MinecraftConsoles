#include "stdafx.h"
#include "lce_filesystem.h"

#if defined(__3DS__)
#include <dirent.h>
#include <sys/stat.h>
#include <cstring>
#else
#if __cplusplus >= 201703L
#include <filesystem>
#else
#include <experimental/filesystem>
#endif
#endif

#include <stdio.h>

#if !defined(__3DS__)
#if __cplusplus >= 201703L
namespace fs = std::filesystem;
#else
namespace fs = std::experimental::filesystem;
#endif
#endif

bool FileOrDirectoryExists(const char* path)
{
    if (path == nullptr) return false;
#if defined(__3DS__)
    struct stat st;
    return stat(path, &st) == 0;
#else
    std::error_code ec;
    return fs::exists(fs::u8path(path), ec) && !ec;
#endif
}

bool FileExists(const char* path)
{
    if (path == nullptr) return false;
#if defined(__3DS__)
    struct stat st;
    return (stat(path, &st) == 0) && S_ISREG(st.st_mode);
#else
    std::error_code ec;
    return fs::is_regular_file(fs::u8path(path), ec) && !ec;
#endif
}

bool DirectoryExists(const char* path)
{
    if (path == nullptr) return false;
#if defined(__3DS__)
    struct stat st;
    return (stat(path, &st) == 0) && S_ISDIR(st.st_mode);
#else
    std::error_code ec;
    return fs::is_directory(fs::u8path(path), ec) && !ec;
#endif
}

bool GetFirstFileInDirectory(const char* directory, char* outFilePath, size_t outFilePathSize)
{
    if (directory == nullptr || outFilePath == nullptr || outFilePathSize == 0) return false;

#if defined(__3DS__)
    DIR* dir = opendir(directory);
    if (dir == nullptr) return false;

    struct dirent* entry = nullptr;
    while ((entry = readdir(dir)) != nullptr)
    {
        if (std::strcmp(entry->d_name, ".") == 0 || std::strcmp(entry->d_name, "..") == 0)
        {
            continue;
        }

        char candidate[1024];
        std::snprintf(candidate, sizeof(candidate), "%s/%s", directory, entry->d_name);
        candidate[sizeof(candidate) - 1] = '\0';

        struct stat st;
        if (stat(candidate, &st) == 0 && S_ISREG(st.st_mode))
        {
            std::snprintf(outFilePath, outFilePathSize, "%s", candidate);
            outFilePath[outFilePathSize - 1] = '\0';
            closedir(dir);
            return true;
        }
    }

    closedir(dir);
    return false;
#else
    std::error_code ec;
    const fs::path dirPath = fs::u8path(directory);
    if (!fs::is_directory(dirPath, ec) || ec) return false;

    for (const auto& entry : fs::directory_iterator(dirPath, ec))
    {
        if (ec) return false;
        if (entry.is_regular_file(ec) && !ec)
        {
            const std::string firstFile = entry.path().string();
            std::snprintf(outFilePath, outFilePathSize, "%s", firstFile.c_str());
            outFilePath[outFilePathSize - 1] = '\0';
            return true;
        }
    }

    return false;
#endif
}
