#include "framework.h"


namespace multimedia
{


   namespace audio_opensles
   {


      factory_exchange::factory_exchange(::aura::application * papp) :
         object(papp)
      {

         //System.factory().cloneable_large < wave_in               >  (System.type_info < ::multimedia::audio::wave_in                  > ());
         System.factory().cloneable_large < wave_out              >  (System.type_info < ::multimedia::audio::wave_out                 > ());

      }


      factory_exchange::~factory_exchange()
      {
      }


   } // namespace audio_opensles


} // namespace multimedia


extern "C"
void ca2_factory_exchange(::aura::application * papp)
{

   ::multimedia::audio_opensles::factory_exchange factoryexchange(papp);

}









