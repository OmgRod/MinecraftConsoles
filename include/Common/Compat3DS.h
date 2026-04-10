#pragma once

#include <cstdint>
#include <cstring>
#include <cwchar>
#include <cstddef>
#include <chrono>
#include <thread>
#include <cstdio>
#include <ctime>
#include <cstdarg>
#include <cstdlib>
#include <climits>
#include <cfloat>

typedef uint8_t BYTE;
typedef BYTE* PBYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint32_t UINT;
typedef int32_t HRESULT;
typedef int BOOL;
typedef bool boolean;
typedef float FLOAT;
typedef float F32;
typedef char CHAR;
typedef short SHORT;
typedef int INT;
typedef long LONG;
typedef unsigned long ULONG;
typedef int32_t __int32;
typedef void VOID;
typedef void* LPVOID;
typedef void* PVOID;
typedef const void* LPCVOID;
typedef wchar_t WCHAR;
typedef WCHAR* LPWSTR;
typedef const WCHAR* LPCWSTR;
typedef const char* LPCSTR;
typedef uintptr_t ULONG_PTR;
typedef unsigned long long ULONGLONG;
typedef long long LONGLONG;
typedef size_t SIZE_T;
typedef uint64_t XUID;
typedef uint64_t XNKID;

typedef XUID PlayerUID;
typedef XNKID SessionID;
typedef XUID GameSessionUID;

typedef struct _XPARTY_USER_LIST
{
  DWORD dwUserCount;
} XPARTY_USER_LIST;

#ifndef XPARTY_E_NOT_IN_PARTY
#define XPARTY_E_NOT_IN_PARTY static_cast<HRESULT>(0x80070057)
#endif

inline HRESULT XPartyGetUserList(XPARTY_USER_LIST* list)
{
  if (list != nullptr)
  {
    list->dwUserCount = 0;
  }
  return XPARTY_E_NOT_IN_PARTY;
}

#ifndef _SOCIAL_MANAGER_H
#define _SOCIAL_MANAGER_H
class CSocialManager
{
public:
  static CSocialManager* Instance()
  {
    static CSocialManager instance;
    return &instance;
  }

  bool IsTitleAllowedToPostImages() const { return false; }
  bool AreAllUsersAllowedToPostImages() const { return false; }
};
#endif

#ifndef CONTEXT_PRESENCE_MENUS
#define CONTEXT_PRESENCE_MENUS                    1
#endif

typedef void* HANDLE;

// 3DS Rich Presence stubs (not applicable to 3DS hardware)
#define CONTEXT_GAME_STATE                          0
#define CONTEXT_GAME_STATE_BLANK                    0
#define CONTEXT_GAME_STATE_RIDING_PIG               1
#define CONTEXT_GAME_STATE_RIDING_MINECART          2
#define CONTEXT_GAME_STATE_BOATING                  3
#define CONTEXT_GAME_STATE_FISHING                  4
#define CONTEXT_GAME_STATE_CRAFTING                 5
#define CONTEXT_GAME_STATE_FORGING                  6
#define CONTEXT_GAME_STATE_NETHER                   7
#define CONTEXT_GAME_STATE_CD                       8
#define CONTEXT_GAME_STATE_MAP                      9
#define CONTEXT_GAME_STATE_ENCHANTING               5
#define CONTEXT_GAME_STATE_BREWING                  5
#define CONTEXT_GAME_STATE_ANVIL                    6
#define CONTEXT_GAME_STATE_TRADING                  0
#define CONTEXT_PRESENCE_IDLE                       0
#define CONTEXT_PRESENCE                            1
#define CONTEXT_PRESENCE_MULTIPLAYER                2
#define CONTEXT_PRESENCE_MULTIPLAYEROFFLINE         3
#define CONTEXT_PRESENCE_MULTIPLAYER_1P             4
#define CONTEXT_PRESENCE_MULTIPLAYER_1POFFLINE      5
typedef LONG* PLONG;
typedef DWORD* LPDWORD;
typedef void* XMEMCOMPRESSION_CONTEXT;
typedef void* XMEMDECOMPRESSION_CONTEXT;

struct LARGE_INTEGER {
  union {
    struct {
      DWORD LowPart;
      LONG HighPart;
    };
    long long QuadPart;
  };
};

