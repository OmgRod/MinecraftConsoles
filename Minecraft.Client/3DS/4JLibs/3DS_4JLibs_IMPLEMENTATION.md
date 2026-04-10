# 3DS 4JLibs Implementation Guide

## Overview
This document describes the 3DS-specific implementation of the 4JLibs abstraction layer for Minecraft: Pocket Edition on Nintendo 3DS.

## What Was Created

### 1. **4J_Input.h** - Nintendo 3DS Input Handling
**Location:** `Minecraft.Client/3DS/4JLibs/inc/4J_Input.h`

**Key Features:**
- `C_4JInput` class for unified input management
- Support for 3DS hardware:
  - A, B, X, Y buttons
  - D-Pad (up, down, left, right)
  - L and R buttons
  - ZL and ZR buttons (for extended controls)
  - Start and Select buttons
  - Circle Pad (analog stick)
  - C-Stick (New 3DS only)
  - Touch screen input

**Button Mapping:**
- Standard 3DS button constants with `_3DS_BUTTON_*` prefix
- Digital directions from Circle Pad (`_3DS_CPAD_*`)
- Touch screen press detection
- Analog values for Circle Pad and C-Stick

**Implementation (`4J_Input_Impl.cpp`):**
- Uses libctru's `hidKeysHeld()`, `hidCircleRead()`, `hidTouchRead()`
- Joystick sensitivity configuration (0.1x to 2.0x)
- Deadzone threshold support
- Menu display state tracking to prevent input during menus
- Idle time calculation
- Keyboard request callback (simplified for 3DS)
- String verification bypass (no online support)

---

### 2. **4J_Render.h** - 3DS Graphics Rendering
**Location:** `Minecraft.Client/3DS/4JLibs/inc/4J_Render.h`

**Screen Specifications:**
- Top Screen: 320x240 pixels
- Bottom Screen: 320x120 pixels
- Dual-screen 3D rendering support

**Rendering Pipeline:**
- Matrix operations (ModelView, Projection, Texture)
- Vertex formats:
  - `VERTEX_TYPE_SIMPLE` - position + color only
  - `VERTEX_TYPE_UV` - with texture coordinates
  - `VERTEX_TYPE_UV_NORMAL` - full format with normals
- Primitive types: triangles, strips, lines
- Viewport management (fullscreen, split-screen)

**Graphics State Management:**
- Blend modes (SRC_ALPHA, ONE_MINUS_SRC_ALPHA)
- Depth testing and writing
- Alpha testing
- Face culling (CW or CCW)
- Lighting state
- Fog parameters
- Texture coordinate generation

**Implementation (`4J_Render_Impl.cpp`):**
- Matrix stack with push/pop operations
- Identity, translation, rotation, scaling, perspective matrices
- Texture management (create, bind, load, save)
- Clear color and depth buffer operations
- State control for all GPU features
- Support for frame capture and screenshots

---

### 3. **4J_Storage.h** - SDMC File System Storage
**Location:** `Minecraft.Client/3DS/4JLibs/inc/4J_Storage.h`

**Storage Paths:**
- Save data: `/3ds/MinecraftLCE/saves/`
- Global settings: `/3ds/MinecraftLCE/global/`
- Content/DLC: `/3ds/MinecraftLCE/content/`

**Save File Structure:**
- Container-based save system with metadata
- Each save contains:
  - `data.bin` - Main game data
  - `thumb.png` - Thumbnail image
  - Modification timestamp and size info

**API Features:**
- Create/delete/rename save containers
- Load and save game data with metadata
- Global data storage for settings
- File operations (read, write, delete, exists)
- Directory creation
- Thumbnail management
- Async save/load with callbacks (implemented as sync on 3DS)

**Implementation (`4J_Storage_Impl.cpp`):**
- Uses standard POSIX file I/O
- Automatic directory creation
- Safe file operations with error handling
- Metadata tracking (file size, modification time)
- Support for backup and restore operations

---

### 4. **4J_Profile.h** - Local Profile Management
**Location:** `Minecraft.Client/3DS/4JLibs/inc/4J_Profile.h`

**Profile System:**
- Local-only profiles (no online account support)
- Player names and avatar storage
- Per-profile playtime tracking
- Local achievement system

**Profile Settings:**
- Y-axis inversion toggle
- Controller sensitivity (0-10 scale)
- Vibration on/off
- Stick swapping (Southpaw mode)

**System Detection:**
- Identifies 3DS vs 3DS XL
- Detects New 3DS hardware (for C-Stick support)
- System name and serial number retrieval

**Implementation (`4J_Profile_Impl.cpp`):**
- Multiple profile support
- Achievement management (local)
- Avatar customization (raw binary storage)
- Settings callback system
- Persistent profile data
- System information queries via libctru

---

## File Structure Created

```
Minecraft.Client/
└── 3DS/
    └── 4JLibs/
        ├── inc/
        │   ├── 4J_Input.h       (3DS-specific input header)
        │   ├── 4J_Render.h      (3DS-specific render header)
        │   ├── 4J_Storage.h     (3DS-specific storage header)
        │   └── 4J_Profile.h     (3DS-specific profile header)
        ├── 4J_Input_Impl.cpp    (libctru-based implementation)
        ├── 4J_Render_Impl.cpp   (Graphics pipeline impl)
        ├── 4J_Storage_Impl.cpp  (SDMC file I/O impl)
        └── 4J_Profile_Impl.cpp  (Profile management impl)
```

