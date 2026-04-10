// 3DS Storage Implementation (SDMC File System)
#include "4JLibs/inc/4J_Storage.h"
#include <3ds.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>
#include <vector>

// Global storage manager singleton
C4JStorage StorageManager;

// Internal paths for 3DS save data
#define SDMC_SAVE_PATH "/3ds/MinecraftLCE/saves"
#define SDMC_GLOBAL_PATH "/3ds/MinecraftLCE/global"
#define SDMC_CONTENT_PATH "/3ds/MinecraftLCE/content"

class C4JStorageImpl
{
public:
	std::vector<SAVE_INFO> savedGames;
	char savePath[512];
	char globalPath[512];
	char contentPath[512];
	bool initialized;

	C4JStorageImpl() : initialized(false)
	{
		snprintf(savePath, sizeof(savePath), SDMC_SAVE_PATH);
		snprintf(globalPath, sizeof(globalPath), SDMC_GLOBAL_PATH);
		snprintf(contentPath, sizeof(contentPath), SDMC_CONTENT_PATH);
	}

	bool EnsureDirectory(const char *path)
	{
		struct stat st;
		if(stat(path, &st) != 0)
		{
			// Directory doesn't exist, create it
			return mkdir(path, 0755) == 0 || stat(path, &st) == 0;
		}
		return S_ISDIR(st.st_mode);
	}
};

static C4JStorageImpl g_storageImpl;

void C4JStorage::Initialise()
{
	if(g_storageImpl.initialized) return;

	// Ensure all directories exist
	g_storageImpl.EnsureDirectory("/3ds");
	g_storageImpl.EnsureDirectory("/3ds/MinecraftLCE");
	g_storageImpl.EnsureDirectory(g_storageImpl.savePath);
	g_storageImpl.EnsureDirectory(g_storageImpl.globalPath);
	g_storageImpl.EnsureDirectory(g_storageImpl.contentPath);

	g_storageImpl.initialized = true;
}

void C4JStorage::Shutdown()
{
	// Clean up resources
	for(auto &save : g_storageImpl.savedGames)
	{
		if(save.thumbnailData)
		{
			free(save.thumbnailData);
			save.thumbnailData = nullptr;
		}
	}
	g_storageImpl.savedGames.clear();
}

int C4JStorage::GetSaveCount()
{
	return g_storageImpl.savedGames.size();
}

int C4JStorage::GetSaveInfo(PSAVE_DETAILS pSaveDetailsOut)
{
	if(!pSaveDetailsOut) return -1;

	pSaveDetailsOut->iSaveC = g_storageImpl.savedGames.size();
	pSaveDetailsOut->SaveInfoA = g_storageImpl.savedGames.data();

	return g_storageImpl.savedGames.size();
}

int C4JStorage::CreateSaveContainer(const char *szDisplayName, const char *szSaveFilename)
{
	if(!szSaveFilename || !szDisplayName) return -1;

	char filepath[512];
	snprintf(filepath, sizeof(filepath), "%s/%s", g_storageImpl.savePath, szSaveFilename);

	// Create directory for this save
	return mkdir(filepath, 0755) == 0 ? 0 : -1;
}

int C4JStorage::DeleteSaveContainer(const char *szSaveFilename)
{
	if(!szSaveFilename) return -1;

	char filepath[512];
	snprintf(filepath, sizeof(filepath), "%s/%s", g_storageImpl.savePath, szSaveFilename);

	// Remove directory (simplified - doesn't do deep delete)
	return rmdir(filepath);
}

int C4JStorage::RenameSaveContainer(const char *szOldFilename, const char *szNewFilename)
{
	if(!szOldFilename || !szNewFilename) return -1;

	char oldpath[512], newpath[512];
	snprintf(oldpath, sizeof(oldpath), "%s/%s", g_storageImpl.savePath, szOldFilename);
	snprintf(newpath, sizeof(newpath), "%s/%s", g_storageImpl.savePath, szNewFilename);

	return rename(oldpath, newpath);
}

