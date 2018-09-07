#pragma once


namespace multimedia
{


   namespace audio_opensles
   {


      class CLASS_DECL_AUDIO_MMSYSTEM engine :
         virtual public ::multimedia::audio::wave_base
      {
      public:



         SLObjectItf engineObject;
         SLEngineItf engineEngine;

         

         engine(sp(::base::application) papp);
         virtual ~engine();

         // creates the OpenSL ES audio engine
         SLresult create();
         void destroy();

      };


   } // namespace audio_opensles


} // namespace multimedia




