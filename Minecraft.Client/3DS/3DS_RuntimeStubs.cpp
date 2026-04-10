#include "..\stdafx.h"

extern "C" void *__aeabi_read_tp()
{
	static int tls_stub = 0;
	return &tls_stub;
}

void MemSect(int section)
{
	(void)section;
}

void XMemCpy(void *a, const void *b, size_t s)
{
	memcpy(a, b, s);
}

void XMemSet(void *a, int t, size_t s)
{
	memset(a, t, s);
}

void XMemSet128(void *a, int t, size_t s)
{
	memset(a, t, s);
}

void *XPhysicalAlloc(SIZE_T a, ULONG_PTR b, ULONG_PTR c, DWORD d)
{
	(void)b;
	(void)c;
	(void)d;
	return malloc(a);
}

void XPhysicalFree(void *a)
{
	free(a);
}

C_4JInput InputManager;
C4JRender RenderManager;
C_4JProfile ProfileManager;
C4JStorage StorageManager;
CConsoleMinecraftApp app;
ConsoleUIController ui;

extern "C" int __system_argc = 0;
extern "C" char **__system_argv = nullptr;

extern "C" void initSystem()
{
}

CConsoleMinecraftApp::CConsoleMinecraftApp() {}

void CConsoleMinecraftApp::SetRichPresenceContext(int, int) {}
void CConsoleMinecraftApp::CaptureSaveThumbnail() {}
void CConsoleMinecraftApp::GetSaveThumbnail(PBYTE *ppbThumbnailData, DWORD *pdwThumbnailSize)
{
	if (ppbThumbnailData != nullptr)
	{
		*ppbThumbnailData = nullptr;
	}
	if (pdwThumbnailSize != nullptr)
	{
		*pdwThumbnailSize = 0;
	}
}
void CConsoleMinecraftApp::ReleaseSaveThumbnail() {}
void CConsoleMinecraftApp::GetScreenshot(int, PBYTE *pbData, DWORD *pdwSize)
{
	if (pbData != nullptr)
	{
		*pbData = nullptr;
	}
	if (pdwSize != nullptr)
	{
		*pdwSize = 0;
	}
}
void CConsoleMinecraftApp::ReadBannedList(int, eTMSAction, bool) {}
int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR *) { return -1; }
int CConsoleMinecraftApp::LoadLocalTMSFile(WCHAR *, eFileExtensionType) { return -1; }
void CConsoleMinecraftApp::FreeLocalTMSFiles(eTMSFileType) {}
int CConsoleMinecraftApp::GetLocalTMSFileIndex(WCHAR *, bool, eFileExtensionType) { return -1; }

void ConsoleUIController::init(S32, S32) {}
void ConsoleUIController::render() {}
void ConsoleUIController::shutdown() {}
void ConsoleUIController::beginIggyCustomDraw4J(IggyCustomDrawCallbackRegion *, CustomDrawData *) {}
CustomDrawData *ConsoleUIController::setupCustomDraw(UIScene *, IggyCustomDrawCallbackRegion *) { return nullptr; }
CustomDrawData *ConsoleUIController::calculateCustomDraw(IggyCustomDrawCallbackRegion *) { return nullptr; }
void ConsoleUIController::endCustomDraw(IggyCustomDrawCallbackRegion *) {}
void ConsoleUIController::setTileOrigin(S32 xPos, S32 yPos) { UIController::setTileOrigin(xPos, yPos); }
GDrawTexture *ConsoleUIController::getSubstitutionTexture(int) { return nullptr; }
void ConsoleUIController::destroySubstitutionTexture(void *, GDrawTexture *) {}

int main(int, char **)
{
	return 0;
}

wchar_t g_Win64UsernameW[17] = L"Player";
char g_Win64Username[17] = "Player";

static bool g_saveDisabled = false;
static int g_lockedProfile = -1;
static wchar_t g_keyboardText[256] = {0};
static wchar_t g_saveTitle[128] = {0};
static SAVE_INFO g_dummySaveInfo = {};
static SAVE_DETAILS g_dummySaveDetails = {0, nullptr};
static XMARKETPLACE_CONTENTOFFER_INFO g_dummyOffer = {};

