#pragma once


#include "nodeapp/operational_system/config_debug.h"


#define BSD_STYLE_SOCKETS
#if !defined VARIADIC_TEMPLATE
#define VARIADIC_TEMPLATE
#endif

//#ifdef __cplusplus
//typedef wchar_t unichar;
//#else
//typedef unsigned short unichar;
//#endif
//typedef short index;


#if defined(_DEBUG) || defined(NDEBUG)
#ifndef DEBUG
#define DEBUG
#endif // DEBUG
#ifndef _DEBUG
#define _DEBUG
#endif // _DEBUG
#ifndef NDEBUG
#define NDEBUG
#endif // NDEBUG
#endif

//#ifdef __clang__
//#define VSNORD // Visual Studio Nor-droid (Android)
//#endif

#ifndef ANDROID
#define ANDROID
#ifndef __arm__
#define MD_SWAP
#endif
#define VSNORD // Visual Studio Nor-droid (Android)
#endif

#include <features.h>

#define __WINESRC__


#define CLASS_DECL_EXPORT
#define CLASS_DECL_IMPORT


#include "bit.h"



typedef int int_bool;


#define index system_index
#define user system_user
#define lconv system_lconv
#define localeconv system_localeconv


#ifdef __cplusplus

#define LOG_THIS_TAG (demangle(typeid(*this).name()))
#define LOG_MEMBER_PREFIX __FUNCTION__

#endif



#define static_inline inline


#define FONT_MONO "mono"
#define FONT_SANS "sans-serif"
#define FONT_SERIF "FreeSerif"
#define FONT_SANS_EX "Liberation Sans"
#define FONT_SERIF_EX "Liberation Serif"
#define FONT_SANS_FX "Ubuntu"
#define FONT_SERIF_FX "Bitstream Charter"




#define _strdup strdup




#define STR_NEWLINE "\n"

