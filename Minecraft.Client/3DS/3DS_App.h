#pragma once

#include "..\Common\Consoles_App.h"

// 3DS-specific app type that keeps the common console app contract without
// relying on Windows platform headers.
class CConsoleMinecraftApp : public CMinecraftApp
{
public:
	CConsoleMinecraftApp();

	virtual void SetRichPresenceContext(int iPad, int contextId) override;
	virtual void CaptureSaveThumbnail() override;
	virtual void GetSaveThumbnail(PBYTE *ppbThumbnailData, DWORD *pdwThumbnailSize) override;
	virtual void ReleaseSaveThumbnail() override;
	virtual void GetScreenshot(int iPad, PBYTE *pbData, DWORD *pdwSize) override;
	virtual void ReadBannedList(int iPad, eTMSAction action = static_cast<eTMSAction>(0), bool bCallback = false) override;
	virtual int LoadLocalTMSFile(WCHAR *wchTMSFile) override;
	virtual int LoadLocalTMSFile(WCHAR *wchTMSFile, eFileExtensionType eExt) override;
	virtual void FreeLocalTMSFiles(eTMSFileType eType) override;
	virtual int GetLocalTMSFileIndex(WCHAR *wchTMSFile, bool bFilenameIncludesExtension, eFileExtensionType eEXT = eFileExtensionType_PNG) override;
};

extern CConsoleMinecraftApp app;