void C_4JInput::Initialise(int, unsigned char, unsigned char, unsigned char) {}
void C_4JInput::Tick(void) {}
void C_4JInput::SetDeadzoneAndMovementRange(unsigned int, unsigned int) {}
void C_4JInput::SetGameJoypadMaps(unsigned char, unsigned char, unsigned int) {}
unsigned int C_4JInput::GetGameJoypadMaps(unsigned char, unsigned char) { return 0; }
void C_4JInput::SetJoypadMapVal(int, unsigned char) {}
unsigned char C_4JInput::GetJoypadMapVal(int) { return 0; }
void C_4JInput::SetJoypadSensitivity(int, float) {}
unsigned int C_4JInput::GetValue(int, unsigned char, bool) { return 0; }
bool C_4JInput::ButtonPressed(int, unsigned char) { return false; }
bool C_4JInput::ButtonReleased(int, unsigned char) { return false; }
bool C_4JInput::ButtonDown(int, unsigned char) { return false; }
void C_4JInput::SetJoypadStickAxisMap(int, unsigned int, unsigned int) {}
void C_4JInput::SetJoypadStickTriggerMap(int, unsigned int, unsigned int) {}
void C_4JInput::SetKeyRepeatRate(float, float) {}
void C_4JInput::SetDebugSequence(const char *, int (*)(LPVOID), LPVOID) {}
FLOAT C_4JInput::GetIdleSeconds(int) { return 0.0f; }
bool C_4JInput::IsPadConnected(int iPad) { return iPad >= 0 && iPad < XUSER_MAX_COUNT; }
float C_4JInput::GetJoypadStick_LX(int, bool) { return 0.0f; }
float C_4JInput::GetJoypadStick_LY(int, bool) { return 0.0f; }
float C_4JInput::GetJoypadStick_RX(int, bool) { return 0.0f; }
float C_4JInput::GetJoypadStick_RY(int, bool) { return 0.0f; }
unsigned char C_4JInput::GetJoypadLTrigger(int, bool) { return 0; }
unsigned char C_4JInput::GetJoypadRTrigger(int, bool) { return 0; }
void C_4JInput::SetMenuDisplayed(int, bool) {}
EKeyboardResult C_4JInput::RequestKeyboard(LPCWSTR, LPCWSTR Text, DWORD, UINT, int (*Func)(LPVOID, const bool), LPVOID lpParam, C_4JInput::EKeyboardMode)
{
	if (Text != nullptr)
	{
		wcsncpy(g_keyboardText, Text, 255);
		g_keyboardText[255] = L'\0';
	}
	if (Func != nullptr)
	{
		Func(lpParam, true);
	}
	return EKeyboard_ResultAccept;
}
void C_4JInput::GetText(uint16_t *UTF16String)
{
	if (UTF16String != nullptr)
	{
		wcscpy(reinterpret_cast<wchar_t *>(UTF16String), g_keyboardText);
	}
}
bool C_4JInput::VerifyStrings(WCHAR **, int, int (*)(LPVOID, STRING_VERIFY_RESPONSE *), LPVOID) { return true; }
void C_4JInput::CancelQueuedVerifyStrings(int (*)(LPVOID, STRING_VERIFY_RESPONSE *), LPVOID) {}
void C_4JInput::CancelAllVerifyInProgress(void) {}

