/*
* Copyright (C) 2010 The Android Open Source Project
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
* You may obtain a copy of the License at
*
*      http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS,
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*/

//
// pch.h
// Header for standard system include files.
//
// Used by the build system to generate the precompiled header. Note that no
// pch.cpp is needed and the pch.h is automatically included in all cpp files
// that are part of the project
//

#include "core/core/core/core.h"
#include "axis/os/android/android.h"
#include "axis/node/android/android.h"
#include "aura/aura/os/android/android_data_exchange.h"
#include "aura/user/user/user_interaction.h"
#include "aura/aura/os/android/android_windowing.h"

#define LOGI(...) ((void)__android_log_print(ANDROID_LOG_INFO, "AndroidProject1.NativeActivity", __VA_ARGS__))
#define LOGW(...) ((void)__android_log_print(ANDROID_LOG_WARN, "AndroidProject1.NativeActivity", __VA_ARGS__))

//extern "C"
//void lib_ANativeActivity_onCreate(ANativeActivity* activity, void* savedState, size_t savedStateSize);


/**
* Our saved state data.
*/
struct saved_state
{
   float angle;
   int32_t x;
   int32_t y;
};


/**
* Initialize an EGL context for the current display.
*/
static int engine_init_display(oswindow window)
{
   // initialize OpenGL ES and EGL


   return 0;
}

/**
* Just the current frame in the display.
*/
static void engine_draw_frame(oswindow window)
{
   //if (window->m_engine.display == NULL) {
   // // No display.
   // return;
   //}

   window->get_user_interaction()->_001UpdateWindow();

   // Just fill the screen with a color.
   //glClearColor(((float)window->m_engine.m_pstate->x) / window->m_engine.width, window->m_engine.m_pstate->angle,
   // ((float)window->m_engine.m_pstate->y) / window->m_engine.height, 1);
   //glClear(GL_COLOR_BUFFER_BIT);

   //eglSwapBuffers(window->m_engine.display, window->m_engine.surface);
}

/**
* Tear down the EGL context currently associated with the display.
*/
static void engine_term_display(oswindow window)
{
   //if (window->m_engine.display != EGL_NO_DISPLAY) {
   // eglMakeCurrent(window->m_engine.display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
   // if (window->m_engine.context != EGL_NO_CONTEXT) {
   //    eglDestroyContext(window->m_engine.display, window->m_engine.context);
   // }
   // if (window->m_engine.surface != EGL_NO_SURFACE) {
   //    eglDestroySurface(window->m_engine.display, window->m_engine.surface);
   // }
   // eglTerminate(window->m_engine.display);
   //}
   window->m_engine.animating = 0;
   //window->m_engine.display = EGL_NO_DISPLAY;
   //window->m_engine.context = EGL_NO_CONTEXT;
   //window->m_engine.surface = EGL_NO_SURFACE;
}

/**
* Process the next input event.
*/
static int32_t engine_handle_input(struct android_app* app, AInputEvent* event)
{
   oswindow window = (oswindow)app->userData;
   if (AInputEvent_getType(event) == AINPUT_EVENT_TYPE_MOTION)
   {
      window->m_engine.m_pstate->x = AMotionEvent_getX(event, 0);
      window->m_engine.m_pstate->y = AMotionEvent_getY(event, 0);
      return 1;
   }
   return 0;
}

/**
* Process the next main command.
*/
static void engine_handle_cmd(struct android_app* app, int32_t cmd)
{
   oswindow window = (oswindow)app->userData;
   switch (cmd)
   {
   case APP_CMD_SAVE_STATE:
      // The system has asked us to save our current state.  Do so.
      window->m_engine.app->savedState = malloc(sizeof(struct saved_state));
      *((struct saved_state*)window->m_engine.app->savedState) = *window->m_engine.m_pstate;
      window->m_engine.app->savedStateSize = sizeof(struct saved_state);
      break;
   case APP_CMD_INIT_WINDOW:
      // The window is being shown, get it ready.
      if (window->m_engine.app->window != NULL)
      {
         engine_init_display(window);
         engine_draw_frame(window);
      }
      break;
   case APP_CMD_TERM_WINDOW:
      // The window is being hidden or closed, clean it up.
      engine_term_display(window);
      break;
   case APP_CMD_GAINED_FOCUS:
      // When our app gains focus, we start monitoring the accelerometer.
      if (window->m_engine.accelerometerSensor != NULL)
      {
         ASensorEventQueue_enableSensor(window->m_engine.sensorEventQueue,
                                        window->m_engine.accelerometerSensor);
         // We'd like to get 60 events per second (in us).
         ASensorEventQueue_setEventRate(window->m_engine.sensorEventQueue,
                                        window->m_engine.accelerometerSensor, (1000L / 60) * 1000);
      }
      break;
   case APP_CMD_LOST_FOCUS:
      // When our app loses focus, we stop monitoring the accelerometer.
      // This is to avoid consuming battery while not being used.
      if (window->m_engine.accelerometerSensor != NULL)
      {
         ASensorEventQueue_disableSensor(window->m_engine.sensorEventQueue,
                                         window->m_engine.accelerometerSensor);
      }
      // Also stop animating.
      window->m_engine.animating = 0;
      engine_draw_frame(window);
      break;
   }
}


extern unichar * g_pwszCommandLine;

