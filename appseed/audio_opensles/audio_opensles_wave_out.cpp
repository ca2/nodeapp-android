#include "framework.h"

extern int g_iAndroidSampleRate;

extern int g_iAndroidBufferSize;

void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context);

namespace multimedia
{


   namespace audio_opensles
   {


      wave_out::wave_out(::aura::application * papp) :
         object(papp),
         ::thread(papp),
         wave_base(papp),
         engine(papp),
         ::multimedia::audio::wave_out(papp)

      {

         m_estate             = state_initial;
         m_pthreadCallback    = NULL;
         m_iBufferedCount     = 0;
         m_mmr                = result_success;
         m_peffect            = NULL;
         m_bWrite             = false;


         // output mix interfaces
         outputMixObject = NULL;

         // buffer queue player interfaces
         bqPlayerObject = NULL;
         bqPlayerPlay = NULL;
         bqPlayerVolume = NULL;
         bqPlayerBufferQueue = NULL;
         bqPlayerEffectSend = NULL;


      }


      wave_out::~wave_out()
      {


      }


      void wave_out::install_message_routing(::message::sender * pinterface)
      {

         ::multimedia::audio::wave_out::install_message_routing(pinterface);

         //IGUI_MSG_LINK(message_ready, pinterface, this, &wave_out::OnReady);
         //IGUI_MSG_LINK(message_free, pinterface, this, &wave_out::OnFree);

      }


      bool wave_out::init_thread()
      {

         if (!::multimedia::audio::wave_out::init_thread())
         {

            return false;

         }

         return true;

      }


      void wave_out::term_thread()
      {

         ::multimedia::audio::wave_out::term_thread();

         thread::term_thread();

      }


      //::multimedia::e_result wave_out::wave_out_open(thread * pthreadCallback, int32_t iBufferCount, int32_t iBufferSampleCount)
      //{
      //   single_lock sLock(m_pmutex, TRUE);
      //   if(engineObject != NULL &&
      //         m_estate != state_initial)
      //      return result_success;
      //   m_pthreadCallback = pthreadCallback;
      //   ::multimedia::e_result mmr;
      //   ASSERT(engineObject == NULL);
      //   ASSERT(m_estate == state_initial);

      //   //m_pwaveformat->wFormatTag = WAVE_FORMAT_PCM;
      //   m_pwaveformat->wFormatTag = 0;
      //   m_pwaveformat->nChannels = 2;
      //   m_pwaveformat->nSamplesPerSec = 44100;
      //   m_pwaveformat->wBitsPerSample = sizeof(multimedia::audio::WAVEBUFFERDATA) * 8;
      //   m_pwaveformat->nBlockAlign = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
      //   m_pwaveformat->nAvgBytesPerSec = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
      //   m_pwaveformat->cbSize = 0;

      //   //if((m_mmr = this->snd_pcm_open(SND_PCM_STREAM_PLAYBACK)) != result_success)
      //   //{

      //   //   return result_error;

      //   //}



      //   //uint32_t uiBufferSizeLog2;
      //   //uint32_t uiBufferSize;
      //   //uint32_t uiAnalysisSize;
      //   //uint32_t uiAllocationSize;
      //   //uint32_t uiInterestSize;
      //   //uint32_t uiSkippedSamplesCount;
      //   //uint32_t uiBufferCount = iBufferCount;

      //   //if(m_pwaveformat->nSamplesPerSec == 44100)
      //   //{
      //   //   uiBufferSizeLog2 = 16;
      //   //   uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
      //   //   uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
      //   //   if(iBufferCount > 0)
      //   //   {
      //   //      uiAllocationSize = iBufferCount * uiAnalysisSize;
      //   //   }
      //   //   else
      //   //   {
      //   //      uiAllocationSize = 8 * uiAnalysisSize;
      //   //   }
      //   //   uiInterestSize = 200;
      //   //   uiSkippedSamplesCount = 2;
      //   //}
      //   //else if(m_pwaveformat->nSamplesPerSec == 22050)
      //   //{
      //   //   uiBufferSizeLog2 = 10;
      //   //   uiBufferSize = 4 * 1 << uiBufferSizeLog2;
      //   //   uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
      //   //   uiAllocationSize = 4 * uiAnalysisSize;
      //   //   uiInterestSize = 200;
      //   //   uiSkippedSamplesCount = 1;
      //   //}
      //   //else if(m_pwaveformat->nSamplesPerSec == 11025)
      //   //{
      //   //   uiBufferSizeLog2 = 10;
      //   //   uiBufferSize = 2 * 1 << uiBufferSizeLog2;
      //   //   uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
      //   //   uiAllocationSize = 4 * uiAnalysisSize;
      //   //   uiInterestSize = 200;
      //   //   uiSkippedSamplesCount = 1;
      //   //}