bool C4JStorage::SaveData(const char *szSaveFilename, const void *pData, uint32_t dwDataSize, const void *pThumbnail, uint32_t dwThumbnailSize)
{
	if(!szSaveFilename || !pData || dwDataSize == 0) return false;

	char datapath[512], thumbpath[512];
	snprintf(datapath, sizeof(datapath), "%s/%s/data.bin", g_storageImpl.savePath, szSaveFilename);
	snprintf(thumbpath, sizeof(thumbpath), "%s/%s/thumb.png", g_storageImpl.savePath, szSaveFilename);

	// Ensure directory exists
	char dirpath[512];
	snprintf(dirpath, sizeof(dirpath), "%s/%s", g_storageImpl.savePath, szSaveFilename);
	mkdir(dirpath, 0755);

	// Write data file
	FILE *f = fopen(datapath, "wb");
	if(!f) return false;

	bool success = fwrite(pData, 1, dwDataSize, f) == dwDataSize;
	fclose(f);

	// Write thumbnail if provided
	if(success && pThumbnail && dwThumbnailSize > 0)
	{
		f = fopen(thumbpath, "wb");
		if(f)
		{
			fwrite(pThumbnail, 1, dwThumbnailSize, f);
			fclose(f);
		}
	}

	return success;
}

bool C4JStorage::LoadData(const char *szSaveFilename, void *pDataOut, uint32_t dwMaxSize, uint32_t *pdwDataSizeOut)
{
	if(!szSaveFilename || !pDataOut || dwMaxSize == 0) return false;

	char datapath[512];
	snprintf(datapath, sizeof(datapath), "%s/%s/data.bin", g_storageImpl.savePath, szSaveFilename);

	FILE *f = fopen(datapath, "rb");
	if(!f) return false;

	// Get file size
	fseek(f, 0, SEEK_END);
	uint32_t filesize = ftell(f);
	fseek(f, 0, SEEK_SET);

	if(filesize > dwMaxSize)
	{
		fclose(f);
		return false;
	}

	uint32_t bytesRead = fread(pDataOut, 1, filesize, f);
	fclose(f);

	if(pdwDataSizeOut)
	{
		*pdwDataSizeOut = bytesRead;
	}

	return bytesRead == filesize;
}

bool C4JStorage::GetSaveMetadata(const char *szSaveFilename, CONTAINER_METADATA *pMetadataOut)
{
	if(!szSaveFilename || !pMetadataOut) return false;

	char datapath[512];
	snprintf(datapath, sizeof(datapath), "%s/%s/data.bin", g_storageImpl.savePath, szSaveFilename);

	struct stat st;
	if(stat(datapath, &st) != 0) return false;

	pMetadataOut->modifiedTime = st.st_mtime;
	pMetadataOut->dataSize = st.st_size;
	pMetadataOut->thumbnailSize = 0;

	// Check thumbnail size
	char thumbpath[512];
	snprintf(thumbpath, sizeof(thumbpath), "%s/%s/thumb.png", g_storageImpl.savePath, szSaveFilename);
	if(stat(thumbpath, &st) == 0)
	{
		pMetadataOut->thumbnailSize = st.st_size;
	}

	return true;
}

bool C4JStorage::SaveGlobalData(eGlobalStorage eType, const void *pData, uint32_t dwDataSize)
{
	if(!pData || dwDataSize == 0) return false;

	char filepath[512];
	snprintf(filepath, sizeof(filepath), "%s/global_%d.bin", g_storageImpl.globalPath, eType);

	FILE *f = fopen(filepath, "wb");
	if(!f) return false;

	bool success = fwrite(pData, 1, dwDataSize, f) == dwDataSize;
	fclose(f);

	return success;
}

bool C4JStorage::LoadGlobalData(eGlobalStorage eType, void *pDataOut, uint32_t dwMaxSize, uint32_t *pdwDataSizeOut)
{
	if(!pDataOut || dwMaxSize == 0) return false;

	char filepath[512];
	snprintf(filepath, sizeof(filepath), "%s/global_%d.bin", g_storageImpl.globalPath, eType);

	FILE *f = fopen(filepath, "rb");
	if(!f) return false;

	fseek(f, 0, SEEK_END);
	uint32_t filesize = ftell(f);
	fseek(f, 0, SEEK_SET);

	if(filesize > dwMaxSize)
	{
		fclose(f);
		return false;
	}

	uint32_t bytesRead = fread(pDataOut, 1, filesize, f);
	fclose(f);

	if(pdwDataSizeOut)
	{
		*pdwDataSizeOut = bytesRead;
	}

	return bytesRead == filesize;
}

bool C4JStorage::FileExists(const char *szRelativePath)
{
	if(!szRelativePath) return false;

	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/%s", g_storageImpl.savePath, szRelativePath);

	struct stat st;
	return stat(fullpath, &st) == 0;
}