void C4JRender::Tick() {}
void C4JRender::UpdateGamma(unsigned short) {}
bool C4JRender::IsWidescreen() { return true; }
bool C4JRender::IsHiDef() { return true; }
void C4JRender::MatrixMode(int) {}
void C4JRender::MatrixSetIdentity() {}
void C4JRender::MatrixTranslate(float, float, float) {}
void C4JRender::MatrixRotate(float, float, float, float) {}
void C4JRender::MatrixScale(float, float, float) {}
void C4JRender::MatrixPerspective(float, float, float, float) {}
void C4JRender::MatrixOrthogonal(float, float, float, float, float, float) {}
void C4JRender::MatrixPop() {}
void C4JRender::MatrixPush() {}
void C4JRender::MatrixMult(float *) {}
const float *C4JRender::MatrixGet(int)
{
	static float ident[16] = {1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f};
	return ident;
}
void C4JRender::Set_matrixDirty() {}
void C4JRender::Initialise(ID3D11Device *, IDXGISwapChain *) {}
void C4JRender::InitialiseContext() {}
void C4JRender::StartFrame() {}
void C4JRender::DoScreenGrabOnNextPresent() {}
void C4JRender::Present() {}
void C4JRender::Clear(int, D3D11_RECT *) {}
void C4JRender::SetClearColour(const float[4]) {}
void C4JRender::CaptureThumbnail(ImageFileBuffer *pngOut)
{
	if (pngOut != nullptr)
	{
		pngOut->m_pBuffer = nullptr;
		pngOut->m_bufferSize = 0;
	}
}
void C4JRender::CaptureScreen(ImageFileBuffer *, XSOCIAL_PREVIEWIMAGE *) {}
void C4JRender::BeginConditionalSurvey(int) {}
void C4JRender::EndConditionalSurvey() {}
void C4JRender::BeginConditionalRendering(int) {}
void C4JRender::EndConditionalRendering() {}
void C4JRender::DrawVertices(ePrimitiveType, int, void *, eVertexType, C4JRender::ePixelShaderType) {}
void C4JRender::DrawVertexBuffer(ePrimitiveType, int, ID3D11Buffer *, C4JRender::eVertexType, C4JRender::ePixelShaderType) {}
void C4JRender::CBuffLockStaticCreations() {}
int C4JRender::CBuffCreate(int) { return 1; }
void C4JRender::CBuffDelete(int, int) {}
void C4JRender::CBuffStart(int, bool) {}
void C4JRender::CBuffClear(int) {}
int C4JRender::CBuffSize(int) { return 0; }
void C4JRender::CBuffEnd() {}
bool C4JRender::CBuffCall(int, bool) { return true; }
void C4JRender::CBuffTick() {}
void C4JRender::CBuffDeferredModeStart() {}
void C4JRender::CBuffDeferredModeEnd() {}
int C4JRender::TextureCreate()
{
	static int nextTexture = 1;
	return nextTexture++;
}
void C4JRender::TextureFree(int) {}
void C4JRender::TextureBind(int) {}
void C4JRender::TextureBindVertex(int) {}
void C4JRender::TextureSetTextureLevels(int) {}
int C4JRender::TextureGetTextureLevels() { return 1; }
void C4JRender::TextureData(int, int, void *, int, eTextureFormat) {}
void C4JRender::TextureDataUpdate(int, int, int, int, void *, int) {}
void C4JRender::TextureSetParam(int, int) {}
void C4JRender::TextureDynamicUpdateStart() {}
void C4JRender::TextureDynamicUpdateEnd() {}
HRESULT C4JRender::LoadTextureData(const char *, D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut)
{
	if (pSrcInfo != nullptr)
	{
		pSrcInfo->Width = 0;
		pSrcInfo->Height = 0;
	}
	if (ppDataOut != nullptr)
	{
		*ppDataOut = nullptr;
	}
	return E_FAIL;
}
HRESULT C4JRender::LoadTextureData(BYTE *, DWORD, D3DXIMAGE_INFO *pSrcInfo, int **ppDataOut)
{
	if (pSrcInfo != nullptr)
	{
		pSrcInfo->Width = 0;
		pSrcInfo->Height = 0;
	}
	if (ppDataOut != nullptr)
	{
		*ppDataOut = nullptr;
	}
	return E_FAIL;
}
HRESULT C4JRender::SaveTextureData(const char *, D3DXIMAGE_INFO *, int *) { return E_FAIL; }
HRESULT C4JRender::SaveTextureDataToMemory(void *, int, int *, int, int, int *) { return E_FAIL; }
void C4JRender::TextureGetStats() {}
ID3D11ShaderResourceView *C4JRender::TextureGetTexture(int) { return nullptr; }
void C4JRender::StateSetColour(float, float, float, float) {}
void C4JRender::StateSetDepthMask(bool) {}
void C4JRender::StateSetBlendEnable(bool) {}
void C4JRender::StateSetBlendFunc(int, int) {}
void C4JRender::StateSetBlendFactor(unsigned int) {}
void C4JRender::StateSetAlphaFunc(int, float) {}
void C4JRender::StateSetDepthFunc(int) {}
void C4JRender::StateSetFaceCull(bool) {}
void C4JRender::StateSetFaceCullCW(bool) {}
void C4JRender::StateSetLineWidth(float) {}
void C4JRender::StateSetWriteEnable(bool, bool, bool, bool) {}
void C4JRender::StateSetDepthTestEnable(bool) {}
void C4JRender::StateSetAlphaTestEnable(bool) {}
void C4JRender::StateSetDepthSlopeAndBias(float, float) {}
void C4JRender::StateSetFogEnable(bool) {}
void C4JRender::StateSetFogMode(int) {}
void C4JRender::StateSetFogNearDistance(float) {}
void C4JRender::StateSetFogFarDistance(float) {}
void C4JRender::StateSetFogDensity(float) {}
void C4JRender::StateSetFogColour(float, float, float) {}
void C4JRender::StateSetLightingEnable(bool) {}
void C4JRender::StateSetVertexTextureUV(float, float) {}
void C4JRender::StateSetLightColour(int, float, float, float) {}
void C4JRender::StateSetLightAmbientColour(float, float, float) {}
void C4JRender::StateSetLightDirection(int, float, float, float) {}
void C4JRender::StateSetLightEnable(int, bool) {}
void C4JRender::StateSetViewport(eViewportType) {}
void C4JRender::StateSetEnableViewportClipPlanes(bool) {}
void C4JRender::StateSetTexGenCol(int, float, float, float, float, bool) {}
void C4JRender::StateSetStencil(int, uint8_t, uint8_t, uint8_t) {}
void C4JRender::StateSetForceLOD(int) {}
void C4JRender::BeginEvent(LPCWSTR) {}
void C4JRender::EndEvent() {}
void C4JRender::Suspend() {}
bool C4JRender::Suspended() { return false; }
void C4JRender::Resume() {}

