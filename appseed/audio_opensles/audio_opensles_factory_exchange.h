#pragma once


namespace multimedia
{


   namespace audio_opensles
   {


      class CLASS_DECL_AUDIO_MMSYSTEM factory_exchange :
         virtual public ::object
      {
      public:


         factory_exchange(::aura::application * papp);
         virtual ~factory_exchange();

      };


   } // namespace audio_opensles


} // namespace multimedia






