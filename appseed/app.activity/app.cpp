#include "activity.h"


#define  LOG_TAG    "app.activity (app.cpp)"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)

extern "C"
void nodeos_set_cache_dir(const char * pszDir);
extern "C"
const char * nodeos_get_cache_dir();


PFN_android_fill_plasma g_android_fill_plasma = NULL;

PFN_mouse l_button_down = NULL;

PFN_mouse mouse_move = NULL;

PFN_mouse l_button_up = NULL;

node_data_exchange g_nodedataexchange;

PFN_key key_down = NULL;

PFN_key key_up = NULL;

PFN_text on_text = NULL;

void * load_lib(const char * l)
{

   void * handle = dlopen(l, RTLD_NOW | RTLD_GLOBAL);

   if (handle)
   {

      LOGI("dlopen(\"%s\"){3}: Library Opened Successfully : %s", l, l);

   }
   else
   {

      LOGE("dlopen(\"%s\"): %s", l, strerror(errno));

   }

   return handle;

}

PFN_native_activity_android_end g_pfnEnd = NULL;


void start(int iScreenWidth, int iScreenHeight, const char * pszCommandLine, const char * pszCacheDir)
{

   LOGI("start(%d, %d)", iScreenWidth, iScreenHeight);

   PFN_native_activity_android_start main = NULL;

   {

      void * handle = load_lib("libaura.so");

      if (!handle)
      {

         LOGE("Fatal: Unable to reload shared library libaura.so. It should be already be loaded thorugh Java System.loadLibrary call (explicitly or implicitly)");

         exit(1);

      }

      g_android_fill_plasma = (PFN_android_fill_plasma)dlsym(handle, "android_fill_plasma");

      if (!g_android_fill_plasma)
      {

         LOGE("Fatal: undefined symbol \"android_fill_plasma\" from libaura.so");

         exit(1);

      }

      l_button_down = (PFN_mouse)dlsym(handle, "android_l_button_down");

      if (!l_button_down)
      {

         LOGE("Fatal: undefined symbol \"android_l_button_down\" from libaura.so");

         exit(1);

      }

      mouse_move = (PFN_mouse)dlsym(handle, "android_mouse_move");

      if (!mouse_move)
      {

         LOGE("Fatal: undefined symbol \"android_l_button_down\" from libaura.so");

         exit(1);

      }

      l_button_up = (PFN_mouse)dlsym(handle, "android_l_button_up");

      if (!l_button_up)
      {

         LOGE("Fatal: undefined symbol \"android_l_button_up\" from libaura.so");

         exit(1);

      }

      key_down = (PFN_key)dlsym(handle, "android_key_down");

      if (!key_down)
      {

         LOGE("Fatal: undefined symbol \"android_key_down\" from libaura.so");

         exit(1);

      }

      key_up = (PFN_key)dlsym(handle, "android_key_up");

      if (!key_up)
      {

         LOGE("Fatal: undefined symbol \"android_key_up\" from libaura.so");

         exit(1);

      }


      on_text = (PFN_text)dlsym(handle,"android_on_text");

      if(!on_text)
      {

         LOGE("Fatal: undefined symbol \"android_on_text\" from libaura.so");

         exit(1);

      }

   }

   {

      void * handle = load_lib("liblauncher.so");

      if (!handle)
      {

         LOGE("Fatal: Unable to reload shared library liblauncher.so. It should be already be loaded thorugh Java System.loadLibrary call (explicitly or implicitly)");

         exit(1);

      }


      main = (PFN_native_activity_android_start)dlsym(handle, "native_activity_android_start");

      if (!main)
      {

         LOGE("Fatal: undefined symbol \"native_activity_android_start\" from liblauncher.so");

         exit(1);

      }

      g_pfnEnd = (PFN_native_activity_android_end)dlsym(handle, "native_activity_android_end");

   }

   node_data_exchange & nodedataexchange = g_nodedataexchange;

   nodedataexchange.m_iScreenWidth    = iScreenWidth;

   nodedataexchange.m_iScreenHeight   = iScreenHeight;

   nodedataexchange.m_pszCommandLine  = pszCommandLine;

   nodedataexchange.m_pszCacheDir     = pszCacheDir;

   nodeos_set_cache_dir(pszCacheDir);

   nodedataexchange.m_bShowKeyboard   = false;

   nodedataexchange.m_pszOpenUrl      = NULL;

   main(&nodedataexchange);

}



//extern "C"
//jint JNI_OnLoad(JavaVM* vm, void* reserved)
//{
// JNIEnv* env;
// if (vm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6) != JNI_OK) {
//    return -1;
// }
//
// // Get jclass with env->FindClass.
// // Register methods with env->RegisterNatives.
//
// return JNI_VERSION_1_6;
//
//}

int g_iScreenW = 0;
int g_iScreenH = 0;
const char * g_pszCommandLine = NULL;
const char * g_pszCacheDir = NULL;

const char * jstrdup(JNIEnv * env, jstring jstr)
{

   const char * nativeString = env->GetStringUTFChars(jstr, NULL);

   const char * psz = strdup(nativeString);

   env->ReleaseStringUTFChars(jstr, nativeString);

   return psz;

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_app_end(JNIEnv * env, jobject  obj)
{

   if (g_pfnEnd)
   {
      (*g_pfnEnd)();
   }

}


extern "C"
JNIEXPORT void JNICALL Java_com_ca2_app_configureApp(JNIEnv * env, jobject  obj, jstring strCommandLine, jstring strCacheDir, jint iScreenW, jint iScreenH)
{

   g_pszCommandLine = jstrdup(env, strCommandLine);

   g_pszCacheDir = jstrdup(env, strCacheDir);

   g_iScreenW = iScreenW;

   g_iScreenH = iScreenH;

}
