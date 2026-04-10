// 3DS Profile Management Implementation
#include "4JLibs/inc/4J_Profile.h"
#include "4JLibs/inc/4J_Storage.h"
#include <3ds.h>
#include <string.h>
#include <vector>

C_4JProfile ProfileManager;

class C_4JProfileImpl
{
public:
	std::vector<C_4JProfile::LocalProfile> profiles;
	uint32_t activeProfileID;
	std::vector<uint32_t> unlockedAchievements;
	bool systemIs3DSXL;
	bool systemIsNew3DS;
	uint32_t systemSerialNumber;

	C_4JProfileImpl() : activeProfileID(0)
	{
		// Detect system information
		systemIs3DSXL = osGetSystemInfo(OS_SYSINFO_MODELID) == OS_SYSINFO_MODELID_3DSXL;
		systemIsNew3DS = osGetSystemInfo(OS_SYSINFO_ARCH) != OS_SYSINFO_ARCH_OLD3DS;
		systemSerialNumber = 0; // Would need to read from system
	}
};

static C_4JProfileImpl g_profileImpl;

void C_4JProfile::Initialise(uint32_t uiProfileValuesC, uint32_t uiProfileSettingsC, uint32_t *pdwProfileSettingsA)
{
	// Initialize profile system
	// Load profiles from storage
	
	// Create default profile if none exist
	if(g_profileImpl.profiles.empty())
	{
		CreateProfile("Player1");
	}
	g_profileImpl.activeProfileID = 0;
}

void C_4JProfile::CreateProfile(const char *szPlayerName)
{
	if(!szPlayerName) return;

	LocalProfile newProfile;
	newProfile.profileID = g_profileImpl.profiles.size();
	strncpy(newProfile.playerName, szPlayerName, sizeof(newProfile.playerName) - 1);
	newProfile.playerName[sizeof(newProfile.playerName) - 1] = 0;
	newProfile.playtime = 0;
	newProfile.settings.iYAxisInversion = 0;
	newProfile.settings.iControllerSensitivity = 5;
	newProfile.settings.iVibration = 1;
	newProfile.settings.bSwapSticks = false;
	newProfile.avatarData = nullptr;
	newProfile.avatarSize = 0;

	g_profileImpl.profiles.push_back(newProfile);

	// Save profile to storage
	// StorageManager.SaveGlobalData(...);
}

void C_4JProfile::DeleteProfile(uint32_t profileID)
{
	if(profileID >= g_profileImpl.profiles.size()) return;

	if(g_profileImpl.profiles[profileID].avatarData)
	{
		free(g_profileImpl.profiles[profileID].avatarData);
	}

	g_profileImpl.profiles.erase(g_profileImpl.profiles.begin() + profileID);

	// Reindex remaining profiles
	for(size_t i = profileID; i < g_profileImpl.profiles.size(); i++)
	{
		g_profileImpl.profiles[i].profileID = i;
	}
}

int C_4JProfile::GetProfileCount()
{
	return g_profileImpl.profiles.size();
}

bool C_4JProfile::GetProfile(uint32_t profileID, LocalProfile *pProfileOut)
{
	if(profileID >= g_profileImpl.profiles.size() || !pProfileOut) return false;

	*pProfileOut = g_profileImpl.profiles[profileID];
	return true;
}

uint32_t C_4JProfile::GetActiveProfileID()
{
	return g_profileImpl.activeProfileID;
}

void C_4JProfile::SetActiveProfile(uint32_t profileID)
{
	if(profileID < g_profileImpl.profiles.size())
	{
		g_profileImpl.activeProfileID = profileID;
	}
}

const char* C_4JProfile::GetProfileName(uint32_t profileID)
{
	if(profileID >= g_profileImpl.profiles.size()) return "Unknown";
	return g_profileImpl.profiles[profileID].playerName;
}

C_4JProfile::PROFILESETTINGS* C_4JProfile::GetProfileSettings(uint32_t profileID)
{
	if(profileID >= g_profileImpl.profiles.size()) return nullptr;
	return &g_profileImpl.profiles[profileID].settings;
}

void C_4JProfile::SaveProfileSettings(uint32_t profileID, const PROFILESETTINGS *pSettings)
{
	if(profileID >= g_profileImpl.profiles.size() || !pSettings) return;

	g_profileImpl.profiles[profileID].settings = *pSettings;

	// Persist to storage
	// StorageManager.SaveGlobalData(...);
}