struct FILETIME {
  DWORD dwLowDateTime;
  DWORD dwHighDateTime;
};

struct SYSTEMTIME {
  WORD wYear;
  WORD wMonth;
  WORD wDayOfWeek;
  WORD wDay;
  WORD wHour;
  WORD wMinute;
  WORD wSecond;
  WORD wMilliseconds;
};
typedef SYSTEMTIME* LPSYSTEMTIME;

struct WIN32_FIND_DATA {
  DWORD dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD nFileSizeHigh;
  DWORD nFileSizeLow;
  CHAR cFileName[260];
};

struct WIN32_FILE_ATTRIBUTE_DATA {
  DWORD dwFileAttributes;
  FILETIME ftCreationTime;
  FILETIME ftLastAccessTime;
  FILETIME ftLastWriteTime;
  DWORD nFileSizeHigh;
  DWORD nFileSizeLow;
};

struct MEMORYSTATUS {
  DWORD dwLength;
  DWORD dwMemoryLoad;
  SIZE_T dwTotalPhys;
  SIZE_T dwAvailPhys;
  SIZE_T dwTotalPageFile;
  SIZE_T dwAvailPageFile;
  SIZE_T dwTotalVirtual;
  SIZE_T dwAvailVirtual;
};

struct D3D11_RECT {
  long left;
  long top;
  long right;
  long bottom;
};

struct D3D11_VIEWPORT {
  float TopLeftX;
  float TopLeftY;
  float Width;
  float Height;
  float MinDepth;
  float MaxDepth;
};

struct ID3D11Device;
struct ID3D11DeviceContext;
struct ID3D11RenderTargetView;
struct ID3D11DepthStencilView;
struct IDXGISwapChain;
struct ID3D11Buffer;
struct ID3D11ShaderResourceView;

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef WINAPI
#define WINAPI
#endif

inline void XShowAchievementsUI(int) {}

inline VOID GetLocalTime(LPSYSTEMTIME lpSystemTime)
{
  if (lpSystemTime == nullptr) return;
  const std::time_t now = std::time(nullptr);
  std::tm tmNow = {};
  std::tm* local = std::localtime(&now);
  if (local != nullptr)
  {
    tmNow = *local;
  }

  lpSystemTime->wYear = static_cast<WORD>(tmNow.tm_year + 1900);
  lpSystemTime->wMonth = static_cast<WORD>(tmNow.tm_mon + 1);
  lpSystemTime->wDay = static_cast<WORD>(tmNow.tm_mday);
  lpSystemTime->wDayOfWeek = static_cast<WORD>(tmNow.tm_wday);
  lpSystemTime->wHour = static_cast<WORD>(tmNow.tm_hour);
  lpSystemTime->wMinute = static_cast<WORD>(tmNow.tm_min);
  lpSystemTime->wSecond = static_cast<WORD>(tmNow.tm_sec);
  lpSystemTime->wMilliseconds = 0;
}

#ifndef __cdecl
#define __cdecl
#endif

#ifndef __stdcall
#define __stdcall
#endif

#ifndef CDECL
#define CDECL
#endif

#ifndef CONST
#define CONST const
#endif

#ifndef WIN32
#define WIN32 1
#endif

#ifndef _WIN32
#define _WIN32 1
#endif

#ifndef NN_PLATFORM_CTR
#define NN_PLATFORM_CTR 1
#endif

#ifndef __int64
#define __int64 long long
#endif

#ifndef RADLINK
#define RADLINK
#endif

#ifndef D3D11_BLEND_SRC_ALPHA
#define D3D11_BLEND_SRC_ALPHA 1
#define D3D11_BLEND_INV_SRC_ALPHA 2
#define D3D11_BLEND_ONE 3
#define D3D11_BLEND_ZERO 4
#define D3D11_BLEND_DEST_ALPHA 5
#define D3D11_BLEND_SRC_COLOR 6
#define D3D11_BLEND_DEST_COLOR 7
#define D3D11_BLEND_INV_DEST_COLOR 8
#define D3D11_BLEND_INV_SRC_COLOR 9
#define D3D11_BLEND_BLEND_FACTOR 10
#define D3D11_BLEND_INV_BLEND_FACTOR 11
#define D3D11_COMPARISON_GREATER 12
#define D3D11_COMPARISON_EQUAL 13
#define D3D11_COMPARISON_LESS_EQUAL 14
#define D3D11_COMPARISON_GREATER_EQUAL 15
#define D3D11_COMPARISON_ALWAYS 16
#endif

