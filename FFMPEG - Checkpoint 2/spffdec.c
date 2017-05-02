/*
 * SPFF image format decoder
 * Copyright (c) 2017 Sam Nelson, Skylar Shyu
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <inttypes.h>

#include "avcodec.h"
#include "bytestream.h"
#include "spff.h"
#include "internal.h"
#include "msrledec.h"

static int spff_decode_frame(AVCodecContext *avctx,
                            void *data, int *got_frame,
                            AVPacket *avpkt)
{
    const uint8_t *buf = avpkt->data;
    int buf_size       = avpkt->size;
    AVFrame *photo         = data;
    unsigned int fsize, hsize;
    int width, height;
    unsigned int depth;
    int i, n, linesize;
    uint8_t *ptr;
    const uint8_t *buf0 = buf;

    /*
     * Verifies the type of file is .SPFF-type via matching file-type signature
     * before even attempting to decode ANY pixels
     */
    if (bytestream_get_byte(&buf) != 'S' || // ensures the 's' in '.SPFF' is present
        bytestream_get_byte(&buf) != 'P' || // ...same for the 'p'
        bytestream_get_byte(&buf) != 'F' || // ...and the 'f'
        bytestream_get_byte(&buf) != 'F')   // ...if we get 'SPFF' at this point,
                                            // we're good
      {
        av_log(avctx, AV_LOG_ERROR, "Characters do not align!"); // stop decode attempt and notify the reason in AV_LOG
        return AVERROR_INVALIDDATA;
    }

    fsize = bytestream_get_le32(&buf);    //set the file size equal to the buffer size of the encoded image.
 
    if (buf_size < fsize) //if, for whatever reason, the fsize is smaller than the buffer size...
    {
        av_log(avctx, AV_LOG_ERROR, "not enough data (%d < %u), trying to decode anyway\n",
               buf_size, fsize);    // warn the user in the AV_LOG
        fsize = buf_size;           // but still go ahead with the decode attempt
	                            // and make the file size equal to the buffer size
    }

    /*
     * Getting more info about the .spff image we're trying to decode
     * via the bytestream
     */
    hsize  = bytestream_get_le32(&buf);    // header size
    width  = bytestream_get_le32(&buf);    // the width 
    height = bytestream_get_le32(&buf);    // the height
    depth = bytestream_get_le16(&buf);     // depth is bits per pixel

    /*
     * Assigning values to the decoded image fields (in AVCodec Context)
     */
    avctx->width  = width;                         // assign width                    
    avctx->height = height > 0 ? height : -height; // assign height
    avctx->pix_fmt = AV_PIX_FMT_RGB8;              // assign pixel format

    ff_get_buffer(avctx, photo, 0);  // getting the buffer for a single frame   

    /*
     * Some picture type information
     */
    photo->pict_type = AV_PICTURE_TYPE_I;  // spff is decoded as an I-type
    photo->key_frame = 1;                  // key-frame (if used as an animation of 
                                           // sorts) of value 1.

    buf   = buf0 + hsize;    // set the buffer size 

    n = ((avctx->width * depth + 31) / 8) & ~3;  // Sets the # of bytes per row; rounds the value up to 
                                                                                    // the nearest multiple of 4

    ptr = photo->data[0] + (avctx->height - 1) * photo->linesize[0];    // points to last row & column of data
    linesize = -photo->linesize[0];    // sets the linesize for decoding 

    /*
     * Where decode dreams come true <3 ~~ line by line in a for loop, top to bottom
     */
    for (i = 0; i < avctx->height; i++) {
      memcpy(ptr, buf, n);    // copies contents of pointer into buffer, n-bytes worth
      buf += n;               // increase the buffer size by n-bytes 
      ptr += linesize;        // drop the row pointer down by a line
    }

    *got_frame = 1;    // 1 SPFF image, 1 frame.

    return buf_size;   // Done decoding
}

/*
 * .spff decoder struct
 */
AVCodec ff_spff_decoder = {
  .name           = "spff", // the ending of the SPFF filetype
  .long_name      = NULL_IF_CONFIG_SMALL("SPFF image (a project for CS 3505)"), // description
  .type           = AVMEDIA_TYPE_VIDEO, // SPFF is a visual codec, NOT audio
  .id             = AV_CODEC_ID_SPFF,   // FFMPEG identifies the spff format 
  .decode         = spff_decode_frame,  // The method to call when decoding
  .capabilities   = AV_CODEC_CAP_DR1,   // ?? Profit.
};