void C_4JProfile::Initialise(DWORD, DWORD, unsigned short, UINT, UINT, DWORD *, int, unsigned int *) {}
void C_4JProfile::SetTrialTextStringTable(CXuiStringTable *, int, int) {}
void C_4JProfile::SetTrialAwardText(eAwardType, int, int) {}
int C_4JProfile::GetLockedProfile() { return g_lockedProfile; }
void C_4JProfile::SetLockedProfile(int iProf) { g_lockedProfile = iProf; }
bool C_4JProfile::IsSignedIn(int iQuadrant) { return InputManager.IsPadConnected(iQuadrant); }
bool C_4JProfile::IsSignedInLive(int iProf) { return IsSignedIn(iProf); }
bool C_4JProfile::IsGuest(int) { return false; }
UINT C_4JProfile::RequestSignInUI(bool, bool, bool, bool, bool, int (*Func)(LPVOID, const bool, const int), LPVOID lpParam, int iQuadrant)
{
	if (Func != nullptr)
	{
		Func(lpParam, true, iQuadrant < 0 ? 0 : iQuadrant);
	}
	return 0;
}
UINT C_4JProfile::DisplayOfflineProfile(int (*Func)(LPVOID, const bool, const int), LPVOID lpParam, int iQuadrant)
{
	if (Func != nullptr)
	{
		Func(lpParam, true, iQuadrant < 0 ? 0 : iQuadrant);
	}
	return 0;
}
UINT C_4JProfile::RequestConvertOfflineToGuestUI(int (*Func)(LPVOID, const bool, const int), LPVOID lpParam, int iQuadrant)
{
	if (Func != nullptr)
	{
		Func(lpParam, true, iQuadrant < 0 ? 0 : iQuadrant);
	}
	return 0;
}
void C_4JProfile::SetPrimaryPlayerChanged(bool) {}
bool C_4JProfile::QuerySigninStatus(void) { return true; }
void C_4JProfile::GetXUID(int iPad, PlayerUID *pXuid, bool)
{
	if (pXuid != nullptr)
	{
		*pXuid = static_cast<PlayerUID>(0x1000 + (iPad < 0 ? 0 : iPad));
	}
}
BOOL C_4JProfile::AreXUIDSEqual(PlayerUID xuid1, PlayerUID xuid2) { return xuid1 == xuid2; }
BOOL C_4JProfile::XUIDIsGuest(PlayerUID) { return false; }
bool C_4JProfile::AllowedToPlayMultiplayer(int) { return true; }
bool C_4JProfile::GetChatAndContentRestrictions(int, bool *, bool *, int *) { return true; }
void C_4JProfile::StartTrialGame() {}
void C_4JProfile::AllowedPlayerCreatedContent(int, bool, BOOL *allAllowed, BOOL *friendsAllowed)
{
	if (allAllowed != nullptr) *allAllowed = true;
	if (friendsAllowed != nullptr) *friendsAllowed = true;
}
BOOL C_4JProfile::CanViewPlayerCreatedContent(int, bool, PPlayerUID, DWORD) { return true; }
void C_4JProfile::ShowProfileCard(int, PlayerUID) {}
bool C_4JProfile::GetProfileAvatar(int, int (*)(LPVOID, PBYTE, DWORD), LPVOID) { return false; }
void C_4JProfile::CancelProfileAvatarRequest() {}