#ifndef FILE_BEGIN
#define FILE_BEGIN 0
#define FILE_CURRENT 1
#define FILE_END 2
#endif

#ifndef FILE_ATTRIBUTE_DIRECTORY
#define FILE_ATTRIBUTE_DIRECTORY 0x00000010
#define FILE_ATTRIBUTE_NORMAL 0x00000080
#define INVALID_FILE_ATTRIBUTES 0xFFFFFFFF
#define FILE_FLAG_RANDOM_ACCESS 0x10000000
#endif

#ifndef GENERIC_WRITE
#define GENERIC_WRITE 0x40000000
#endif

#ifndef GENERIC_READ
#define GENERIC_READ 0x80000000
#endif

#ifndef OPEN_ALWAYS
#define OPEN_ALWAYS 4
#endif

#ifndef OPEN_EXISTING
#define OPEN_EXISTING 3
#endif

#ifndef FILE_FLAG_SEQUENTIAL_SCAN
#define FILE_FLAG_SEQUENTIAL_SCAN 0x08000000
#endif

#ifndef MAX_PATH
#define MAX_PATH 260
#endif

#ifndef WAIT_OBJECT_0
#define WAIT_OBJECT_0 0
#define WAIT_TIMEOUT 258
#define WAIT_FAILED 0xFFFFFFFF
#endif

#ifndef INFINITE
#define INFINITE 0xFFFFFFFF
#endif

#ifndef STILL_ACTIVE
#define STILL_ACTIVE 259
#endif

#ifndef S_OK
#define S_OK ((HRESULT)0)
#define E_FAIL ((HRESULT)0x80004005L)
#endif

#ifndef ERROR_SUCCESS
#define ERROR_SUCCESS 0L
#endif

#ifndef ERROR_IO_PENDING
#define ERROR_IO_PENDING 997L
#endif

#ifndef ERROR_CANCELLED
#define ERROR_CANCELLED 1223L
#endif

#ifndef FAILED
#define FAILED(hr) (((HRESULT)(hr)) < 0)
#endif

#ifndef SUCCEEDED
#define SUCCEEDED(hr) (((HRESULT)(hr)) >= 0)
#endif

#ifndef _TRUNCATE
#define _TRUNCATE static_cast<size_t>(-1)
#endif

#ifndef CREATE_SUSPENDED
#define CREATE_SUSPENDED 0x00000004
#endif

#ifndef THREAD_PRIORITY_HIGHEST
#define THREAD_PRIORITY_HIGHEST 2
#endif

#ifndef MEM_RESERVE
#define MEM_RESERVE 0x00002000
#define MEM_COMMIT 0x00001000
#define MEM_DECOMMIT 0x00004000
#define MEM_LARGE_PAGES 0x20000000
#define PAGE_READWRITE 0x00000004
#endif

#ifndef MAXULONG_PTR
#define MAXULONG_PTR static_cast<ULONG_PTR>(~static_cast<ULONG_PTR>(0))
#endif

#ifndef INVALID_HANDLE_VALUE
#define INVALID_HANDLE_VALUE reinterpret_cast<HANDLE>(static_cast<intptr_t>(-1))
#endif

#ifndef GetFileExInfoStandard
#define GetFileExInfoStandard 0
#endif

#ifndef ZeroMemory
#define ZeroMemory(Destination, Length) std::memset((Destination), 0, (Length))
#endif

struct CRITICAL_SECTION {
  int unused;
};

inline void InitializeCriticalSection(CRITICAL_SECTION*) {}
inline void DeleteCriticalSection(CRITICAL_SECTION*) {}
inline BOOL InitializeCriticalSectionAndSpinCount(CRITICAL_SECTION*, DWORD) { return TRUE; }
inline void EnterCriticalSection(CRITICAL_SECTION*) {}
inline void LeaveCriticalSection(CRITICAL_SECTION*) {}
inline BOOL TryEnterCriticalSection(CRITICAL_SECTION*) { return TRUE; }