      //   ////uiBufferCount = 1;

      //   //wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, uiBufferCount, 128, m_pwaveformat, m_pwaveformat);

      //   //m_pprebuffer->open(this, m_pwaveformat->nChannels, uiBufferCount, iBufferSampleCount); // group sample count

      //   //int iFrameSize = (m_pwaveformat->nChannels * m_pwaveformat->wBitsPerSample) / 8;

      //   //int err;

      //   //snd_pcm_sw_params_alloca(&m_pswparams);

      //   ///* get the current m_pswparams */
      //   //err = snd_pcm_sw_params_current(m_ppcm, m_pswparams);

      //   //if (err < 0)
      //   //{

      //   //   TRACE("Unable to determine current m_pswparams for playback: %s\n", snd_strerror(err));

      //   //   return result_error;

      //   //}


      //   ///* start the transfer when the buffer is almost full: */
      //   ///* (buffer_size / avail_min) * avail_min */
      //   //err = snd_pcm_sw_params_set_start_threshold(m_ppcm, m_pswparams, (buffer_size / period_size) * period_size);
      //   //if (err < 0)
      //   //{

      //   //   TRACE("Unable to set start threshold mode for playback: %s\n", snd_strerror(err));

      //   //   return result_error;

      //   //}

      //   ///* allow the transfer when at least period_size samples can be processed */
      //   //err = snd_pcm_sw_params_set_avail_min(m_ppcm, m_pswparams, period_size);
      //   //if (err < 0)
      //   //{

      //   //   TRACE("Unable to set avail min for playback: %s\n", snd_strerror(err));

      //   //   return result_error;

      //   //}

      //   ///* write the parameters to the playback device */
      //   //err = snd_pcm_sw_params(m_ppcm, m_pswparams);
      //   //if (err < 0)
      //   //{

      //   //   TRACE("Unable to set sw params for playback: %s\n", snd_strerror(err));

      //   //   return result_error;

      //   //}

