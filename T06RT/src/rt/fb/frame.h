/* FILE NAME  : frame.h
 * PROGRAMMER : BZ6
 * LAST UPDATE: 01.08.2021
 * PURPOSE    : Frame buffer declaration module.
 */

#ifndef __frame_h_
#define __frame_h_

#include <string>
#include <fstream>

#pragma pack(push, 1)
#include <tgahead.h>
#pragma pack(pop)

#include "../../def.h"

/* Project namespace */
namespace zart
{
  /* Frame image class */
  class frame
  {
  public:
    INT W = 0, H = 0;        // Frame size
    DWORD *Pixels = nullptr; // Frame buffer

    /* Default constructor */
    frame( VOID )
    {
    } /* End of 'frame' constructor */
    
    /* Default distructor */
    ~frame( VOID )
    {
      delete[] Pixels;
    } /* End of 'frame' constructor */

    /* Resize frame buffer function
      * ARGUMENTS:
      *   - size of frame buffer:
      *       INT W, H;
      * RETURNS: None.
      */
    VOID Resize( INT W, INT H )
    {
      delete[] Pixels;
      frame::W = W;
      frame::H = H;
      Pixels = new DWORD[W * H];
    } /* End of 'Resize' function */

    /* Put pixel in frame buffer function
     * ARGUMENTS:
     *   - position of pixel:
     *       INT X, Y;
     *   - color of pixel:
     *       DWORD Color;
     * RETURNS: None.
     */
    VOID PutPixel( INT X, INT Y, DWORD Color )
    {
      if (X < 0 || Y < 0 || X >= W || Y >= H)
        return;
      Pixels[Y * W + X] = Color;
    } /* End of 'PutPixel' function */

    /* Draw pixels in display function
     * ARGUMENTS:
     *   - handle descriptor:
     *       HDC hDC;
     *   - position of start:
     *       INT X, Y;
     * RETURNS: None.
     */
    VOID Draw( HDC hDC, INT X, INT Y )
    {
      BITMAPINFOHEADER bih;

      bih.biSize = sizeof(BITMAPINFOHEADER);
      bih.biBitCount = 32;
      bih.biPlanes = 1;
      bih.biWidth = W;
      bih.biHeight = -H;
      bih.biSizeImage = W * H * 4;
      bih.biCompression = BI_RGB;
      bih.biClrUsed = 0;
      bih.biClrImportant = 0;
      bih.biXPelsPerMeter = 30;
      bih.biYPelsPerMeter = 30;

      SetStretchBltMode(hDC, COLORONCOLOR);
      StretchDIBits(hDC, X, Y, W, H, 0, 0, W, H, Pixels, (BITMAPINFO *)&bih, DIB_RGB_COLORS, SRCCOPY);
    } /* End of 'Draw' function */

    /* Save pixels in TGA image function
     * ARGUMENTS:
     *   - file name :
     *       const std::string &FileName;
     * RETURNS: (BOOL) if succed TRUE else FALSE.
     */
    BOOL SaveTGA( const std::string &FileName )
    {
      tgaFILEHEADER fh;
      tgaFILEFOOTER ff;

      fh.IDLength = 0;
      fh.ColorMapType = 0;
      fh.ImageType = 2;
      fh.X = 0;
      fh.Y = 0;
      fh.Width = W;
      fh.Height = H;
      fh.BitsPerPixel = 32;
      fh.ImageDescr = 32;

      ff.DeveloperOffset = 0;
      ff.ExtensionOffset = 0;
      CopyMemory(ff.Signature, TGA_EXT_SIGNATURE, 18);

      std::fstream f("bin/image/" + FileName + ".tga", std::fstream::out | std::fstream::binary);

      if (!f.is_open())
        return FALSE;
      f.write((CHAR *)(&fh), sizeof(fh));
      f.write((CHAR *)(Pixels), W * H * 4);
      f.write((CHAR *)(&ff), sizeof(ff));
      return TRUE;
    } /* End of 'SaveTGA' function */
  }; /* End of 'frame' class */
} /* end of 'zart' namespace */

#endif /* __frame_h_ */

/* END OF 'frame.h' FILE */