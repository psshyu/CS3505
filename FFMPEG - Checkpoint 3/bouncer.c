
/*
 * FFMPEG part 3 | SPRING 2017
 * Brought to you by Sam Nelson & Skylar Shyu
 */
 
/*
 * FOR YOUR REFERENCING PLEASURE
 *   Dranger: dranger.com/ffmpeg/tutorial01.html
 *   swscaler deprecation fix: stackoverflow.com/questions/23067722/swscaler-warning-deprecated-pixel-format-use
 *   codec deprecation fix: ffmpeg.org/pipermail/libav-user/2016-October/009801.html
 *   all other deprecations: libav.org/documentation/doxygen/master/deprecated.html
 *   Sound: stackoverflow.com/questions/42242140/encode-wav-file-using-ffmpeg-in-objective-c-or-c
 *
 * In our documentation below, if we used someone else's source, we will
 * make note of it. 
 *
 * Example:
 * // This line does blah - source: sws deprecation fix
 *
 */

#include <math.h>

#ifndef INT64_C
#define INT64_C(c) (c ## LL)
#define UINT64_C(c) (c ## ULL)
#endif

#include "../ffmpeg/libavcodec/avcodec.h"
#include "../ffmpeg/libavformat/avformat.h"
#include "../ffmpeg/libswscale/swscale.h"
#include "../ffmpeg/libavutil/avutil.h"
#include "../ffmpeg/libavutil/imgutils.h"

/*
 * Takes the input and returns its square
 */
int square(int i)
{
  return i*i;
}

