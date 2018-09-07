#include "framework.h"


namespace multimedia
{


   namespace audio_opensles
   {


      engine::engine(sp(::base::application) papp) :
         object(papp),
         thread(papp),
         wave_base(papp)
      {
         engineObject = NULL;
         engineEngine = NULL;
         //m_ppcm            = NULL;
         //m_phwparams       = NULL;
         //m_pswparams       = NULL;
         //m_phandler        = NULL;
         //m_iCurrentBuffer  = 0;
         //buffer_time       = 100 * 1000; /* ring buffer length in us */
         //period_time       =  20 * 1000; /* period time in us */
      }

      engine::~engine()
      {

         destroy();

      }

      // creates the OpenSL ES audio engine
      SLresult engine::create()
      {
         SLresult result;
         // create engine
         result = slCreateEngine(&engineObject, 0, NULL, 0, NULL, NULL);
         //DEBUG_SND("engineObject=%p", engineObject);
         if (result != SL_RESULT_SUCCESS) goto  engine_end;

         // realize the engine 
         result = (*engineObject)->Realize(engineObject, SL_BOOLEAN_FALSE);
         //DEBUG_SND("Realize=%d", result);
         if (result != SL_RESULT_SUCCESS) goto engine_end;

         // get the engine interface, which is needed in order to create other objects
         result = (*engineObject)->GetInterface(engineObject, SL_IID_ENGINE, &(engineEngine));
         //DEBUG_SND("engineEngine=%p", engineEngine);
         if (result != SL_RESULT_SUCCESS) goto  engine_end;

      engine_end:
         return result;
      }     
      
      
      void engine::destroy()
      {

         // destroy engine object, and invalidate all associated interfaces
         if (engineObject != NULL)
         {
            (*engineObject)->Destroy(engineObject);
            engineObject = NULL;
            engineEngine = NULL;
         }

      }



   } // namespace audio_opensles


} // namespace multimedia