inline void __debugbreak() {}

inline int wcscpy_s(wchar_t* dst, size_t dstSize, const wchar_t* src) {
  if (dst == nullptr || src == nullptr || dstSize == 0) {
    return 1;
  }
  std::wcsncpy(dst, src, dstSize - 1);
  dst[dstSize - 1] = L'\0';
  return 0;
}

inline int strcpy_s(char* dst, size_t dstSize, const char* src) {
  if (dst == nullptr || src == nullptr || dstSize == 0) {
    return 1;
  }
  std::strncpy(dst, src, dstSize - 1);
  dst[dstSize - 1] = '\0';
  return 0;
}

inline int sprintf_s(char* dst, size_t dstSize, const char* fmt, ...) {
  if (dst == nullptr || dstSize == 0 || fmt == nullptr) {
    return -1;
  }
  va_list args;
  va_start(args, fmt);
  const int written = std::vsnprintf(dst, dstSize, fmt, args);
  va_end(args);
  if (written < 0) {
    dst[0] = '\0';
  } else {
    dst[dstSize - 1] = '\0';
  }
  return written;
}

inline int swprintf_s(wchar_t* dst, size_t dstSize, const wchar_t* fmt, ...) {
  if (dst == nullptr || dstSize == 0 || fmt == nullptr) {
    return -1;
  }
  va_list args;
  va_start(args, fmt);
  const int written = std::vswprintf(dst, dstSize, fmt, args);
  va_end(args);
  if (written < 0) {
    dst[0] = L'\0';
  } else {
    dst[dstSize - 1] = L'\0';
  }
  return written;
}

template <size_t N>
inline int _vsnprintf_s(char (&dst)[N], size_t, const char* fmt, va_list args) {
  if (fmt == nullptr) {
    dst[0] = '\0';
    return -1;
  }
  const int written = std::vsnprintf(dst, N, fmt, args);
  dst[N - 1] = '\0';
  return written;
}

inline int _itoa_s(int value, char* buffer, size_t bufferSize, int radix) {
  if (buffer == nullptr || bufferSize == 0 || radix < 2 || radix > 36) {
    return 1;
  }
  if (radix == 10) {
    std::snprintf(buffer, bufferSize, "%d", value);
  } else if (radix == 16) {
    std::snprintf(buffer, bufferSize, "%x", value);
  } else {
    // Fallback for less common radices.
    char tmp[65];
    int i = 0;
    bool negative = (value < 0 && radix == 10);
    unsigned int u = negative ? static_cast<unsigned int>(-value) : static_cast<unsigned int>(value);
    do {
      const unsigned int digit = u % static_cast<unsigned int>(radix);
      tmp[i++] = static_cast<char>(digit < 10 ? '0' + digit : 'a' + (digit - 10));
      u /= static_cast<unsigned int>(radix);
    } while (u != 0 && i < 64);
    size_t out = 0;
    if (negative && out + 1 < bufferSize) {
      buffer[out++] = '-';
    }
    while (i > 0 && out + 1 < bufferSize) {
      buffer[out++] = tmp[--i];
    }
    buffer[out] = '\0';
    return 0;
  }
  buffer[bufferSize - 1] = '\0';
  return 0;
}

inline int _i64toa_s(long long value, char* buffer, size_t bufferSize, int radix) {
  if (buffer == nullptr || bufferSize == 0 || radix < 2 || radix > 36) {
    return 1;
  }
  if (radix == 10) {
    std::snprintf(buffer, bufferSize, "%lld", value);
  } else if (radix == 16) {
    std::snprintf(buffer, bufferSize, "%llx", value);
  } else {
    char tmp[65];
    int i = 0;
    bool negative = (value < 0 && radix == 10);
    unsigned long long u = negative ? static_cast<unsigned long long>(-value) : static_cast<unsigned long long>(value);
    do {
      const unsigned long long digit = u % static_cast<unsigned long long>(radix);
      tmp[i++] = static_cast<char>(digit < 10 ? '0' + digit : 'a' + (digit - 10));
      u /= static_cast<unsigned long long>(radix);
    } while (u != 0 && i < 64);
    size_t out = 0;
    if (negative && out + 1 < bufferSize) {
      buffer[out++] = '-';
    }
    while (i > 0 && out + 1 < bufferSize) {
      buffer[out++] = tmp[--i];
    }
    buffer[out] = '\0';
    return 0;
  }
  buffer[bufferSize - 1] = '\0';
  return 0;
}