int main(int argc, char *argv[]) {

  /******************
   * BEGIN DECODING *
   ******************/
  AVFormatContext *pFormatCtx = NULL;
  AVCodecContext  *pCodecCtx = NULL;
  AVCodec         *pCodec = NULL;
  AVCodec         *spffCodec = NULL;
  AVFrame         *pFrame = NULL; 
  AVFrame         *pFrameRGB = NULL;
  AVPacket        packet;
  AVPacket        *spffpacket = av_packet_alloc();
  int             numBytes;
  uint8_t         *buffer = NULL;
  struct SwsContext      *sws_ctx = NULL;
  
  // ensure command line argument for image
  if(argc < 2) {
    printf("Please provide an jpeg file\n");
    return -1;
  }
  
  av_register_all();      // Register all formats... - source: Dranger
  avcodec_register_all(); // ...AND codecs
    
  avformat_open_input(&pFormatCtx, argv[1], NULL, NULL); // Open video file - source: Dranger
  avformat_find_stream_info(pFormatCtx, NULL);           // Retrieve stream information - source: Dranger

  pCodec = avcodec_find_decoder(pFormatCtx->streams[0]->codecpar->codec_id);  // Find and set decoder - source: codec deprecation fix
  pCodecCtx = avcodec_alloc_context3(pCodec);  // Find and set the codec context - source: codec deprecation fix

  // Set the image format context - source: codec deprecation fix
  avcodec_parameters_to_context(pCodecCtx,
                                pFormatCtx->streams[0]->codecpar);  

  avcodec_open2(pCodecCtx, pCodec, NULL); // Open codec - source: Dranger
  pFrame = av_frame_alloc();              // Allocate video frame - source: Dranger
  pFrameRGB = av_frame_alloc();           // Allocate an AVFrame structure - source: Dranger

  // Determine required buffer size - source: all other deprecations
  numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 
                                      pCodecCtx->width, 
				      pCodecCtx->height, 
				      1
    );
  buffer = (uint8_t *) av_malloc(numBytes*sizeof(uint8_t)+256); // allocate buffer - source: Dranger

  /*
   * This switch block ensures that we're not using a deprecated
   * pixel format when handling the jpg input. 
   * source: sws deprecation fix
   */
  switch (pCodecCtx->pix_fmt) {
  case AV_PIX_FMT_YUVJ420P :
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV420P;
    break;
  case AV_PIX_FMT_YUVJ422P  :
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV422P;
    break;
  case AV_PIX_FMT_YUVJ444P   :
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV444P;
    break;
  case AV_PIX_FMT_YUVJ440P :
    pCodecCtx->pix_fmt = AV_PIX_FMT_YUV440P;
  default:
    break;
  }

  // set up for image reconfiguration - source: Dranger
  sws_ctx = sws_getContext(
        pCodecCtx->width,
        pCodecCtx->height,
        pCodecCtx->pix_fmt,
        pCodecCtx->width,
        pCodecCtx->height,
        AV_PIX_FMT_RGB24,
        SWS_BILINEAR,
        NULL,
        NULL,
        NULL
    );
  
  // Assign appropriate parts of buffer to image planes in pFrameRGB - source: all other deprecations
  av_image_fill_arrays(pFrameRGB->data, 
                       pFrameRGB->linesize, 
                       buffer, 
                       AV_PIX_FMT_RGB24, 
                       pCodecCtx->width, 
                       pCodecCtx->height, 1
    );
  
  
  av_read_frame(pFormatCtx, &packet); // Read frames and save  - source: Dranger

  // Decode video frames - source: all other deprecations
  avcodec_send_packet(pCodecCtx, &packet);
  avcodec_receive_frame(pCodecCtx, pFrame);
  
  //use image source - source: Dranger
  sws_scale(sws_ctx,
            (uint8_t const * const *)pFrame->data,
            pFrame->linesize,
            0,
            pCodecCtx->height,
            pFrameRGB->data,
            pFrameRGB->linesize
  ); 
  
  sws_freeContext(sws_ctx); //free the sws_ctx - source: Dranger



  /******************
   * BEGIN ENCODING *
   ******************/
  // 3 lines below - source: Dranger 
  int height = pCodecCtx->height; 
  int width = pCodecCtx->width;   
  int i;
  
  // 7 lines below - source: Sound
  AVCodec *aCodec;
  AVCodecContext *aCodecCtx = NULL;
  AVFrame *aFrame;
  AVPacket *Audiopkt = av_packet_alloc();
  int Audio_j, Audio_k, Audio_buffer_size;
  uint16_t *Audio_samples;
  float t, tincr;
  
  aCodec = avcodec_find_encoder(AV_CODEC_ID_WAVPACK); // find the wav encoder - source: Sound
  aCodecCtx = avcodec_alloc_context3(aCodec);         // set codec context for the audio - source: Sound

  /* 
   * Set audio context settings - source: Sound
   */
  aCodecCtx->bit_rate       = 64000;
  aCodecCtx->sample_fmt     = AV_SAMPLE_FMT_S16P;
  aCodecCtx->sample_rate    = 44100;
  aCodecCtx->channel_layout = AV_CH_LAYOUT_STEREO;
  aCodecCtx->channels       = av_get_channel_layout_nb_channels(aCodecCtx->channel_layout);

  avcodec_open2(aCodecCtx, aCodec, NULL); //open audio context - source: Sound
  aFrame = av_frame_alloc(); // allocate a frame for audio encoding - source: Sound

  /*
   * Set some frame settings for audio - source: Sound
   */
  aFrame->nb_samples     = aCodecCtx->frame_size;
  aFrame->format         = aCodecCtx->sample_fmt;
  aFrame->channel_layout = aCodecCtx->channel_layout;
  
  // Set audio buffer size - source: Sound
  Audio_buffer_size = av_samples_get_buffer_size(NULL, 
                                                 aCodecCtx->channels, 
                                                 aCodecCtx->frame_size, 
                                                 aCodecCtx->sample_fmt, 
                                                 0);

  Audio_samples = av_malloc(Audio_buffer_size); // allocate memory for audio - source: Sound
  
  // setup the data pointers in the audio frame - source: Sound
  avcodec_fill_audio_frame(aFrame, 
                           aCodecCtx->channels, 
                           aCodecCtx->sample_fmt, 
                           (const uint8_t*)Audio_samples, 
                           Audio_buffer_size, 
                           0
  );
     
  char Audiofilename[32]; // create character array for audio
  FILE *aFile;            // create file to write audio - source: Sound

  // redirects the output to Audiofilename - source: Sound
  snprintf(Audiofilename, 
           sizeof(Audiofilename), 
           "sound.wav"
  );
 
  aFile = fopen(Audiofilename, "wb"); // open the file as a writeback - source: Sound

  /* encode a single tone sound */
  t = 0;
  tincr = 2 * M_PI * 440.0 / aCodecCtx->sample_rate;
  
  int frameSize   = av_get_audio_frame_duration(aCodecCtx, 10); // set frame size- source: Sound

  /***************************** 
   * Begin creating 300 frames *
   *****************************/
   
  for(i = 0; i < 300; i++) {
    // Allocate an AVFrame structure.. 
    AVFrame * temp;           
    temp = av_frame_alloc();
    
    // ..and set up height, width, and image format appropriately  
    temp->height = height;
    temp->width = width;
    temp->format = AV_PIX_FMT_RGB24;
     
    // Get buffer size for image... - source: all other deprecations
    numBytes = av_image_get_buffer_size(AV_PIX_FMT_RGB24, 
                                        pCodecCtx->width, 
                                        pCodecCtx->height, 
                                        1
      );

    // ..then allocate buffer for image 
    buffer = (uint8_t *) av_malloc(numBytes*sizeof(uint8_t));
    
    // set up for image reconfiguration
    sws_ctx = sws_getContext(pCodecCtx->width,
                             pCodecCtx->height,
                             AV_PIX_FMT_RGB24,
                             pCodecCtx->width,
                             pCodecCtx->height,
                             AV_PIX_FMT_RGB24,
                             SWS_BILINEAR,
                             NULL,
                             NULL,
                             NULL
      );
      
    // Assign appropriate parts of buffer to image planes in temp - source: all other deprecations
    av_image_fill_arrays(temp->data, 
                         temp->linesize, 
                         buffer, 
                         AV_PIX_FMT_RGB24, 
                         width, 
                         height, 
                         1
    );
    // execute scaling of the image with the parameters of sws_ctx - source: Dranger
    sws_scale(sws_ctx, 
              (uint8_t const * const *)pFrameRGB->data, 
              pFrameRGB->linesize, 
              0, 
              height,
              temp->data,
              temp->linesize
      );
    
    sws_freeContext(sws_ctx);  //free the sws_context - source: Dranger
    
    /******************* 
     * BEGIN DRAW BALL *
     *******************/
    int centery = height/2+(int)(sin((double)i/(height/40))*(height/4)); // calculate the y-pos of the center of the ball
    int centerx = width/2*3;                                             // calculate the x-pos of the center of the ball
    int radius = (int)(fmin((double)height,(double)width))/10;           // calculate the radius of the ball
    
    int shadx = centerx+(radius/4); // calculate the x-pos of the center of the shine on the ball
    int shady = centery-(radius/4); // calculate the y-pos of the center of the shine on the ball
    int shadr = radius*(3.0/4.0);   //calculate the radius of the center of the shine on the ball
    int m; //loop counter below
    
    // loop through all pixels to and examine their location
    for(m = 0; m < height*width*3; m += 3) {
      int tempy = m/temp->linesize[0];
      int tempx = m-tempy*temp->linesize[0];
      
      //check to see if the pixel is in the area where we want the ball to be
      if(sqrt(square(tempx/3-centerx/3)+square(tempy-centery))<radius){ //if it is, attempt draw ball
 
        // Set the RGB values of the ball for that particular pixel
        temp->data[0][m] = 0;     // R
        temp->data[0][m+1] = 255; // G
        temp->data[0][m+2] = 0;   // B
 
        //check to see if the pixel is in the area where we want the shine on the ball to be
        if(sqrt(square(tempx/3-shadx/3)+square(tempy-shady)) < shadr){ //if it is, attempt to draw shine
	  /* 
           * prop is a proportional shading double used to add a to r,g,or b values to make the color lighter. 
           * the closer to the shading raidus, shar the lighter the color will be and the higher the value will
           * be addeded. Currently we start with a green ball and so we add to the B and R values. 
           */
          double prop=250-250*(sqrt(square(tempx/3-shadx/3)+square(tempy-shady))/shadr);

          // Set the RGB values of the ball's shine for that particular pixel
          temp->data[0][m] = 0 + prop;    // R
          temp->data[0][m+1] = 255;       // G
          temp->data[0][m+2] = 0 + prop;  // B
          }
        }
      }
    
    spffpacket->size = 0;     // set spff packet size
    spffpacket->data = NULL;  // wipe data of spff packet 
    spffCodec = avcodec_find_encoder(AV_CODEC_ID_SPFF);  // find spff codec
    pCodecCtx->codec = spffCodec;                        // set the codec to spff 
    
    //encode the spff image! - source: Dranger
    avcodec_send_frame(pCodecCtx, temp);     
    avcodec_receive_packet(pCodecCtx, spffpacket);

    // save files frame000.spff - source: Dranger
    char filename[32]; // create character array for spff image
    FILE *pFile;       // create file to store spff image
	
    // redirects output to filename - source: Dranger
    snprintf(filename, 
             sizeof(filename), 
             "frame%03d.spff", 
             i
      );  
    pFile = fopen(filename, "wb");  //open the file with writeback capabilities - source: Dranger
    fwrite(spffpacket->data, 1, spffpacket->size, pFile); // write spff packet to file - source: Dranger
    fclose(pFile); // close the written file - source: Dranger
    av_free(temp); // free up the temp frame - source: Dranger
    
    av_init_packet(Audiopkt); // initialize packet for audio - source: Sound
    Audiopkt->data = NULL; // packet data will be allocated by the encoder - source: Sound
    Audiopkt->size = 0;    // set packet size to 0 - source: Sound
    
    // Encoding audio tone - source: Sound
    for (Audio_j = 0; Audio_j < frameSize; Audio_j++) {
      Audio_samples[2*Audio_j] = (int)(sin(centery*t) * 10000 );

      for (Audio_k = 1; Audio_k < aCodecCtx->channels; Audio_k++)
        Audio_samples[2*Audio_j + Audio_k] = Audio_samples[2*Audio_j];
        
      t += tincr;
      }

      
      aCodec = avcodec_find_encoder(AV_CODEC_ID_WAVPACK); // find wav codec - source: Sound
      aCodecCtx->codec = aCodec;                          // set wav codec context - source: Sound
      
      // Encode the sound - source: Sound/all other deprecations
      avcodec_send_frame(aCodecCtx, aFrame);
      avcodec_receive_packet(aCodecCtx, Audiopkt);

      //write the contents of the audio packet to the audiofile
      fwrite(Audiopkt->data, 
             1, 
             Audiopkt->size, 
             aFile
        );  

    }

  fclose(aFile);    // close audio file - source: Sound
  av_free(aFrame);  // free the audio frame - source: Sound

  av_packet_unref(Audiopkt);   // free the audio packet - source: all other deprecations
  av_freep(&Audio_samples);    // free the audio buffer - source: Sound
  avcodec_close(aCodecCtx);    // close the audio codec context - source: Dranger
  av_free(aCodecCtx);          // free the audio codec context - source: Dranger

  av_packet_unref(&packet);     // free the image frame (the decoded jpg) - source: all other deprecations
  av_packet_unref(spffpacket);  // free the image frame (the one to generate the spffs) - source: all other deprecations

  av_free(buffer);     // free the image buffer - source: Dranger
  av_free(pFrameRGB);  // free the image frame (RGB24 spff one) - source: Dranger
  av_free(pFrame);     // Free the jpg frame - source: Dranger
  
  avcodec_close(pCodecCtx);           // Close the image codec - source: Dranger
  avformat_close_input(&pFormatCtx);  // Close the image codec context - source: Dranger
  
  return 0;  //close the program - source: Dranger
}