      //   m_estate = state_opened;
      //   return result_success;
      //}





      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose)
      {

         single_lock sLock(m_pmutex, TRUE);

         if(engineObject != NULL && m_estate != state_initial)
            return result_success;

         m_pthreadCallback = pthreadCallback;

         int32_t iBufferCount;
         int32_t iBufferSampleCount;

         iBufferCount = 4;
         int period_size = g_iAndroidBufferSize;
         ASSERT(engineObject == NULL);

         ASSERT(m_estate == state_initial);

         m_pwaveformat->wFormatTag        = 0;
         m_pwaveformat->nChannels         = (WORD) uiChannelCount;
         m_pwaveformat->nSamplesPerSec    = uiSamplesPerSec;
         m_pwaveformat->wBitsPerSample    = (WORD) uiBitsPerSample;
         m_pwaveformat->nBlockAlign       = m_pwaveformat->wBitsPerSample * m_pwaveformat->nChannels / 8;
         m_pwaveformat->nAvgBytesPerSec   = m_pwaveformat->nSamplesPerSec * m_pwaveformat->nBlockAlign;
         m_pwaveformat->cbSize            = 0;

         SLresult result;
         SLuint32 sr = uiSamplesPerSec;
         SLuint32  channels = uiChannelCount;
         //uint32_t uiBufferSizeLog2;
         uint32_t uiBufferSize;
         //uint32_t uiAnalysisSize;
         //uint32_t uiAllocationSize;
         //uint32_t uiInterestSize;
         //uint32_t uiSkippedSamplesCount;
         int err;
         if (create() != SL_RESULT_SUCCESS)
            return ::multimedia::result_error;

         // configure audio source
         SLDataLocator_AndroidSimpleBufferQueue loc_bufq =
         {
            SL_DATALOCATOR_ANDROIDSIMPLEBUFFERQUEUE,
            iBufferCount
         };
         m_iBufferCount = iBufferCount;
         switch (sr)
         {

         case 8000:
            sr = SL_SAMPLINGRATE_8;
            break;
         case 11025:
            sr = SL_SAMPLINGRATE_11_025;
            break;
         case 16000:
            sr = SL_SAMPLINGRATE_16;
            break;
         case 22050:
            sr = SL_SAMPLINGRATE_22_05;
            break;
         case 24000:
            sr = SL_SAMPLINGRATE_24;
            break;
         case 32000:
            sr = SL_SAMPLINGRATE_32;
            break;
         case 44100:
            sr = SL_SAMPLINGRATE_44_1;
            break;
         case 48000:
            sr = SL_SAMPLINGRATE_48;
            break;
         case 64000:
            sr = SL_SAMPLINGRATE_64;
            break;
         case 88200:
            sr = SL_SAMPLINGRATE_88_2;
            break;
         case 96000:
            sr = SL_SAMPLINGRATE_96;
            break;
         case 192000:
            sr = SL_SAMPLINGRATE_192;
            break;
         default:
            return ::multimedia::result_error;
         }

         {

            //const SLInterfaceID ids[] = { SL_IID_VOLUME };
            //const SLboolean req[] = { SL_BOOLEAN_FALSE };
            //result = (*engineEngine)->CreateOutputMix(engineEngine, &(outputMixObject), 1, ids, req);
            result = (*engineEngine)->CreateOutputMix(engineEngine, &(outputMixObject), 0, NULL, NULL);
            output_debug_string("engineEngine="+ ::str::from((uint_ptr)engineEngine));
            ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // realize the output mix
            result = (*outputMixObject)->Realize(outputMixObject, SL_BOOLEAN_FALSE);
            output_debug_string("Realize" + ::str::from((uint_ptr)result));
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            int speakers;
            if (channels > 1)
               speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;
            else speakers = SL_SPEAKER_FRONT_CENTER;
            SLDataFormat_PCM format_pcm = { SL_DATAFORMAT_PCM,channels, sr,
                                            SL_PCMSAMPLEFORMAT_FIXED_16, SL_PCMSAMPLEFORMAT_FIXED_16,
                                            speakers, SL_BYTEORDER_LITTLEENDIAN
                                          };

            SLDataSource audioSrc = { &loc_bufq, &format_pcm };

            // configure audio sink
            SLDataLocator_OutputMix loc_outmix = { SL_DATALOCATOR_OUTPUTMIX, outputMixObject };
            SLDataSink audioSnk = { &loc_outmix, NULL };

            // create audio player
            const SLInterfaceID ids1[] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME };
            const SLboolean req1[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
            result = (*engineEngine)->CreateAudioPlayer(engineEngine,
                     &(bqPlayerObject), &audioSrc, &audioSnk, 2, ids1, req1);
            output_debug_string("bqPlayerObject="+::str::from((uint_ptr)bqPlayerObject));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // realize the player
            result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
            output_debug_string("Realize="+::str::from((uint_ptr)result));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // get the play interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &(bqPlayerPlay));
            output_debug_string("bqPlayerPlay=" + ::str::from((uint_ptr) bqPlayerPlay));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // get the volume interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &(bqPlayerVolume));
            output_debug_string("bqPlayerVolume=" + ::str::from((uint_ptr) bqPlayerVolume));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // get the buffer queue interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                     &(bqPlayerBufferQueue));
            ::output_debug_string("bqPlayerBufferQueue=" + ::str::from((uint_ptr) bqPlayerBufferQueue));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;

            // register callback on the buffer queue
            result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, this);
            output_debug_string("bqPlayerCallback=" + ::str::from((uint_ptr)bqPlayerCallback));
            //ASSERT(!result);
            if (result != SL_RESULT_SUCCESS) goto end_openaudio;


         }


         if(true)
         {
            //uiBufferSizeLog2 = 16;
            //uiBufferSize = m_pwaveformat->nChannels * 2 * iBufferSampleCount; // 512 kbytes
            /*while(((double)(buffer_size * 8) / (double)(uiBitsPerSample * uiSamplesPerSec)) > 0.084)
            {
                buffer_size /= 2;
            }*/
            uiBufferSize = period_size * (m_pwaveformat->nChannels * 2);
            iBufferSampleCount = period_size;
            //uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            //if(iBufferCount > 0)
            //{
            //   uiAllocationSize = iBufferCount * uiAnalysisSize;
            //}
            //else
            //{
            //   uiAllocationSize = 8 * uiAnalysisSize;
            //}
            //uiInterestSize = 200;
            //uiSkippedSamplesCount = 2;
         }
         else if(m_pwaveformat->nSamplesPerSec == 22050)
         {
            //uiBufferSizeLog2 = 10;
            //uiBufferSize = 4 * 1 << uiBufferSizeLog2;
            //uiAnalysisSize = 4 * 1 << uiBufferSizeLog2;
            //uiAllocationSize = 4 * uiAnalysisSize;
            //uiInterestSize = 200;
            //uiSkippedSamplesCount = 1;
         }
         else if(m_pwaveformat->nSamplesPerSec == 11025)
         {
            //uiBufferSizeLog2 = 10;
            //uiBufferSize = 2 * 1 << uiBufferSizeLog2;
            //uiAnalysisSize = 2 * 1 << uiBufferSizeLog2;
            //uiAllocationSize = 4 * uiAnalysisSize;
            //uiInterestSize = 200;
            //uiSkippedSamplesCount = 1;
         }

