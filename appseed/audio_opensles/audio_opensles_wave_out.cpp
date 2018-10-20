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


      ::multimedia::e_result wave_out::wave_out_open_ex(thread * pthreadCallback, uint32_t uiSamplesPerSec, uint32_t uiChannelCount, uint32_t uiBitsPerSample, ::multimedia::audio::e_purpose epurpose)
      {

         synch_lock sl(m_pmutex);

         if (engineObject != NULL && m_estate != state_initial)
         {

            return result_success;

         }

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
         {

            return ::multimedia::result_error;

         }

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
            {

               speakers = SL_SPEAKER_FRONT_LEFT | SL_SPEAKER_FRONT_RIGHT;

            }
            else
            {

               speakers = SL_SPEAKER_FRONT_CENTER;

            }

            SLDataFormat_PCM format_pcm;

            format_pcm.formatType = SL_DATAFORMAT_PCM;
            format_pcm.numChannels = channels;
            format_pcm.samplesPerSec = sr;
            format_pcm.bitsPerSample = SL_PCMSAMPLEFORMAT_FIXED_16;
            format_pcm.containerSize = SL_PCMSAMPLEFORMAT_FIXED_16;
            format_pcm.channelMask = speakers;
            format_pcm.endianness = SL_BYTEORDER_LITTLEENDIAN;

            SLDataSource audioSrc;

            audioSrc.pLocator = &loc_bufq;
            audioSrc.pFormat = &format_pcm;

            // configure audio sink
            SLDataLocator_OutputMix loc_outmix;

            loc_outmix.locatorType = SL_DATALOCATOR_OUTPUTMIX;
            loc_outmix.outputMix = outputMixObject;

            SLDataSink audioSnk;

            audioSnk.pLocator = &loc_outmix;
            audioSnk.pFormat = NULL;

            // create audio player
            const SLInterfaceID ids1[] = { SL_IID_ANDROIDSIMPLEBUFFERQUEUE, SL_IID_VOLUME };
            const SLboolean req1[] = { SL_BOOLEAN_TRUE, SL_BOOLEAN_TRUE };
            result = (*engineEngine)->CreateAudioPlayer(engineEngine,
                     &(bqPlayerObject), &audioSrc, &audioSnk, 2, ids1, req1);

            output_debug_string("bqPlayerObject="+::str::from((uint_ptr)bqPlayerObject));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

            // realize the player
            result = (*bqPlayerObject)->Realize(bqPlayerObject, SL_BOOLEAN_FALSE);
            output_debug_string("Realize="+::str::from((uint_ptr)result));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

            // get the play interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_PLAY, &(bqPlayerPlay));
            output_debug_string("bqPlayerPlay=" + ::str::from((uint_ptr) bqPlayerPlay));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

            // get the volume interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_VOLUME, &(bqPlayerVolume));
            output_debug_string("bqPlayerVolume=" + ::str::from((uint_ptr) bqPlayerVolume));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

            // get the buffer queue interface
            result = (*bqPlayerObject)->GetInterface(bqPlayerObject, SL_IID_ANDROIDSIMPLEBUFFERQUEUE,
                     &(bqPlayerBufferQueue));
            ::output_debug_string("bqPlayerBufferQueue=" + ::str::from((uint_ptr) bqPlayerBufferQueue));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

            // register callback on the buffer queue
            result = (*bqPlayerBufferQueue)->RegisterCallback(bqPlayerBufferQueue, bqPlayerCallback, this);
            output_debug_string("bqPlayerCallback=" + ::str::from((uint_ptr)bqPlayerCallback));

            if (result != SL_RESULT_SUCCESS)
            {

               goto end_openaudio;

            }

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
            m_iBufferSize = uiBufferSize;
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

         synch_lock sl(m_pmutex);

         if(m_estate == state_playing)
         {

            wave_out_stop();

         }

         if (m_estate != state_opened)
         {

            return result_success;

         }

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

         synch_lock sl(m_pmutex);

         if (m_estate != state_playing && m_estate != state_paused)
         {

            return result_error;

         }

         m_pprebuffer->stop();

         m_estate = state_stopping;

         // waveOutReset
         // The waveOutReset function stops playback on the given
         // waveform-audio_opensles output device and resets the current position
         // to zero. All pending playback buffers are marked as done and
         // returned to the application.
         //m_mmr = translate_alsa(snd_pcm_drain(m_ppcm));

         m_estate = state_opened;

         return m_mmr;

      }


      ::multimedia::e_result wave_out::wave_out_pause()
      {

         synch_lock sl(m_pmutex);

         ASSERT(m_estate == state_playing);

         if (m_estate != state_playing)
         {

            return result_error;

         }

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

         synch_lock sl(m_pmutex);

         ASSERT(m_estate == state_paused);

         if (m_estate != state_paused)
         {

            return result_error;

         }

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

         synch_lock sl(m_pmutex);

         SLmillisecond ms = 0;

         SLresult result = (*bqPlayerPlay)->GetPosition(bqPlayerPlay, &ms);

         if (result == SL_RESULT_SUCCESS)
         {

            if (ms >= m_iLastOpenSLESPos)
            {

               m_iLastOpenSLESPos = ms;

               return m_iLastOpenSLESPos;

            }

         }

         return (m_iPos * (u64)1000) / ((u64)m_pprebuffer->get_frame_size() * (u64)m_pwaveformat->nSamplesPerSec);

      }


      imedia_position wave_out::wave_out_get_position()
      {

         return (wave_out_get_position_millis() * (u64)m_pwaveformat->nSamplesPerSec) / ((u64) 1000);

      }


      void wave_out::wave_out_on_playback_end()
      {

         ::multimedia::audio::wave_out::wave_out_on_playback_end();

      }


      void * wave_out::get_os_data()
      {

         return NULL;

      }


      void wave_out::wave_out_filled(int iBuffer)
      {

         synch_lock sl(m_pmutex);

         if(m_estate != audio::wave_out::state_playing && m_estate != audio::wave_out::state_stopping)
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

         synch_lock sl(m_pmutex);

         if (m_estate == state_playing)
         {

            return result_success;

         }

         if (m_estate != state_opened && m_estate != state_stopped)
         {

            return result_error;

         }

         int err = 0;

         output_debug_string("wave_out::wave_out_start");

         m_iPlayBuffer = 0;

         m_iBufferedCount = 0;

         m_iPos = 0;

         m_iLastOpenSLESPos = 0;

         m_mmr = ::multimedia::audio::wave_out::wave_out_start(position);

         if (failed(m_mmr))
         {

            return m_mmr;

         }

         // set the player's state to playing
         SLresult result = (*bqPlayerPlay)->SetPlayState(bqPlayerPlay, SL_PLAYSTATE_PLAYING);

         if (result != SL_RESULT_SUCCESS)
         {

            return ::multimedia::result_error;

         }

         return result_success;

      }


      bool wave_out::on_run_step()
      {

         return false;

      }


      int wave_out::underrun_recovery(int err)
      {

         if(m_pprebuffer->is_eof() || wave_out_get_state() == state_stopping)
         {

            return 0;

         }
         else if (err == -EPIPE)
         {

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
void bqPlayerCallback(SLAndroidSimpleBufferQueueItf bq, void * pcontext)
{

   multimedia::audio_opensles::wave_out *p = (multimedia::audio_opensles::wave_out *) pcontext;

   SLAndroidSimpleBufferQueueState s;

   ZERO(s);

   (*bq)->GetState(bq, &s);

   p->m_iPos += p->m_iBufferSize;

   p->post_message(multimedia::audio::wave_out::message_free, (s.index - 1) % p->m_iBufferCount);

}