int C_4JProfile::GetPrimaryPad()
{
	return 0;
}

void C_4JProfile::SetPrimaryPad(int) {}
char *C_4JProfile::GetGamertag(int)
{
	static char name[] = "Player";
	return name;
}
wstring C_4JProfile::GetDisplayName(int)
{
	return L"Player";
}
bool C_4JProfile::IsFullVersion() { return true; }
void C_4JProfile::SetSignInChangeCallback(void (*)(LPVOID, bool, unsigned int), LPVOID) {}
void C_4JProfile::SetNotificationsCallback(void (*)(LPVOID, DWORD, unsigned int), LPVOID) {}
bool C_4JProfile::RegionIsNorthAmerica(void) { return false; }
bool C_4JProfile::LocaleIsUSorCanada(void) { return false; }
HRESULT C_4JProfile::GetLiveConnectionStatus() { return S_OK; }
bool C_4JProfile::IsSystemUIDisplayed() { return false; }
void C_4JProfile::SetProfileReadErrorCallback(void (*)(LPVOID), LPVOID) {}
int C_4JProfile::SetDefaultOptionsCallback(int (*)(LPVOID, PROFILESETTINGS *, const int), LPVOID) { return 0; }
int C_4JProfile::SetOldProfileVersionCallback(int (*)(LPVOID, unsigned char *, const unsigned short, const int), LPVOID) { return 0; }
static C_4JProfile::PROFILESETTINGS g_profileSettings[XUSER_MAX_COUNT] = {};
C_4JProfile::PROFILESETTINGS *C_4JProfile::GetDashboardProfileSettings(int iPad) { return &g_profileSettings[iPad < 0 ? 0 : iPad % XUSER_MAX_COUNT]; }
void C_4JProfile::WriteToProfile(int, bool, bool) {}
void C_4JProfile::ForceQueuedProfileWrites(int) {}
void *C_4JProfile::GetGameDefinedProfileData(int) { return nullptr; }
void C_4JProfile::ResetProfileProcessState() {}
void C_4JProfile::Tick(void) {}
void C_4JProfile::RegisterAward(int, int, eAwardType, bool, CXuiStringTable *, int, int, int, char *, unsigned int) {}
int C_4JProfile::GetAwardId(int) { return 0; }
eAwardType C_4JProfile::GetAwardType(int) { return eAwardType_Achievement; }
bool C_4JProfile::CanBeAwarded(int, int) { return false; }
void C_4JProfile::Award(int, int, bool) {}
bool C_4JProfile::IsAwardsFlagSet(int, int) { return false; }
void C_4JProfile::RichPresenceInit(int, int) {}
void C_4JProfile::RegisterRichPresenceContext(int) {}
void C_4JProfile::SetRichPresenceContextValue(int, int, int) {}
void C_4JProfile::SetCurrentGameActivity(int, int, bool) {}
void C_4JProfile::DisplayFullVersionPurchase(bool, int, int) {}
void C_4JProfile::SetUpsellCallback(void (*)(LPVOID, eUpsellType, eUpsellResponse, int), LPVOID) {}
void C_4JProfile::SetDebugFullOverride(bool) {}

C4JStorage::C4JStorage() : m_pStringTable(nullptr) {}
void C4JStorage::Tick(void) {}
C4JStorage::EMessageResult C4JStorage::RequestMessageBox(UINT, UINT, UINT *, UINT, DWORD, int (*)(LPVOID, int, const C4JStorage::EMessageResult), LPVOID, C4JStringTable *, WCHAR *, DWORD) { return C4JStorage::EMessage_ResultAccept; }
C4JStorage::EMessageResult C4JStorage::GetMessageBoxResult() { return C4JStorage::EMessage_ResultAccept; }
bool C4JStorage::SetSaveDevice(int (*)(LPVOID, const bool), LPVOID, bool) { return true; }
void C4JStorage::Init(unsigned int, LPCWSTR, char *, int, int (*)(LPVOID, const ESavingMessage, int), LPVOID, LPCSTR) {}
void C4JStorage::ResetSaveData() {}
void C4JStorage::SetDefaultSaveNameForKeyboardDisplay(LPCWSTR) {}
void C4JStorage::SetSaveTitle(LPCWSTR title)
{
	if (title != nullptr)
	{
		wcsncpy(g_saveTitle, title, 127);
		g_saveTitle[127] = L'\0';
	}
}
bool C4JStorage::GetSaveUniqueNumber(INT *piVal) { if (piVal != nullptr) *piVal = 1; return true; }
bool C4JStorage::GetSaveUniqueFilename(char *pszName) { if (pszName != nullptr) strcpy(pszName, "save"); return true; }
void C4JStorage::SetSaveUniqueFilename(char *) {}
void C4JStorage::SetState(ESaveGameControlState, int (*)(LPVOID, const bool), LPVOID) {}
void C4JStorage::SetSaveDisabled(bool bDisable) { g_saveDisabled = bDisable; }

