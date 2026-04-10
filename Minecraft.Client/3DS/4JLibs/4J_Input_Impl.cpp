// 3DS Input Implementation
#include "4JLibs/inc/4J_Input.h"
#include <3ds.h>
#include <cstring>
#include <algorithm>

// Global singleton
C_4JInput InputManager;

// Internal state
static circlePosition g_circlePos = {0, 0};
static circlePosition g_cstickPos = {0, 0};
static touchPosition g_touchPos = {0, 0};
static uint32_t g_keyDownPrev = 0;
static uint32_t g_keyDownCurr = 0;
static bool g_touchDown = false;
static float g_deadzoneThreshold = 20.0f;
static float g_movementRangeMax = 156.0f;
static float g_joystickSensitivity[4] = {1.0f, 1.0f, 1.0f, 1.0f};
static uint32_t g_joypadButtonMap[4] = {0xFF, 0xFF, 0xFF, 0xFF};
static bool g_menuDisplayed[4] = {false, false, false, false};
static float g_idleSeconds[4] = {0.0f, 0.0f, 0.0f, 0.0f};

void C_4JInput::Initialise(int iInputStateC, unsigned char ucMapC, unsigned char ucActionC, unsigned char ucMenuActionC)
{
	// Initialize joystick maps with default values
	for(int i = 0; i < 4; i++)
	{
		g_joypadButtonMap[i] = 0xFF;
		g_joystickSensitivity[i] = 1.0f;
		g_idleSeconds[i] = 0.0f;
		g_menuDisplayed[i] = false;
	}
}

void C_4JInput::Tick(void)
{
	// Update button states
	g_keyDownPrev = g_keyDownCurr;
	g_keyDownCurr = hidKeysHeld();

	// Update Circle Pad
	hidCircleRead(&g_circlePos);

	// Update C-Stick (New 3DS only)
	if(osGetSystemInfo(OS_SYSINFO_ARCH) != OS_SYSINFO_ARCH_OLD3DS)
	{
		irrstReadRawImage(&g_cstickPos);
	}

	// Update touch screen
	hidTouchRead(&g_touchPos);
	bool touchNow = hidIsTouchPressed();

	if(!g_touchDown && touchNow)
	{
		g_touchDown = true;
	}
	else if(g_touchDown && !touchNow)
	{
		g_touchDown = false;
	}

	// Update idle times
	if(g_keyDownCurr == 0 && g_circlePos.dx == 0 && g_circlePos.dy == 0 && !g_touchDown)
	{
		for(int i = 0; i < 4; i++) g_idleSeconds[i] += 0.016f; // ~60 FPS
	}
	else
	{
		for(int i = 0; i < 4; i++) g_idleSeconds[i] = 0.0f;
	}
}

void C_4JInput::SetDeadzoneAndMovementRange(unsigned int uiDeadzone, unsigned int uiMovementRangeMax)
{
	g_deadzoneThreshold = static_cast<float>(uiDeadzone);
	g_movementRangeMax = static_cast<float>(uiMovementRangeMax);
}

void C_4JInput::SetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction, unsigned int uiActionVal)
{
	if(ucMap < 4)
	{
		g_joypadButtonMap[ucMap] = uiActionVal;
	}
}

unsigned int C_4JInput::GetGameJoypadMaps(unsigned char ucMap, unsigned char ucAction)
{
	if(ucMap < 4)
	{
		return g_joypadButtonMap[ucMap];
	}
	return 0;
}

void C_4JInput::SetJoypadMapVal(int iPad, unsigned char ucMap)
{
	if(iPad < 4)
	{
		g_joypadButtonMap[iPad] = ucMap;
	}
}

unsigned char C_4JInput::GetJoypadMapVal(int iPad)
{
	if(iPad < 4)
	{
		return g_joypadButtonMap[iPad];
	}
	return 0xFF;
}

void C_4JInput::SetJoypadSensitivity(int iPad, float fSensitivity)
{
	if(iPad < 4)
	{
		g_joystickSensitivity[iPad] = std::max(0.1f, std::min(2.0f, fSensitivity));
	}
}

unsigned int C_4JInput::GetValue(int iPad, unsigned char ucAction, bool bRepeat)
{
	uint32_t result = 0;

	// Map 3DS buttons to game actions
	if(g_keyDownCurr & KEY_A) result |= _3DS_BUTTON_A;
	if(g_keyDownCurr & KEY_B) result |= _3DS_BUTTON_B;
	if(g_keyDownCurr & KEY_X) result |= _3DS_BUTTON_X;
	if(g_keyDownCurr & KEY_Y) result |= _3DS_BUTTON_Y;
	if(g_keyDownCurr & KEY_START) result |= _3DS_BUTTON_START;
	if(g_keyDownCurr & KEY_SELECT) result |= _3DS_BUTTON_SELECT;
	if(g_keyDownCurr & KEY_L) result |= _3DS_BUTTON_L;
	if(g_keyDownCurr & KEY_R) result |= _3DS_BUTTON_R;
	if(g_keyDownCurr & KEY_ZL) result |= _3DS_BUTTON_ZL;
	if(g_keyDownCurr & KEY_ZR) result |= _3DS_BUTTON_ZR;
	if(g_keyDownCurr & KEY_DUP) result |= _3DS_DPAD_UP;
	if(g_keyDownCurr & KEY_DDOWN) result |= _3DS_DPAD_DOWN;
	if(g_keyDownCurr & KEY_DLEFT) result |= _3DS_DPAD_LEFT;
	if(g_keyDownCurr & KEY_DRIGHT) result |= _3DS_DPAD_RIGHT;

	// Circle Pad as digital directions
	if(g_circlePos.dx > g_deadzoneThreshold) result |= _3DS_CPAD_RIGHT;
	if(g_circlePos.dx < -g_deadzoneThreshold) result |= _3DS_CPAD_LEFT;
	if(g_circlePos.dy > g_deadzoneThreshold) result |= _3DS_CPAD_UP;
	if(g_circlePos.dy < -g_deadzoneThreshold) result |= _3DS_CPAD_DOWN;

	// Touch screen
	if(g_touchDown) result |= _3DS_TOUCH_PRESSED;

	return result;
}

