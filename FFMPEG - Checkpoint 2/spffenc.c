/*
 * SPFF image format encoder
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

/*
 * Including necessary headers
 */
#include "libavutil/imgutils.h"
#include "libavutil/avassert.h"
#include "avcodec.h"
#include "bytestream.h"
#include "spff.h"
#include "internal.h"

/*
 * spff_encode_init: Initializes the encoding process: the process to 
 * create a .spff-type image.
 */
static av_cold int spff_encode_init(AVCodecContext *avctx){
    avctx->bits_per_coded_sample = 8;   //sets the number of bits/pixel to 8
    return 0;
}

/*
 * spff_encode_frame: Initializes encoding for a frame. Since .spff is a 1-frame  
 * image-type, this will only run once for each .spff image encoding
 */
static int spff_encode_frame(AVCodecContext *avctx, AVPacket *pkt,
                            const AVFrame *pict, int *got_packet)
{
    const AVFrame * const photo = pict; 
    int bytes_in_image, bytes_per_row, buffer_size, i, header_size;
    const uint32_t *pal = NULL;
    uint32_t palette256[256];
    int pad_bytes_per_row, pal_entries = 256;
    int bit_count = avctx->bits_per_coded_sample;
    uint8_t *row_pointer, *buf;


/*
 * Disables deprecation warning when setting the picture-type of the spff image,
 * then re-enables the warning. 
 */ 
#if FF_API_CODED_FRAME
FF_DISABLE_DEPRECATION_WARNINGS                         // Disables deprication warnings.
    avctx->coded_frame->pict_type = AV_PICTURE_TYPE_I;  // Identifies the spff image to be encoded as a I-type (key-frame)
    avctx->coded_frame->key_frame = 1;                  // Gives the spff image a key_frame value of 1, if it's ever to be
                                                        // in an animation of sorts. 
FF_ENABLE_DEPRECATION_WARNINGS                          // Re-enables deprecation warning 
#endif

    switch (avctx->pix_fmt) {
    case AV_PIX_FMT_RGB8:
        avpriv_set_systematic_pal2(palette256, avctx->pix_fmt); // assigns palette based off of the pixel format
        pal = palette256;                                       // make an iterable copy
        break;
    }

    /*
     * Image Calculations
     */
    bytes_per_row = ((int64_t)avctx->width * (int64_t)bit_count + 7LL) >> 3LL;      // Sets the # of bytes per row; rounds the value up to 
                                                                                    // the nearest multiple of 4.LL usage ensures no overflow.
    pad_bytes_per_row = (4 - bytes_per_row) & 3;                                    // How many bytes were added in the process of rounding up
    bytes_in_image = avctx->height * (bytes_per_row + pad_bytes_per_row);           // Total byte size of the image

    // STRUCTURE.field refer to the MSVC documentation for BITMAPFILEHEADER
    // and related pages. It's what SPFF_FILEHEADER is based off of.

    header_size = SIZE_SPFF_FILEHEADER + SIZE_SPFF_INFOHEADER + (pal_entries << 2);   //sets the header_size
    buffer_size = bytes_in_image + header_size;                                         //sets the buffer_size

    ff_alloc_packet2(avctx, pkt, buffer_size, 0);     // allocates new buffer, setting minimum required packet size to be 0

    buf = pkt->data;                                  // buf iterates through the data array as we read the contents
                                                      // of the array through the bytestream

    bytestream_put_byte(&buf, 'S');                   // SPFF_FILEHEADER.bfType
    bytestream_put_byte(&buf, 'P');                   // do.
    bytestream_put_byte(&buf, 'F');                   // do.
    bytestream_put_byte(&buf, 'F');                   // do.
    bytestream_put_le32(&buf, buffer_size);           // SPFF_FILEHEADER.bfSize
    bytestream_put_le32(&buf, header_size);           // SPFF_FILEHEADER.bfOffset
    bytestream_put_le32(&buf, avctx->width);          // SPFF_INFOHEADER.biWidth
    bytestream_put_le32(&buf, avctx->height);         // SPFF_INFOHEADER.biHeight
    bytestream_put_le16(&buf, bit_count);             // SPFF_INFOHEADER.biBitCount

    for (i = 0; i < pal_entries; i++)                 // iterates through all the colours 
        bytestream_put_le32(&buf, pal[i] & 0xFFFFFF); // masks them against the colour white

    // SPFF files are (also shockingly) bottom-to-top so we start from the end...
    row_pointer = photo->data[0] + (avctx->height - 1) * photo->linesize[0];    // Points to the last row & column of data
    buf = pkt->data + header_size;                                              // Initializes buffer pointer after header 
                                                                                // data for image to be written


    /*
     * Encodes the image, row by row, from the bottom to top.
     */                                                                            
    for(i = 0; i < avctx->height; i++) {
        memcpy(buf, row_pointer, bytes_per_row);    // copies the buffer's contents to where the row is pointing
        buf += bytes_per_row;                       // add the row of next bytes to the buffer

        memset(buf, 0, pad_bytes_per_row);          // copies the contents to the beginning, offset by the padding
        buf += pad_bytes_per_row;                   // add the padding to the buffer
        row_pointer -= photo->linesize[0];          // ... and go back
    }

    pkt->flags |= AV_PKT_FLAG_KEY;
    *got_packet = 1;
    return 0;
}

AVCodec ff_spff_encoder = {
    .name           = "spff",
    .long_name      = NULL_IF_CONFIG_SMALL("SPFF image (a project for CS 3505)"),
    .type           = AVMEDIA_TYPE_VIDEO,
    .id             = AV_CODEC_ID_SPFF,
    .init           = spff_encode_init,
    .encode2        = spff_encode_frame,
    .pix_fmts       = (const enum AVPixelFormat[]){
        AV_PIX_FMT_RGB8,
        AV_PIX_FMT_NONE
    },
};
