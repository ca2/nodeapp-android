#pragma once

#include <android/log.h>
#include <dlfcn.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <jni.h>
#include <time.h>
#include <android/log.h>
#include <android/bitmap.h>

#include <stdio.h>
#include <stdlib.h>
#include <math.h>


#include "aura/aura/os/os_text.h"
#include "aura/aura/os/android/android_data_exchange.h"




typedef void FN_android_fill_plasma(AndroidBitmapInfo * info, void * pixels, double  t);

typedef FN_android_fill_plasma * PFN_android_fill_plasma;

extern PFN_android_fill_plasma g_android_fill_plasma;


typedef void FN_OnSiZe(float xScreen, float yScreen, float yBitmap, float xBitmap);

typedef void FN_mouse(float x, float y);

typedef FN_mouse * PFN_mouse;

extern PFN_mouse l_button_down;

extern PFN_mouse mouse_move;

extern PFN_mouse l_button_up;

typedef FN_OnSiZe * PFN_on_size;


extern PFN_on_size on_size;

typedef void FN_text(enum e_os_text etext, const wchar_t * pwch, size_t len);

typedef FN_text * PFN_text;

extern PFN_text on_text;



typedef void FN_key(int keyCode, int iUni);

typedef FN_key * PFN_key;

extern PFN_key key_down;

extern PFN_key key_up;





extern int g_iScreenW;

extern int g_iScreenH;

extern const char * g_pszCommandLine;

extern const char * g_pszCacheDir;

extern node_data_exchange g_nodedataexchange;


void set_view_initialized(int i);
int is_view_initialized();