         wave_out_get_buffer()->PCMOutOpen(this, uiBufferSize, iBufferCount, 128, m_pwaveformat, m_pwaveformat);

         m_pprebuffer->open(m_pwaveformat->nChannels, iBufferCount, iBufferSampleCount);

         //m_pprebuffer->SetMinL1BufferCount(wave_out_get_buffer()->GetBufferCount());

         m_estate = state_opened;

         m_epurpose = epurpose;
end_openaudio:
         if (result == SL_RESULT_SUCCESS)
         {
            output_debug_string("wave_out::wave_out_open_ex success");
            return ::multimedia::result_success;
         }
         else
         {
            output_debug_string("wave_out::wave_out_open_ex error");
            return ::multimedia::result_error;
         }

      }



      ::multimedia::e_result wave_out::wave_out_close()
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate == state_playing)
         {
            wave_out_stop();
         }

         if(m_estate != state_opened)
            return result_success;

         ::multimedia::e_result mmr;


         // destroy buffer queue audio player object, and invalidate all associated interfaces
         if (bqPlayerObject != NULL)
         {
            (*bqPlayerObject)->Destroy(bqPlayerObject);
            bqPlayerObject = NULL;
            bqPlayerVolume = NULL;
            bqPlayerPlay = NULL;
            bqPlayerBufferQueue = NULL;
            bqPlayerEffectSend = NULL;
         }

         // destroy output mix object, and invalidate all associated interfaces
         if (outputMixObject != NULL)
         {
            (*outputMixObject)->Destroy(outputMixObject);
            outputMixObject = NULL;
         }


         destroy();

         ::multimedia::audio::wave_out::wave_out_close();

         m_estate = state_initial;

         return result_success;

      }





      ::multimedia::e_result wave_out::wave_out_stop()
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate != state_playing && m_estate != state_paused)
            return result_error;

         //m_eventStopped.ResetEvent();

         m_pprebuffer->stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_opensles output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         //m_mmr = translate_alsa(snd_pcm_drain(m_ppcm));


         //if(m_mmr == result_success)
         //{

         m_estate = state_opened;

