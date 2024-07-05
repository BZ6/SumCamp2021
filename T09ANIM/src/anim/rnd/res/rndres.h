/* FILE NAME: rndres.h
 * PROGRAMMER: BZ6
 * DATE: 21.06.2021
 * PURPOSE: 3D animation rendering resourse declaration module.
 */

#ifndef __rndres_h_
#define __rndres_h_

#include "../rnd.h"

/* Maximum elements in string */
#define BZ6_STR_MAX 100

/***
 * Shaders stock functions
 ***/

/* Shaders stock */
#define BZ6_MAX_SHADERS 30

/* Shader representation type */
typedef struct tagbz6SHADER
{
  CHAR Name[BZ6_STR_MAX];
  INT ProgId;
} bz6SHADER;

/* Shader stock array and it size */
extern bz6SHADER BZ6_RndShaders[BZ6_MAX_SHADERS];
extern INT BZ6_RndShadersSize;

/* Shader stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BZ6_RndShadersInit( VOID );

/* Shader stock deinitialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BZ6_RndShadersClose( VOID );

/* Shader add to stock function.
 * ARGUMENTS:
 *   - shader folder prefix:
 *       CHAR *FileNamePrefix;
 * RETURNS:
 *   (INT) shader number in stock.
 */
INT BZ6_RndShaderAdd( CHAR *FileNamePrefix );

/* Shader stock update function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
VOID BZ6_RndShadersUpdate( VOID );

/* Shader stock delete function.
 * ARGUMENTS: 
 *   - program Id:
 *       INT ProgId.
 * RETURNS: None.
 */
VOID BZ6_RndShdDelete( INT ProgId );

/***
 * Textures stock functions
 ***/

/* Texture representation type */
typedef struct tagbz6TEXTURE
{
  CHAR Name[BZ6_STR_MAX]; /* Texture name */
  INT W, H;               /* Texture size in pixels */
  UINT TexId;             /* OpenGL texture Id */ 
} bz6TEXTURE;

/* Textures stock */
#define BZ6_MAX_TEXTURES 300
extern bz6TEXTURE BZ6_RndTextures[BZ6_MAX_TEXTURES]; /* Array of textures */
extern INT BZ6_RndTexturesSize;                      /* Textures array store size */

/* Texture stock initialization function.
 * ARGUMENTS: None.
 * RETURNS: None.
 */
BZ6_RndTexInit( VOID );

/* Texture image add in stock function.
 * ARGUMENTS:
 *   - name of image:
 *       CHAR *Name; 
 *   - size of image:
 *       INT W, H;
 *   - texture data:
 *       VOID *Bits.
 * RETURNS:
 *   (INT) texture number.
 */
INT BZ6_RndTextureAddImg( CHAR *Name, INT W, INT H, VOID *Bits );
INT BZ6_RndTextureAddFromFile( CHAR *FileName );
VOID BZ6_RndTexClose( VOID );

#endif /* __rndres_h_ */

/* END OF 'rndres.h' FILE */