inline wchar_t* _itow(int value, wchar_t* buffer, int radix) {
  if (buffer == nullptr || radix < 2 || radix > 36) {
    return buffer;
  }

  wchar_t tmp[65];
  int i = 0;
  bool negative = (value < 0 && radix == 10);
  unsigned int u = negative ? static_cast<unsigned int>(-value) : static_cast<unsigned int>(value);

  do {
    const unsigned int digit = u % static_cast<unsigned int>(radix);
    tmp[i++] = static_cast<wchar_t>(digit < 10 ? L'0' + digit : L'a' + (digit - 10));
    u /= static_cast<unsigned int>(radix);
  } while (u != 0);

  if (negative) {
    tmp[i++] = L'-';
  }

  int j = 0;
  while (i > 0) {
    buffer[j++] = tmp[--i];
  }
  buffer[j] = L'\0';
  return buffer;
}

typedef DWORD (WINAPI *LPTHREAD_START_ROUTINE)(LPVOID);

inline HANDLE CreateThread(LPVOID, SIZE_T, LPTHREAD_START_ROUTINE, LPVOID, DWORD, LPDWORD threadId) {
  if (threadId != nullptr) {
    *threadId = 1;
  }
  return reinterpret_cast<HANDLE>(1);
}

inline DWORD ResumeThread(HANDLE) { return 0; }
inline BOOL SetThreadPriority(HANDLE, int) { return TRUE; }
inline DWORD WaitForSingleObject(HANDLE, DWORD) { return WAIT_OBJECT_0; }

inline DWORD WaitForMultipleObjects(DWORD, const HANDLE*, BOOL, DWORD) {
  return WAIT_OBJECT_0;
}

inline BOOL GetExitCodeThread(HANDLE, LPDWORD exitCode) {
  if (exitCode != nullptr) {
    *exitCode = STILL_ACTIVE;
  }
  return TRUE;
}

inline DWORD GetCurrentThreadId() { return 1; }
inline HANDLE GetCurrentThread() { return reinterpret_cast<HANDLE>(1); }

inline DWORD TlsAlloc() { return 0; }

inline LPVOID TlsGetValue(DWORD) {
  static thread_local LPVOID tlsValue = nullptr;
  return tlsValue;
}

inline BOOL TlsSetValue(DWORD, LPVOID value) {
  static thread_local LPVOID tlsValue = nullptr;
  tlsValue = value;
  return TRUE;
}

inline void Sleep(DWORD millisecs) {
  std::this_thread::sleep_for(std::chrono::milliseconds(millisecs));
}

inline HANDLE CreateEvent(LPVOID, BOOL, BOOL, LPCWSTR) { return reinterpret_cast<HANDLE>(1); }
inline BOOL CloseHandle(HANDLE) { return TRUE; }
inline BOOL SetEvent(HANDLE) { return TRUE; }
inline BOOL ResetEvent(HANDLE) { return TRUE; }

inline void* VirtualAlloc(void*, SIZE_T size, DWORD, DWORD) {
  return std::malloc(size);
}

inline BOOL VirtualFree(void* ptr, SIZE_T, DWORD) {
  std::free(ptr);
  return TRUE;
}

inline DWORD GetLastError() { return 0; }

inline VOID OutputDebugStringA(LPCSTR) {}
inline VOID OutputDebugStringW(LPCWSTR) {}

inline BOOL DeleteFile(LPCSTR path) {
  return (path != nullptr && std::remove(path) == 0) ? TRUE : FALSE;
}

inline BOOL MoveFile(LPCSTR from, LPCSTR to) {
  if (from == nullptr || to == nullptr) {
    return FALSE;
  }
  return (std::rename(from, to) == 0) ? TRUE : FALSE;
}

inline BOOL CreateDirectory(LPCSTR, LPVOID) { return TRUE; }
inline DWORD GetFileAttributes(LPCSTR) { return INVALID_FILE_ATTRIBUTES; }

