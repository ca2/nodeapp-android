#include "activity.h"


// https://stackoverflow.com/questions/39823375/clarification-about-getfieldid
// https://stackoverflow.com/users/192373/alex-cohn
// 1
// down vote
// "I" here is the signature of the primitive integer type. Oracle provides a table of type signatures: http://docs.oracle.com/javase/7/docs/technotes/guides/jni/spec/types.html#wp276

// Z boolean
// B byte
// C char
// S short
// I int
// J long
// F float
// D double
// For non-primitive types, the signature is of form

// L fully-qualified-class ;
// For arrays, [ is added.

// For methods, () are used to separate parameters from the return value.

#define  LOG_TAG    "app.activity (view.cpp)"
#define  LOGI(...)  __android_log_print(ANDROID_LOG_INFO,LOG_TAG,__VA_ARGS__)
#define  LOGE(...)  __android_log_print(ANDROID_LOG_ERROR,LOG_TAG,__VA_ARGS__)


void start(int iScreenWidth, int iScreenHeight, const char * pszCommandLine, const char * pszCacheDir);

typedef int32_t  Fixed;



extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_renderImpact(JNIEnv * env, jobject  obj, jobject bitmap, jlong  time_ms, jobject dataexchange)
{

   AndroidBitmapInfo  info;
   void*              pixels;
   int                ret;
   static int         init;

   if (!init)
   {
      init = 1;
      start(g_iScreenW, g_iScreenH, g_pszCommandLine, g_pszCacheDir);
      //init_tables();
      //stats_init(&stats);
   }

   if ((ret = AndroidBitmap_getInfo(env, bitmap, &info)) < 0)
   {
      LOGE("AndroidBitmap_getInfo() failed ! error=%d", ret);
      return;
   }

   if (info.format != ANDROID_BITMAP_FORMAT_RGBA_8888)
   {
      LOGE("Bitmap format is not RGB_565 !");
      return;
   }

   if ((ret = AndroidBitmap_lockPixels(env, bitmap, &pixels)) < 0)
   {
      LOGE("AndroidBitmap_lockPixels() failed ! error=%d", ret);
   }

   //stats_startFrame(&stats);

   /* Now fill the values with a nice little plasma */
   try
   {

      g_android_fill_plasma(&info, pixels, time_ms);

   }
   catch (...)
   {

      LOGE("exception at g_android_fill_plasma");

   }

   AndroidBitmap_unlockPixels(env, bitmap);

   //stats_endFrame(&stats);

   if (g_nodedataexchange.m_bShowKeyboard)
   {

      jfieldID fid;

      jmethodID mid;

      jclass myclass;

      jclass cls = env->GetObjectClass(dataexchange);

      fid = env->GetFieldID(cls, "m_bShowKeyboard", "Z");

      env->SetBooleanField(dataexchange, fid, g_nodedataexchange.m_bShowKeyboard);

      g_nodedataexchange.m_bShowKeyboard = false;

   }

   if (g_nodedataexchange.m_bHideKeyboard)
   {

      jfieldID fid;

      jmethodID mid;

      jclass myclass;

      jclass cls = env->GetObjectClass(dataexchange);

      fid = env->GetFieldID(cls, "m_bHideKeyboard", "Z");

      LOGI("%s\n", "Java_com_ca2_view_renderImpact g_dataexchange.m_bHideKeyboard : true");

      env->SetBooleanField(dataexchange, fid, g_nodedataexchange.m_bHideKeyboard);

      g_nodedataexchange.m_bHideKeyboard = false;

   }

   if (g_nodedataexchange.m_pszOpenUrl != NULL)
   {

      jfieldID fid;

      jmethodID mid;

      jclass myclass;

      jclass cls = env->GetObjectClass(dataexchange);

      fid = env->GetFieldID(cls, "m_strOpenUrl", "Ljava/lang/String;");

      if (fid != NULL)
      {

         // String
         // Get the object given the Field ID
         //jstring message = (*env)->GetObjectField(env,dataexchange,fid);
         jstring message = env->NewStringUTF(g_nodedataexchange.m_pszOpenUrl);
         if (NULL == message) return;

         // modify the instance variables
         env->SetObjectField(dataexchange, fid, message);


         free((void *)g_nodedataexchange.m_pszOpenUrl);

      }

      g_nodedataexchange.m_pszOpenUrl = NULL;

   }

   if (g_nodedataexchange.m_pszUserWallpaper != NULL)
   {

      jfieldID fid;

      jmethodID mid;

      jclass myclass;

      jclass cls = env->GetObjectClass(dataexchange);

      fid = env->GetFieldID(cls, "m_strWallpaper", "Ljava/lang/String;");

      if (fid != NULL)
      {

         // String
         // Get the object given the Field ID
         //jstring message = (*env)->GetObjectField(env,dataexchange,fid);
         jstring message = env->NewStringUTF(g_nodedataexchange.m_pszUserWallpaper);
         if (NULL == message) return;

         // modify the instance variables
         env->SetObjectField(dataexchange, fid, message);


         free((void *)g_nodedataexchange.m_pszUserWallpaper);

      }

      g_nodedataexchange.m_pszUserWallpaper = NULL;

   }

   if (g_nodedataexchange.m_bGetUserWallpaper)
   {

      jfieldID fid;

      jmethodID mid;

      jclass myclass;

      jclass cls = env->GetObjectClass(dataexchange);

      fid = env->GetFieldID(cls, "m_bGetWallpaper", "Z");

      if (fid != NULL)
      {

         env->SetBooleanField(dataexchange, fid, g_nodedataexchange.m_bGetUserWallpaper);

         g_nodedataexchange.m_bGettingUserWallpaper = true;

         g_nodedataexchange.m_bGetUserWallpaper = false;

      }

   }


}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_keyDown(JNIEnv * env, jobject  obj, jint keyCode)
{

   LOGI("%s\n", "Java_com_ca2_view_keyDown");

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_keyUp(JNIEnv * env, jobject  obj, jint keyCode)
{

   LOGI("%s\n", "Java_com_ca2_view_keyUp");

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_keyPreImeDown(JNIEnv * env, jobject  obj, jint keyCode, jint iUni)
{

   //LOGI("%s\n", "Java_com_ca2_view_keyPreImeDown");

   key_down(keyCode, iUni);

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_keyPreImeUp(JNIEnv * env, jobject  obj, jint keyCode, jint iUni)
{

   LOGI("%s\n", "Java_com_ca2_view_keyPreImeUp");

   key_up(keyCode, iUni);

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_onReceivedShowKeyboard(JNIEnv * env, jobject  obj)
{

   LOGI("%s\n", "Java_com_view_onReceivedShowKeyboard");

}


extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_onReceivedHideKeyboard(JNIEnv * env, jobject  obj)
{

   LOGI("%s\n", "Java_com_view_onReceivedHideKeyboard");

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_lButtonDown(JNIEnv * env, jobject  obj, jfloat x, jfloat y)
{

   LOGI("%s\n", "Java_com_ca2_view_lButtonDown");

   l_button_down(x, y);

}


extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_mouseMove(JNIEnv * env, jobject  obj, jfloat x, jfloat y)
{

   mouse_move(x, y);

}


extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_lButtonUp(JNIEnv * env, jobject  obj, jfloat x, jfloat y)
{

   LOGI("%s\n", "Java_com_ca2_view_lButtonUp");

   l_button_up(x, y);

}


extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_onText(JNIEnv * env, jobject  obj, jstring bytes)
{

   LOGI("%s\n", "Java_com_ca2_view_onText");

   if (bytes == NULL)
   {

      return;

   }

   const wchar_t * utf16 = (wchar_t *)env->GetStringChars(bytes, NULL);

   if (utf16 == NULL)
   {

      return;

   }

   size_t length = (size_t)env->GetStringLength(bytes);

   on_text(os_text_keyboard, utf16, length);

   env->ReleaseStringChars(bytes, (jchar *)utf16);

}

extern "C"
JNIEXPORT void JNICALL Java_com_ca2_view_wallpaperResponse(JNIEnv * env, jobject  obj, jstring bytes)
{

   LOGI("%s\n", "Java_com_ca2_view_wallpaperResponse");

   if (bytes == NULL)
   {

      return;

   }

   const wchar_t * utf16 = (wchar_t *)env->GetStringChars(bytes, NULL);

   if (utf16 == NULL)
   {

      return;

   }

   size_t length = (size_t)env->GetStringLength(bytes);

   on_text(os_text_wallpaper, utf16, length);

   env->ReleaseStringChars(bytes, (jchar *)utf16);

}