/**
* This is the main entry point of a native application that is using
* android_native_app_glue.  It runs in its own thread, with its own
* event loop for receiving input events and doing other things.
*/
//void native_activity_android_start(struct android_app* state) {

class aura_aura * g_paura = NULL;

class aura_main_data * g_pmaindata = NULL;

extern "C"
void native_activity_android_end()
{

   if (g_pmaindata != NULL)
   {

      delete g_pmaindata;

      g_pmaindata = NULL;

   }

   if (g_paura != NULL)
   {

      g_paura->~aura_aura();

      free(g_paura);

      g_paura = NULL;

   }

}

extern "C"
void native_activity_android_start(node_data_exchange * pnodedataexchange)
{

   g_paura = (class aura_aura *)malloc(sizeof(class aura_aura));

#undef new

   new(g_paura) class aura_aura();

   g_pmaindata = new aura_main_data((char *) pnodedataexchange->m_pszCommandLine);

   g_pmaindata->m_pnodedataexchange = pnodedataexchange;

   int iExitCode = aura_aura(g_pmaindata);

   //exit(iExitCode);

   //return iExitCode;

//#else

   //return (int)aura_aura(pmaindata);

   //app_core
   //app_core appcore(;

   //if (!defer_core_init())
   //   return;

   //::core::system * psystem = new ::core::system(NULL, &appcore, );

   // psystem->m_durationRunLock = millis(1);

   //psystem->::exception::translator::attach();

   //int32_t nReturnCode = 0;

   //::command::command * pcommandStartup = new ::command::command;

   //if (pdataexchange != NULL)
   //{

   //   pcommandStartup->m_strCommandLine = pdataexchange->m_pszCommandLine;

   //}
   //else
   //{

   //   pcommandStartup->m_strCommandLine = "app : app=app-core/hellomultiverse";

   //}

   //wstring wstrCommandLine(pcommandStartup->m_strCommandLine);

   //g_pwszCommandLine = (unichar *) malloc((wstrCommandLine.length() + 1) * sizeof(unichar));

   //wcscpy_dup(g_pwszCommandLine, wstrCommandLine);

   ////pcommandStartup->->m_nCmdShow = SW_SHOW;

   //psystem->startup_command(pcommandStartup);

   //bool bOk = true;

   //try
   //{

   //   if (psystem->begin_synch())
   //   {

   //      bOk = true;

   //   }

   //}
   //catch (...)
   //{

   //}

   //engine & engine = pwindow->m_engine;

   //engine.m_pstate = new saved_state;

   //memset(&engine, 0, sizeof(engine));
   //state->userData = &engine;
   //state->onAppCmd = engine_handle_cmd;
   //state->onInputEvent = engine_handle_input;
   //engine.app = state;

   //// Prepare to monitor accelerometer
   //engine.sensorManager = ASensorManager_getInstance();
   //engine.accelerometerSensor = ASensorManager_getDefaultSensor(engine.sensorManager,
   // ASENSOR_TYPE_ACCELEROMETER);
   //engine.sensorEventQueue = ASensorManager_createEventQueue(engine.sensorManager,
   // state->looper, LOOPER_ID_USER, NULL, NULL);

   //if (state->savedState != NULL) {
   // // We are starting with a previous saved state; restore from it.
   // *engine.m_pstate = *(struct saved_state*)state->savedState;
   //}

   //engine.animating = 1;

   //psystem->main();

   // loop waiting for stuff to do.

   //while (1) {
   // // Read all pending events.
   // int ident;
   // int events;
   // struct android_poll_source* source;

   // // If not animating, we will block forever waiting for events.
   // // If animating, we loop until all events are read, then continue
   // // to draw the next frame of animation.
   // while ((ident = ALooper_pollAll(engine.animating ? 0 : -1, NULL, &events,
   //    (void**)&source)) >= 0) {

   //    // Process this event.
   //    if (source != NULL) {
   //       source->process(state, source);
   //    }

   //    // If a sensor has data, process it now.
   //    if (ident == LOOPER_ID_USER) {
   //       if (engine.accelerometerSensor != NULL) {
   //          ASensorEvent event;
   //          while (ASensorEventQueue_getEvents(engine.sensorEventQueue,
   //             &event, 1) > 0) {
   //             LOGI("accelerometer: x=%f y=%f z=%f",
   //                event.acceleration.x, event.acceleration.y,
   //                event.acceleration.z);
   //          }
   //       }
   //    }

   //    // Check if we are exiting.
   //    if (state->destroyRequested != 0) {
   //       engine_term_display(window);
   //       return;
   //    }
   // }

   // if (engine.animating) {
   //    // Done with events; draw next animation frame.
   //    engine.state.angle += .01f;
   //    if (engine.state.angle > 1) {
   //       engine.state.angle = 0;
   //    }

   //    // Drawing is throttled to the screen update rate, so there
   //    // is no need to do timing here.
   //    engine_draw_frame(window);
   // }
   //}

   //defer_core_term();

}


//class native_activity_static_init
//{
//public:
//
// native_activity_static_init()
// {
//    g_android_main = native_activity_android_start;
// }
// ~native_activity_static_init()
// {
// }
//
//} g_native_activity_static_init;



//void ANativeActivity_onCreate(ANativeActivity* activity,
// void* savedState, size_t savedStateSize)
//{
// //lib_ANativeActivity_onCreate(activity, savedState, savedStateSize);
//
//}
