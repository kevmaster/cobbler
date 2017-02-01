#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_BITMAP_H
#include <SDL2/SDL.h>
#include FT_GLYPH_H


typedef struct  TGlyph_
{
  FT_UInt    index;  /* glyph index                  */
  FT_Vector  pos;    /* glyph origin on the baseline */
  FT_Glyph   image;  /* glyph image                  */

} TGlyph, *PGlyph;

int initText();
int changeSize(int size);
//zeroes for height and width default to textsize
SDL_Surface * renderText(const char* text, int imageHeight, int imageWidth, unsigned int r, unsigned int g, unsigned int b);