inline BOOL GetFileAttributesEx(LPCSTR, int, LPVOID attrs) {
  if (attrs == nullptr) {
    return FALSE;
  }
  auto* data = static_cast<WIN32_FILE_ATTRIBUTE_DATA*>(attrs);
  data->dwFileAttributes = 0;
  data->nFileSizeHigh = 0;
  data->nFileSizeLow = 0;
  return FALSE;
}

inline DWORD GetFileSize(HANDLE, LPDWORD highSize) {
  if (highSize != nullptr) {
    *highSize = 0;
  }
  return 0;
}

inline VOID GlobalMemoryStatus(MEMORYSTATUS* status) {
  if (status == nullptr) {
    return;
  }
  status->dwLength = sizeof(MEMORYSTATUS);
  status->dwMemoryLoad = 50;
  status->dwTotalPhys = 64u * 1024u * 1024u;
  status->dwAvailPhys = 32u * 1024u * 1024u;
  status->dwTotalPageFile = status->dwTotalPhys;
  status->dwAvailPageFile = status->dwAvailPhys;
  status->dwTotalVirtual = 128u * 1024u * 1024u;
  status->dwAvailVirtual = 64u * 1024u * 1024u;
}

inline HANDLE FindFirstFile(LPCSTR, WIN32_FIND_DATA*) { return INVALID_HANDLE_VALUE; }
inline BOOL FindNextFile(HANDLE, WIN32_FIND_DATA*) { return FALSE; }
inline BOOL FindClose(HANDLE) { return TRUE; }

inline HANDLE CreateFile(LPCSTR, DWORD, DWORD, LPVOID, DWORD, DWORD, HANDLE) {
  return reinterpret_cast<HANDLE>(1);
}

inline BOOL WriteFile(HANDLE, LPCVOID, DWORD toWrite, LPDWORD written, LPVOID) {
  if (written != nullptr) {
    *written = toWrite;
  }
  return TRUE;
}

inline BOOL ReadFile(HANDLE, LPVOID, DWORD toRead, LPDWORD read, LPVOID) {
  if (read != nullptr) {
    *read = toRead;
  }
  return TRUE;
}

inline void GetSystemTime(SYSTEMTIME* t) {
  if (t == nullptr) {
    return;
  }
  t->wYear = 2026;
  t->wMonth = 1;
  t->wDayOfWeek = 0;
  t->wDay = 1;
  t->wHour = 0;
  t->wMinute = 0;
  t->wSecond = 0;
  t->wMilliseconds = 0;
}

inline DWORD GetTickCount() {
  const auto now = std::chrono::steady_clock::now().time_since_epoch();
  return static_cast<DWORD>(std::chrono::duration_cast<std::chrono::milliseconds>(now).count());
}

inline BOOL SystemTimeToFileTime(const SYSTEMTIME*, FILETIME* fileTime) {
  if (fileTime == nullptr) {
    return FALSE;
  }
  const auto now = std::chrono::system_clock::now().time_since_epoch();
  const uint64_t ticks100ns = static_cast<uint64_t>(std::chrono::duration_cast<std::chrono::nanoseconds>(now).count() / 100) + 116444736000000000ULL;
  fileTime->dwLowDateTime = static_cast<DWORD>(ticks100ns & 0xFFFFFFFFULL);
  fileTime->dwHighDateTime = static_cast<DWORD>((ticks100ns >> 32) & 0xFFFFFFFFULL);
  return TRUE;
}

inline BOOL QueryPerformanceFrequency(LARGE_INTEGER* freq) {
  if (freq == nullptr) {
    return FALSE;
  }
  freq->QuadPart = 1000000000LL;
  return TRUE;
}

inline BOOL QueryPerformanceCounter(LARGE_INTEGER* value) {
  if (value == nullptr) {
    return FALSE;
  }
  const auto now = std::chrono::steady_clock::now().time_since_epoch();
  value->QuadPart = std::chrono::duration_cast<std::chrono::nanoseconds>(now).count();
  return TRUE;
}

inline int64_t InterlockedCompareExchangeRelease64(volatile int64_t* destination, int64_t exchange, int64_t comparand) {
  const int64_t original = *destination;
  if (original == comparand) {
    *const_cast<int64_t*>(destination) = exchange;
  }
  return original;
}
