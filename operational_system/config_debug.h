#pragma once


#include "platform_configuration.h"


#define MEMDLEAK 0
#define STORE_LAST_BLOCK 0


//#define __VLD

//#define __MCRTDBG

#define VARIADIC_TEMPLATE_FORMAT2


#ifndef _CRT_SECURE_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#endif


#ifdef __VLD
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#include "vld.h"
#define memory_alloc malloc
#define memory_realloc realloc
#define memory_free free
#endif

#ifdef __MCRTDBG
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>
#define memory_alloc malloc
#define memory_realloc realloc
#define memory_free free
#endif


#ifdef MEMDLEAK
#endif












