#pragma once

// 3DS stub for DynamicConfigurations
// 3DS doesn't support Sentient dynamic configurations like other consoles

#define DYNAMIC_CONFIG_TRIAL_ID 0
#define DYNAMIC_CONFIG_TRIAL_VERSION 1
#define DYNAMIC_CONFIG_DEFAULT_TRIAL_TIME 2400 // 40 mins

class MinecraftDynamicConfigurations
{
public:
	static void Tick() {}
	
	// Return trial time in seconds (40 minutes for 3DS)
	static DWORD GetTrialTime() { return DYNAMIC_CONFIG_DEFAULT_TRIAL_TIME; }

private:
	MinecraftDynamicConfigurations() {}  // Prevent instantiation
};