bool C4JStorage::GetSaveDisabled(void)
{
	return g_saveDisabled;
}

unsigned int C4JStorage::GetSaveSize() { return 0; }
void C4JStorage::GetSaveData(void *, unsigned int *) {}
PVOID C4JStorage::AllocateSaveData(unsigned int uiBytes) { return malloc(uiBytes); }
void C4JStorage::SetSaveImages(PBYTE, DWORD, PBYTE, DWORD, PBYTE, DWORD) {}
C4JStorage::ESaveGameState C4JStorage::SaveSaveData(int (*)(LPVOID, const bool), LPVOID) { return C4JStorage::ESaveGame_Idle; }
void C4JStorage::CopySaveDataToNewSave(PBYTE, DWORD, WCHAR *, int (*)(LPVOID, bool), LPVOID) {}
void C4JStorage::SetSaveDeviceSelected(unsigned int, bool) {}
bool C4JStorage::GetSaveDeviceSelected(unsigned int) { return true; }
C4JStorage::ESaveGameState C4JStorage::DoesSaveExist(bool *pbExists) { if (pbExists != nullptr) *pbExists = false; return C4JStorage::ESaveGame_Idle; }
bool C4JStorage::EnoughSpaceForAMinSaveGame() { return true; }
void C4JStorage::SetSaveMessageVPosition(float) {}
C4JStorage::ESaveGameState C4JStorage::GetSavesInfo(int, int (*Func)(LPVOID, SAVE_DETAILS *, const bool), LPVOID lpParam, char *)
{
	g_dummySaveDetails.iSaveC = 1;
	g_dummySaveDetails.SaveInfoA = &g_dummySaveInfo;
	if (Func != nullptr)
	{
		Func(lpParam, &g_dummySaveDetails, true);
	}
	return C4JStorage::ESaveGame_Idle;
}
PSAVE_DETAILS C4JStorage::ReturnSavesInfo() { return &g_dummySaveDetails; }
void C4JStorage::ClearSavesInfo() {}
C4JStorage::ESaveGameState C4JStorage::LoadSaveDataThumbnail(PSAVE_INFO, int (*)(LPVOID, PBYTE, DWORD), LPVOID) { return C4JStorage::ESaveGame_Idle; }
void C4JStorage::GetSaveCacheFileInfo(DWORD, XCONTENT_DATA &) {}
void C4JStorage::GetSaveCacheFileInfo(DWORD, PBYTE *, DWORD *) {}
C4JStorage::ESaveGameState C4JStorage::LoadSaveData(PSAVE_INFO, int (*Func)(LPVOID, const bool, const bool), LPVOID lpParam)
{
	if (Func != nullptr)
	{
		Func(lpParam, true, false);
	}
	return C4JStorage::ESaveGame_Idle;
}
C4JStorage::ESaveGameState C4JStorage::DeleteSaveData(PSAVE_INFO, int (*Func)(LPVOID, const bool), LPVOID lpParam)
{
	if (Func != nullptr)
	{
		Func(lpParam, true);
	}
	return C4JStorage::ESaveGame_Idle;
}
void C4JStorage::RegisterMarketplaceCountsCallback(int (*)(LPVOID, C4JStorage::DLC_TMS_DETAILS *, int), LPVOID) {}
void C4JStorage::SetDLCPackageRoot(char *) {}
C4JStorage::EDLCStatus C4JStorage::GetDLCOffers(int, int (*)(LPVOID, int, DWORD, int), LPVOID, DWORD) { return C4JStorage::EDLC_Idle; }
DWORD C4JStorage::CancelGetDLCOffers() { return 0; }
void C4JStorage::ClearDLCOffers() {}
XMARKETPLACE_CONTENTOFFER_INFO &C4JStorage::GetOffer(DWORD) { return g_dummyOffer; }
int C4JStorage::GetOfferCount() { return 0; }
DWORD C4JStorage::InstallOffer(int, uint64_t *, int (*Func)(LPVOID, int, int), LPVOID lpParam, bool)
{
	if (Func != nullptr)
	{
		Func(lpParam, 0, 0);
	}
	return 0;
}
DWORD C4JStorage::GetAvailableDLCCount(int) { return 0; }
XCONTENT_DATA &C4JStorage::GetDLC(DWORD)
{
	static XCONTENT_DATA data = {};
	return data;
}
C4JStorage::EDLCStatus C4JStorage::GetInstalledDLC(int, int (*)(LPVOID, int, int), LPVOID) { return C4JStorage::EDLC_Idle; }
DWORD C4JStorage::MountInstalledDLC(int, DWORD, int (*)(LPVOID, int, DWORD, DWORD), LPVOID, LPCSTR) { return 0; }
DWORD C4JStorage::UnmountInstalledDLC(LPCSTR) { return 0; }
void C4JStorage::GetMountedDLCFileList(const char *, std::vector<std::string> &) {}
std::string C4JStorage::GetMountedPath(std::string szMount) { return szMount; }
C4JStorage::ETMSStatus C4JStorage::ReadTMSFile(int, eGlobalStorage, C4JStorage::eTMS_FileType, WCHAR *, BYTE **, DWORD *, int (*)(LPVOID, WCHAR *, int, bool, int), LPVOID, int) { return C4JStorage::ETMSStatus_Idle; }
bool C4JStorage::WriteTMSFile(int, eGlobalStorage, WCHAR *, BYTE *, DWORD) { return true; }
bool C4JStorage::DeleteTMSFile(int, eGlobalStorage, WCHAR *) { return true; }
void C4JStorage::StoreTMSPathName(WCHAR *) {}
C4JStorage::ETMSStatus C4JStorage::TMSPP_ReadFile(int, C4JStorage::eGlobalStorage, C4JStorage::eTMS_FILETYPEVAL, LPCSTR, int (*)(LPVOID, int, int, PTMSPP_FILEDATA, LPCSTR), LPVOID, int) { return C4JStorage::ETMSStatus_Idle; }
unsigned int C4JStorage::CRC(unsigned char *, int) { return 0; }

