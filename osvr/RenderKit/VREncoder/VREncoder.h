/********************************************************************
Copyright 2017 Intel Corp. All Rights Reserved.

Author   :    Wenyi Tang
Email    :    wenyi.tang@intel.com
Created  :    Feb. 15th, 2017
Mod      :    Date      Author

A fast, low latency media encoder for vr applications.
Derived from Intel MediaSDK Samples:
https://github.com/Intel-Media-SDK/samples
********************************************************************/
#ifndef _VRENCODER_H
#define _VRENCODER_H

#include <stdint.h>

#ifndef VREncoder_EXPORTS
#define VRDLLAPI __declspec(dllimport)
#else
#define VRDLLAPI __declspec(dllexport)
#endif

#ifndef MAKE_FOURCC
#define MAKE_FOURCC(A,B,C,D) ((((int)A))+(((int)B)<<8)+(((int)C)<<16)+(((int)D)<<24))
#endif

namespace vrenc {

enum CODECID {
  JPEG = MAKE_FOURCC('J', 'P', 'E', 'G'),
  AVC  = MAKE_FOURCC('A', 'V', 'C', ' ')
};

enum FOURCC {
  NV12 = MAKE_FOURCC('N', 'V', '1', '2'),
  RGB4 = MAKE_FOURCC('R', 'G', 'B', '4'),
  BGR4 = MAKE_FOURCC('B', 'G', 'R', '4'),  // not supported
  YUY2 = MAKE_FOURCC('Y', 'U', 'Y', '2')   // not supported
};

enum Rotate { ROTATE_HOLD = 0, ROTATE_RQUAT = 90, ROTATE_UDOWN = 180, ROTATE_LQUAT = 270 };
enum Mirror { MIRROR_NONE = 0, MIRROR_HORIZONTAL = 1, MIRROR_VERTICAL = 2 };

struct LayerDesc {
  union {
    uint16_t jpegQuality;  ///< For JPEG encode only. Valid from 1 to 100.
    uint16_t targetKbps;   ///< For AVC only.
  };
  uint16_t srcWidth, srcHeight, dstWidth, dstHeight;  ///< size for in/out
  uint16_t cropX, cropY, cropW, cropH;  ///< crop for input
  uint8_t  mirror;       ///< Experimental, mirror the output images. As enum #Mirror
  uint8_t  rotate;       ///< Experimental, rotate the output images. As enum #Rotate
};

struct EncoderDesc {
  uint32_t codec;        ///< #CODECID
  uint32_t colorFormat;  ///< #FOURCC
  uint16_t multiView;    ///< For AVC encode only. Use as boolean.
  void    *renderer;     ///< The native handle for render device. (ID3D11Device*/vaDisplay)
  LayerDesc *descs;      ///< layer description array
  uint16_t *weights;     ///< layer weight
  uint32_t numLayers;    ///< Number of layers
};

}

extern "C" {
  /** Init encoder.
  * \brief Initialize encoder pipeline.
  * \param [in] desc:
                The description for initialization. See \ref EncoderDesc.
  * \return 0 if succeeded.
  */
  int VRDLLAPI VREnc_Init(vrenc::EncoderDesc &desc);

  /**
  * \brief Get d3d11 shared memory handle.
  * \param [out] p:
                 a ID3D11Texture2D array in size of 2.
  * \return 0 if succeeded.
  */
  int VRDLLAPI VREnc_GetSharedTexture(void ** p, int num);

  /**
  * Start to encode a frame.
  * \param [out] left/right:
  *              output data pointer.
  * \param [out] lenleft/lenright:
  *              data length.
  * \return 0 if succeeded.
  */
  int VRDLLAPI VREnc_Encode(void **data, int *lens, int *arraySize);

  /**
  * \brief: Dump encoded images to files
  * \param [in] path:
                the path to save the file. For example "./"
  * \note: will dump every frame after calling this function.
           if you want not to dump any more, call VREnc_DumpDisable().
  */
  void VRDLLAPI VREnc_DumpEnable(const char *path);

  void VRDLLAPI VREnc_DumpDisable();

  // Shutdown the encoder pipeline.
  void VRDLLAPI VREnc_Close();
}  // - extern "C"

#endif  // !_VRENCODER_H