bool C_4JInput::ButtonPressed(int iPad, unsigned char ucAction)
{
	uint32_t currState = GetValue(iPad, ucAction);
	uint32_t prevState = g_keyDownPrev;

	if(ucAction == 255) return (currState & ~prevState) != 0;
	return ((currState ^ prevState) & currState) != 0;
}

bool C_4JInput::ButtonReleased(int iPad, unsigned char ucAction)
{
	uint32_t currState = GetValue(iPad, ucAction);
	uint32_t prevState = g_keyDownPrev;

	if(ucAction == 255) return (prevState & ~currState) != 0;
	return ((currState ^ prevState) & prevState) != 0;
}

bool C_4JInput::ButtonDown(int iPad, unsigned char ucAction)
{
	uint32_t currState = GetValue(iPad, ucAction);
	if(ucAction == 255) return currState != 0;
	return (currState & (1 << ucAction)) != 0;
}

float C_4JInput::GetJoypadStick_LX(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0.0f;
	return static_cast<float>(g_circlePos.dx) / g_movementRangeMax * g_joystickSensitivity[iPad];
}

float C_4JInput::GetJoypadStick_LY(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0.0f;
	return static_cast<float>(g_circlePos.dy) / g_movementRangeMax * g_joystickSensitivity[iPad];
}

float C_4JInput::GetJoypadStick_RX(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0.0f;
	// C-Stick if available, otherwise return 0
	if(osGetSystemInfo(OS_SYSINFO_ARCH) != OS_SYSINFO_ARCH_OLD3DS)
	{
		return static_cast<float>(g_cstickPos.dx) / g_movementRangeMax * g_joystickSensitivity[iPad];
	}
	return 0.0f;
}

float C_4JInput::GetJoypadStick_RY(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0.0f;
	if(osGetSystemInfo(OS_SYSINFO_ARCH) != OS_SYSINFO_ARCH_OLD3DS)
	{
		return static_cast<float>(g_cstickPos.dy) / g_movementRangeMax * g_joystickSensitivity[iPad];
	}
	return 0.0f;
}

unsigned char C_4JInput::GetJoypadLTrigger(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0;
	// Map L button to trigger value (0-255)
	return (g_keyDownCurr & KEY_L) ? 255 : 0;
}

unsigned char C_4JInput::GetJoypadRTrigger(int iPad, bool bCheckMenuDisplay)
{
	if(bCheckMenuDisplay && g_menuDisplayed[iPad]) return 0;
	// Map R button to trigger value (0-255)
	return (g_keyDownCurr & KEY_R) ? 255 : 0;
}

void C_4JInput::SetJoypadStickAxisMap(int iPad, unsigned int uiFrom, unsigned int uiTo)
{
	// Store axis mapping for remapping (Southpaw, etc)
	// This is a simplified implementation
}

void C_4JInput::SetJoypadStickTriggerMap(int iPad, unsigned int uiFrom, unsigned int uiTo)
{
	// Store trigger mapping
}

void C_4JInput::SetKeyRepeatRate(float fRepeatDelaySecs, float fRepeatRateSecs)
{
	// Key repeat configuration (simplified for 3DS)
}

void C_4JInput::SetDebugSequence(const char *chSequenceA, int (*Func)(void*), void* lpParam)
{
	// Debug sequence handler (not used on 3DS)
}

float C_4JInput::GetIdleSeconds(int iPad)
{
	if(iPad < 4) return g_idleSeconds[iPad];
	return 0.0f;
}

bool C_4JInput::IsPadConnected(int iPad)
{
	// 3DS always has one built-in pad
	return iPad == 0;
}

bool C_4JInput::IsTouchPressed()
{
	return g_touchDown;
}

int C_4JInput::GetTouchX()
{
	return g_touchPos.px;
}

int C_4JInput::GetTouchY()
{
	return g_touchPos.py;
}

void C_4JInput::SetMenuDisplayed(int iPad, bool bVal)
{
	if(iPad < 4)
	{
		g_menuDisplayed[iPad] = bVal;
	}
}

C_4JInput::EKeyboardResult C_4JInput::RequestKeyboard(const char16_t* Title, const char16_t* Text, uint32_t dwPad, uint32_t uiMaxChars,
                                                     int (*Func)(void*, const bool), void* lpParam, C_4JInput::EKeyboardMode eMode)
{
	// 3DS software keyboard implementation
	// This is simplified - you would need to integrate with libctru's SwkbdState
	return EKeyboard_ResultAccept;
}

void C_4JInput::GetText(uint16_t *UTF16String)
{
	// Retrieve text from keyboard (placeholder)
	if(UTF16String) UTF16String[0] = 0;
}

bool C_4JInput::VerifyStrings(wchar_t **pwStringA, int iStringC, int (*Func)(void*, void*), void* lpParam)
{
	// String verification not needed on 3DS (no online)
	return true;
}

void C_4JInput::CancelQueuedVerifyStrings(int (*Func)(void*, void*), void* lpParam)
{
	// Placeholder
}

void C_4JInput::CancelAllVerifyInProgress(void)
{
	// Placeholder
}