IggyValuePath *IggyPlayerRootPath(Iggy *) { return nullptr; }
IggyResult IggyPlayerCallMethodRS(Iggy *, IggyDataValue *result, IggyValuePath *, IggyName, S32, IggyDataValue *)
{
	if (result != nullptr)
	{
		result->type = IGGY_DATATYPE_null;
	}
	return IGGY_RESULT_SUCCESS;
}

DWORD XUserGetSigninInfo(DWORD, DWORD, PXUSER_SIGNIN_INFO pSigninInfo)
{
	if (pSigninInfo != nullptr)
	{
		pSigninInfo->xuid = 0;
		pSigninInfo->dwGuestNumber = 0;
	}
	return 0;
}

void XSetThreadProcessor(HANDLE, int)
{
}

DWORD XGetLanguage()
{
	return 1;
}

DWORD XGetLocale()
{
	return 0;
}

HRESULT XMemCompress(void *, void *pDestination, unsigned int *pDestSize, const void *pSource, unsigned int SrcSize)
{
	if (pDestination != nullptr && pSource != nullptr)
	{
		memcpy(pDestination, pSource, SrcSize);
	}
	if (pDestSize != nullptr)
	{
		*pDestSize = SrcSize;
	}
	return S_OK;
}

HRESULT XMemDecompress(void *, void *pDestination, unsigned int *pDestSize, const void *pSource, unsigned int SrcSize)
{
	if (pDestination != nullptr && pSource != nullptr)
	{
		memcpy(pDestination, pSource, SrcSize);
	}
	if (pDestSize != nullptr)
	{
		*pDestSize = SrcSize;
	}
	return S_OK;
}

D3DXVECTOR3::D3DXVECTOR3() {}
D3DXVECTOR3::D3DXVECTOR3(float x, float y, float z) : x(x), y(y), z(z) {}

void SoundEngine::SetStreamingSounds(int, int, int, int, int, int, int)
{
}