//         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_pause()
      {

         single_lock sLock(m_pmutex, TRUE);

         ASSERT(m_estate == state_playing);

         if(m_estate != state_playing)
            return result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_opensles output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         //m_mmr = translate_alsa(snd_pcm_pause(m_ppcm, 1));

         ASSERT(m_mmr == result_success);

         if(m_mmr == result_success)
         {
            m_estate = state_paused;
         }

         return m_mmr;

      }

      ::multimedia::e_result wave_out::wave_out_restart()
      {

         single_lock sLock(m_pmutex, TRUE);

         ASSERT(m_estate == state_paused);

         if(m_estate != state_paused)
            return result_error;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_opensles output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         //m_mmr = translate_alsa(snd_pcm_pause(m_ppcm, 0));

         ASSERT(m_mmr == result_success);

         if(m_mmr == result_success)
         {
            m_estate = state_playing;
         }

         return m_mmr;

      }


      imedia_time wave_out::wave_out_get_position_millis()
      {

         single_lock sLock(m_pmutex, TRUE);

         //if(m_ppcm != NULL)
         //{

         //   snd_pcm_sframes_t frames = snd_pcm_avail_update(m_ppcm);

         //   return frames * 1000 / m_pwaveformat->nSamplesPerSec;

         //}
         //else
         //{

         //   return 0;

         //}

         return 0;

      }


      imedia_position wave_out::wave_out_get_position()
      {

         single_lock sLock(m_pmutex, TRUE);

         //if(m_ppcm != NULL)
         //{

         //   snd_pcm_sframes_t frames = snd_pcm_avail_update(m_ppcm);

         //   return frames;

         //}
         //else
         {

            return 0;

         }

      }


      void wave_out::wave_out_on_playback_end()
      {

         ::multimedia::audio::wave_out::wave_out_on_playback_end();

         //wave_out_stop();

         //if(m_pprebuffer->m_pstreameffectOut != NULL)
         //{
         // ::multimedia::iaudio::wave_stream_effect * peffect = m_pprebuffer->m_pstreameffectOut;
         //m_pprebuffer->m_pstreameffectOut = NULL;
         //delete peffect;
         //}

         //m_eventStopped.SetEvent();

         //m_pplayer->OnEvent(::multimedia::audio::wave_player::EventPlaybackEnd);

      }



      //snd_pcm_t * wave_out::wave_out_get_safe_PCM()
      //{

      //   if(this == NULL)
      //      return NULL;

      //   return m_ppcm;

      //}

      void * wave_out::get_os_data()
      {

         //return m_ppcm;
         return NULL;

      }


      void wave_out::wave_out_filled(int iBuffer)
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate != audio::wave_out::state_playing
               && m_estate != audio::wave_out::state_stopping)
         {

            return;

         }

         SLresult slresult = (*bqPlayerBufferQueue)->Enqueue(bqPlayerBufferQueue, wave_out_get_buffer_data(iBuffer), wave_out_get_buffer_size());

         if (slresult == SL_RESULT_SUCCESS)
         {

            m_iBufferedCount++;

         }

      }


      ::multimedia::e_result wave_out::wave_out_start(const imedia_position & position)
      {

         single_lock sLock(m_pmutex, TRUE);

         if(m_estate == state_playing)
            return result_success;

         if(m_estate != state_opened && m_estate != state_stopped)
            return result_error;

         int err = 0;

         output_debug_string("wave_out::wave_out_start");

         //if ((err = snd_pcm_prepare (m_ppcm)) < 0)
         //{

         //   TRACE ("cannot prepare audio interface for use (%s)\n",snd_strerror (err));

         //   return result_error;

         //}

         m_iPlayBuffer = 0;

         m_iBufferedCount = 0;

         //m_evBufferFull.ResetEvent();

         m_mmr = ::multimedia::audio::wave_out::wave_out_start(position);

         if(failed(m_mmr))
            return m_mmr;

         // set the player's state to playing
         SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);
         //DEBUG_SND("SetPlayState=%d", result);
         //ASSERT(!result);

         if (result != SL_RESULT_SUCCESS)
            return ::multimedia::result_error;


         return result_success;

      }


      bool wave_out::on_run_step()
      {

//         return ::multimedia::audio::wave_out::on_run_step();
         return false;

      }


      int wave_out::underrun_recovery(int err)
      {

         //if(verbose)
         //printf("stream recovery\n");

         if(m_pprebuffer->is_eof() || wave_out_get_state() == state_stopping)
         {

            return 0;

         }
         else if (err == -EPIPE)
         {

            //// under-run
            //err = snd_pcm_prepare(m_ppcm);

            //if (err < 0)
            //{

            //   TRACE("Can't recovery from underrun, prepare failed: %s\n", snd_strerror(err));

            //}
            //else if (err == -ESTRPIPE)
            //{

            //   while ((err = snd_pcm_resume(m_ppcm)) == -EAGAIN)
            //   {

            //      sleep(1); /* wait until the suspend flag is released */

            //   }

            //   if (err < 0)
            //   {

            //      err = snd_pcm_prepare(m_ppcm);

            //      if (err < 0)
            //      {

            //         TRACE("Can't recovery from suspend, prepare failed: %s\n", snd_strerror(err));

            //      }

            //   }

            //}

            //return 0;

         }

         return err;

      }


      void wave_out::wave_out_free(int iBuffer)
      {

         ::multimedia::audio::wave_out::wave_out_free(iBuffer);

      }

   } // namespace audio_opensles


} // namespace multimedia








// this callback handler is called every time a buffer finishes playing
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void *context)
{

   multimedia::audio_opensles::wave_out *p = (multimedia::audio_opensles::wave_out *)context;

   SLAndroidSimpleBufferQueueState s;

   ZERO(s);

   (*bq)->GetState(bq, &s);

   //output_debug_string("buffer_index" + ::str::from(s.index % p->m_iBufferCount));

   p->post_message(multimedia::audio::wave_out::message_free, s.index % p->m_iBufferCount);



}