bool C_4JProfile::IsProfileValid(uint32_t profileID)
{
	return profileID < g_profileImpl.profiles.size();
}

void C_4JProfile::UpdatePlaytime(uint32_t profileID, uint32_t secondsPlayed)
{
	if(profileID >= g_profileImpl.profiles.size()) return;

	g_profileImpl.profiles[profileID].playtime += secondsPlayed;
}

uint32_t C_4JProfile::GetTotalPlaytime(uint32_t profileID)
{
	if(profileID >= g_profileImpl.profiles.size()) return 0;
	return g_profileImpl.profiles[profileID].playtime;
}

void C_4JProfile::ListAchievements(uint32_t profileID, AchievementCallback pCallback, void *lpParam)
{
	if(!pCallback) return;

	// Simple achievement list
	const char *achievements[] = {
		"First Block",
		"Getting Wood",
		"Taking Inventory",
		"Delving Deeper",
		"It Spreads",
		"Monster Hunter",
		"Cow Tipper",
		"When Pigs Fly"
	};

	for(size_t i = 0; i < sizeof(achievements) / sizeof(achievements[0]); i++)
	{
		pCallback(lpParam, i, achievements[i]);
	}
}

bool C_4JProfile::UnlockAchievement(uint32_t profileID, uint32_t achievementID)
{
	if(profileID >= g_profileImpl.profiles.size()) return false;

	// Check if already unlocked
	for(auto id : g_profileImpl.unlockedAchievements)
	{
		if(id == achievementID) return true;
	}

	// Add to unlocked list
	g_profileImpl.unlockedAchievements.push_back(achievementID);
	return true;
}

bool C_4JProfile::IsAchievementUnlocked(uint32_t profileID, uint32_t achievementID)
{
	for(auto id : g_profileImpl.unlockedAchievements)
	{
		if(id == achievementID) return true;
	}
	return false;
}

bool C_4JProfile::SetProfileAvatar(uint32_t profileID, const void *pAvatarData, uint32_t dwAvatarSize)
{
	if(profileID >= g_profileImpl.profiles.size() || !pAvatarData || dwAvatarSize == 0) return false;

	LocalProfile &profile = g_profileImpl.profiles[profileID];

	// Free existing avatar
	if(profile.avatarData)
	{
		free(profile.avatarData);
	}

	// Allocate and copy new avatar
	profile.avatarData = malloc(dwAvatarSize);
	if(!profile.avatarData) return false;

	memcpy(profile.avatarData, pAvatarData, dwAvatarSize);
	profile.avatarSize = dwAvatarSize;

	return true;
}

bool C_4JProfile::GetProfileAvatar(uint32_t profileID, void *pAvatarDataOut, uint32_t dwMaxSize, uint32_t *pdwActualSize)
{
	if(profileID >= g_profileImpl.profiles.size() || !pAvatarDataOut) return false;

	const LocalProfile &profile = g_profileImpl.profiles[profileID];

	if(!profile.avatarData || profile.avatarSize == 0) return false;

	uint32_t copySize = profile.avatarSize;
	if(copySize > dwMaxSize) copySize = dwMaxSize;

	memcpy(pAvatarDataOut, profile.avatarData, copySize);

	if(pdwActualSize)
	{
		*pdwActualSize = copySize;
	}

	return true;
}

void C_4JProfile::DisplaySettingsUI(uint32_t profileID, SettingsCallback pCallback, void *lpParam)
{
	if(!pCallback) return;

	// Call the callback with current settings
	if(IsProfileValid(profileID))
	{
		PROFILESETTINGS settings = g_profileImpl.profiles[profileID].settings;
		pCallback(lpParam, &settings);
	}
}

void C_4JProfile::Tick(void)
{
	// Update profile state if needed
}

bool C_4JProfile::Is3DSXL() const
{
	return g_profileImpl.systemIs3DSXL;
}

bool C_4JProfile::IsNew3DS() const
{
	return g_profileImpl.systemIsNew3DS;
}

const char* C_4JProfile::GetSystemName()
{
	if(g_profileImpl.systemIsNew3DS)
	{
		return g_profileImpl.systemIs3DSXL ? "New 3DS XL" : "New 3DS";
	}
	else
	{
		return g_profileImpl.systemIs3DSXL ? "3DS XL" : "3DS";
	}
}

uint32_t C_4JProfile::GetSystemSerialNumber()
{
	return g_profileImpl.systemSerialNumber;
}
