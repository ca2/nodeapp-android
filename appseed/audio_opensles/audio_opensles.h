#pragma once


#include "app-core/appseed/multimedia/multimedia/multimedia.h"
#include <SLES/OpenSLES.h>
#include <SLES/OpenSLES_Android.h>

#define DEBUG_SND TRACE

#if defined(_AUDIO_ALSA_LIBRARY)
   #define CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_EXPORT
#else
   #define  CLASS_DECL_AUDIO_MMSYSTEM  CLASS_DECL_IMPORT
#endif


#include "audio_opensles_translation.h"


#include "audio_opensles_engine.h"


#include "audio_opensles_wave_out.h"
//#include "audio_opensles_wave_in.h"


#include "audio_opensles_factory_exchange.h"
