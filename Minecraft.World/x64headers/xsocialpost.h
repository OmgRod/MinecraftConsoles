#pragma once

#include <cstdint>

#ifndef XSOCIAL_CAPABILITY_POSTIMAGE
#define XSOCIAL_CAPABILITY_POSTIMAGE 0x00000001
#endif

#ifndef XSOCIAL_CAPABILITY_POSTLINK
#define XSOCIAL_CAPABILITY_POSTLINK 0x00000002
#endif

typedef struct _XSOCIAL_IMAGEPOSTPARAMS {
	const wchar_t* pwszTitle;
	const wchar_t* pwszCaption;
	const wchar_t* pwszDescription;
	const void* pImageData;
	uint32_t dwImageDataSize;
} XSOCIAL_IMAGEPOSTPARAMS;

typedef struct _XSOCIAL_LINKPOSTPARAMS {
	const wchar_t* pwszTitle;
	const wchar_t* pwszCaption;
	const wchar_t* pwszDescription;
	const wchar_t* pwszLinkUrl;
} XSOCIAL_LINKPOSTPARAMS;