extern "C"
{
void IggyFontInstallBitmapUTF8(const IggyBitmapFontProvider *, const char *, S32, U32) {}
void IggyFontRemoveUTF8(const char *, S32, U32) {}
void IggyFontSetIndirectUTF8(const char *, S32, U32, const char *, S32, U32) {}
void IggyFontInstallTruetypeUTF8(const void *, S32, const char *, S32, U32) {}
void IggyFontInstallTruetypeFallbackCodepointUTF8(const char *, S32, U32, S32) {}
void IggySetFontCachingCalculationBuffer(S32, void *, S32) {}

void IggyPlayerDestroy(Iggy *) {}
Iggy *IggyPlayerCreateFromMemory(void const *, U32, IggyPlayerConfig *) { return reinterpret_cast<Iggy *>(0x1); }
IggyProperties *IggyPlayerProperties(Iggy *)
{
	static IggyProperties props = {};
	props.movie_width_in_pixels = 854;
	props.movie_height_in_pixels = 480;
	props.movie_frame_rate_current_in_fps = 60.0f;
	props.movie_frame_rate_from_file_in_fps = 60.0f;
	return &props;
}
void IggyPlayerSetDisplaySize(Iggy *, S32, S32) {}
void IggyPlayerInitializeAndTickRS(Iggy *) {}
void IggyPlayerSetUserdata(Iggy *, void *) {}
void IggyPlayerTickRS(Iggy *) {}
rrbool IggyPlayerReadyToTick(Iggy *) { return true; }
IggyName IggyPlayerCreateFastName(Iggy *, IggyUTF16 const *, S32) { return 0; }
void IggyPlayerDraw(Iggy *) {}
void IggyPlayerDrawTilesStart(Iggy *) {}
void IggyPlayerDrawTile(Iggy *, S32, S32, S32, S32, S32) {}
void IggyPlayerDrawTilesEnd(Iggy *) {}
rrbool IggyPlayerDispatchEventRS(Iggy *, IggyEvent *, IggyEventResult *result)
{
	if (result != nullptr)
	{
		result->new_flags = 0;
		result->focus_change = IGGY_FOCUS_CHANGE_None;
		result->focus_direction = 0;
	}
	return true;
}

rrbool IggyValuePathMakeNameRef(IggyValuePath *result, IggyValuePath *parent, char const *)
{
	if (result != nullptr)
	{
		result->f = parent != nullptr ? parent->f : nullptr;
		result->parent = parent;
		result->name = 0;
		result->ref = nullptr;
		result->index = 0;
		result->type = IGGY_DATATYPE_undefined;
	}
	return true;
}
IggyResult IggyValueGetF64RS(IggyValuePath *, IggyName, char const *, F64 *result)
{
	if (result != nullptr)
	{
		*result = 0.0;
	}
	return IGGY_RESULT_SUCCESS;
}
IggyResult IggyValueGetBooleanRS(IggyValuePath *, IggyName, char const *, rrbool *result)
{
	if (result != nullptr)
	{
		*result = false;
	}
	return IGGY_RESULT_SUCCESS;
}
rrbool IggyValueSetBooleanRS(IggyValuePath *, IggyName, char const *, rrbool)
{
	return true;
}

void IggyMakeEventMouseMove(IggyEvent *event, S32 x, S32 y)
{
	if (event != nullptr)
	{
		event->type = IGGY_EVENTTYPE_MouseMove;
		event->flags = 0;
		event->x = x;
		event->y = y;
	}
}
void IggyMakeEventKey(IggyEvent *event, IggyKeyevent event_type, IggyKeycode keycode, IggyKeyloc keyloc)
{
	if (event != nullptr)
	{
		event->type = event_type;
		event->flags = 0;
		event->keycode = keycode;
		event->keyloc = keyloc;
	}
}
}

void gdraw_D3D11_SetTileOrigin(ID3D11RenderTargetView *, ID3D11DepthStencilView *, ID3D11ShaderResourceView *, int, int)
{
}

void gdraw_D3D11_NoMoreGDrawThisFrame()
{
}

void gdraw_D3D11_BeginCustomDraw_4J(IggyCustomDrawCallbackRegion *, float *)
{
}

void gdraw_D3D11_CalculateCustomDraw_4J(IggyCustomDrawCallbackRegion *, float *)
{
}

void gdraw_D3D11_EndCustomDraw(IggyCustomDrawCallbackRegion *)
{
}

void gdraw_D3D11_WrappedTextureDestroy(GDrawTexture *)
{
}