---

## Platform-Specific Notes

### Input System
- **Hardware Support:** Single built-in gamepad (always present)
- **Circle Pad:** Analog stick normalized to ±1.0 range
- **C-Stick:** Only available on New 3DS (code detects and gracefully handles)
- **Touch Screen:** 320x240 touch panel on bottom screen
- **Key Repeat:** Simplified implementation suitable for menu navigation

### Rendering
- **GPU:** PICA200 (3DS GPU)
- **Screens:** Dual independent 4:3 displays with 3D capability
- **Feature Level:** OpenGL ES 1.1 equivalent
- **Resolution:** 320x240 (top), 320x120 (bottom)
- **Color Depth:** 16-bit or 32-bit per pixel
- **Texture Support:** Limited to 1024x1024 maximum
- **Shader Model:** Fixed-function pipeline with programmable vertex/normal generation

### Storage
- **File System:** SDMC (SD Memory Card)
- **Mount Point:** `/` at SDMC root
- **Permissions:** Standard Unix permissions
- **Reliability:** POSIX file operations with error checking
- **Path Encoding:** UTF-8 support via standard C library

### Profiles
- **Account Support:** Local only (no 3DS online account integration)
- **Storage:** Profile data persisted to SDMC
- **Multiplayer:** Single player focus (no network support)
- **Achievements:** Custom local achievement system

---

## Integration with Minecraft PE

### How to Use in Code

**Input Handling:**
```cpp
// Initialize
InputManager.Initialise(1, 0, 0, 0);

// Per-frame
InputManager.Tick();

// Get button state
if(InputManager.ButtonPressed(0, _3DS_BUTTON_A))
{
    // A button was just pressed
}

// Get analog input
float moveX = InputManager.GetJoypadStick_LX(0);
float moveY = InputManager.GetJoypadStick_LY(0);
```

**Graphics:**
```cpp
// Initialize renderer
Renderer.Initialise();

// Per-frame
Renderer.StartFrame();
Renderer.MatrixMode(GL_PROJECTION);
Renderer.MatrixPerspective(45.0f, 4.0f/3.0f, 0.1f, 100.0f);
Renderer.MatrixMode(GL_MODELVIEW);
// ... draw content ...
Renderer.Present();
```

**Save/Load:**
```cpp
// Save game
StorageManager.Initialise();
StorageManager.SaveData("SaveGame_01", worldData, sizeof(worldData), nullptr, 0);

// Load game
uint32_t dataSize;
StorageManager.LoadData("SaveGame_01", worldData, MaxDataSize, &dataSize);
```

**Profiles:**
```cpp
// Initialize profiles
ProfileManager.Initialise();

// Create player profile
ProfileManager.CreateProfile("Player1");

// Get settings
auto settings = ProfileManager.GetProfileSettings(0);
if(settings->iYAxisInversion) { /* invert controls */ }
```

---

## Dependencies

### Required Libraries
1. **libctru** - 3DS system library (input, system info)
2. **citro3d** - 3DS GPU abstraction (optional, using GPU features)
3. **Standard C/C++** - File I/O, memory management

### Compilation
- **Compiler:** arm-none-eabi-g++ (devkitARM)
- **Flags:** C++14 standard
- **Includes:** `-I$(DEVKITPRO)/libctru/include`

---

## Compatibility Notes

### Migration from Windows64
The original Windows64 4JLibs used:
- DirectInput for input
- Direct3D 11 for graphics  
- Xbox LIVE APIs (achievements, friends)
- Windows file system

The 3DS version replaces these with:
- libctru input APIs
- Simple matrix-based rendering
- Local achievement system
- POSIX file I/O

### Breaking Changes
- No network/online features (all callbacks are stubs/local only)
- No gamer profiles or XUIDs
- No DLC or marketplace integration
- No system UI integration (achievements, friends, messaging)
- Single-player focus

---

## Testing Checklist

- [ ] Button inputs received correctly
- [ ] Circle Pad analog values in -1.0 to +1.0 range
- [ ] Touch screen coordinates correct
- [ ] New 3DS C-Stick detection works
- [ ] Save files created in correct directory
- [ ] Save files persist across power cycles
- [ ] Profile settings saved and loaded
- [ ] Matrix transformations calculated correctly
- [ ] Texture operations work on 3DS GPU
- [ ] Frame rate acceptable (30-60 FPS target)

---

## Future Improvements

1. **GPU Integration:** Add citro3d integration for actual GPU rendering
2. **Touch UI:** Implement touch screen keyboards for text input
3. **Stereoscopic 3D:** Support 3D rendering on both screens
4. **Network:** Add simple WiFi save sync (if connected to network)
5. **Storage Encryption:** Encrypt save data for security
6. **Cloud Backup:** Optional cloud storage integration via HomePass

---

## Version Information
- **Created:** 2026
- **Target Platform:** Nintendo 3DS / 3DS XL / New 3DS / New 3DS XL
- **Minecraft PE Version:** Ported from Consoles Edition
- **Development Status:** Alpha (Basic functionality, core systems)