bool C4JStorage::CreateDirectory(const char *szDirectoryPath)
{
	if(!szDirectoryPath) return false;

	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/%s", g_storageImpl.savePath, szDirectoryPath);

	return mkdir(fullpath, 0755) == 0 || FileExists(szDirectoryPath);
}

bool C4JStorage::DeleteFile(const char *szRelativePath)
{
	if(!szRelativePath) return false;

	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/%s", g_storageImpl.savePath, szRelativePath);

	return unlink(fullpath) == 0;
}

bool C4JStorage::ReadFile(const char *szRelativePath, void *pDataOut, uint32_t dwMaxSize, uint32_t *pdwBytesRead)
{
	if(!szRelativePath || !pDataOut) return false;

	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/%s", g_storageImpl.savePath, szRelativePath);

	FILE *f = fopen(fullpath, "rb");
	if(!f) return false;

	uint32_t bytesRead = fread(pDataOut, 1, dwMaxSize, f);
	fclose(f);

	if(pdwBytesRead)
	{
		*pdwBytesRead = bytesRead;
	}

	return bytesRead > 0;
}

bool C4JStorage::WriteFile(const char *szRelativePath, const void *pData, uint32_t dwDataSize, bool bCreateDirectories)
{
	if(!szRelativePath || !pData || dwDataSize == 0) return false;

	char fullpath[512];
	snprintf(fullpath, sizeof(fullpath), "%s/%s", g_storageImpl.savePath, szRelativePath);

	// Create parent directories if requested
	if(bCreateDirectories)
	{
		char dirpath[512];
		strcpy(dirpath, fullpath);
		for(int i = strlen(dirpath) - 1; i > 0; i--)
		{
			if(dirpath[i] == '/')
			{
				dirpath[i] = 0;
				mkdir(dirpath, 0755);
				break;
			}
		}
	}

	FILE *f = fopen(fullpath, "wb");
	if(!f) return false;

	bool success = fwrite(pData, 1, dwDataSize, f) == dwDataSize;
	fclose(f);

	return success;
}

bool C4JStorage::SaveThumbnail(const char *szFilename, int width, int height, const void *pImageData)
{
	if(!szFilename || !pImageData || width <= 0 || height <= 0) return false;

	char thumbpath[512];
	snprintf(thumbpath, sizeof(thumbpath), "%s/%s.thumb", g_storageImpl.savePath, szFilename);

	// Simple thumbnail save (just raw pixel data for now)
	FILE *f = fopen(thumbpath, "wb");
	if(!f) return false;

	bool success = fwrite(pImageData, width * height * 4, 1, f) == 1;
	fclose(f);

	return success;
}

bool C4JStorage::LoadThumbnail(const char *szFilename, int *pWidthOut, int *pHeightOut, void *pImageDataOut)
{
	// Thumbnail loading (placeholder)
	return false;
}

int C4JStorage::GetInstalledContentCount()
{
	return 0;  // No DLC support on 3DS
}

bool C4JStorage::GetContentInfo(int index, char *szContentNameOut, uint32_t *pdwSizeOut)
{
	return false;
}

void C4JStorage::RequestAsyncSave(const char *szSaveFilename, const void *pData, uint32_t dwDataSize,
                                   StorageCallback pCallback, void *lpParam)
{
	// On 3DS, just do it synchronously
	if(SaveData(szSaveFilename, pData, dwDataSize, nullptr, 0))
	{
		if(pCallback) pCallback(lpParam, 0);
	}
	else
	{
		if(pCallback) pCallback(lpParam, -1);
	}
}

void C4JStorage::RequestAsyncLoad(const char *szSaveFilename, void *pDataOut, uint32_t dwMaxSize,
                                   StorageCallback pCallback, void *lpParam)
{
	uint32_t dataSize = 0;
	if(LoadData(szSaveFilename, pDataOut, dwMaxSize, &dataSize))
	{
		if(pCallback) pCallback(lpParam, 0);
	}
	else
	{
		if(pCallback) pCallback(lpParam, -1);
	}
}

void C4JStorage::Tick()
{
	// Process any pending async operations
}

const char* C4JStorage::GetSaveDataPath()
{
	return g_storageImpl.savePath;
}

const char* C4JStorage::GetGlobalDataPath()
{
	return g_storageImpl.globalPath;
}

const char* C4JStorage::GetContentPath()
{
	return g_storageImpl.contentPath;
}
