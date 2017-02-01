#include "text.h"
#include <stdio.h>
#define MAX_GLYPHS 50
//dpi variables
float hdpi;
float vdpi;
FT_Face face;
FT_Library library;
FT_GlyphSlot  slot;  /* a small shortcut */
FT_UInt       glyph_index;
FT_Bool       use_kerning;

TGlyph        glyphs[MAX_GLYPHS];  /* glyphs table */
PGlyph        glyph;               /* current glyph in table */
FT_UInt       num_glyphs;

unsigned char * renderHelper(int angle, int my_target_height, int my_target_width, unsigned int r, unsigned int g, unsigned int b);
void my_draw_bitmap(FT_Bitmap bitmap, FT_Int left, FT_Int top, unsigned char* pixels, int height, int width, unsigned int r, unsigned int g, unsigned int b);
void  compute_string_bbox( FT_BBox  *abbox );

int initText(){
	//init library
	int error = FT_Init_FreeType(&library);
	if(error){
		printf("couldnt init library\n");
		return error;
	}
	//get dpi values
	error = !SDL_GetDisplayDPI(0,NULL,&hdpi,&vdpi);
	if(error){
		printf("couldnt get dpi\n");
		printf("%s\n",SDL_GetError());
		hdpi = 200.0;
		vdpi = 200.0;
		//return error;
	}
	error = FT_New_Face(library,"fonts/Phosphate.ttc",0,&face);
	if(error){
		printf("couldnt load font\n");
		return error;
	}
	// printf("font has %x glyphs",face->charmap->encoding);
	error = FT_Set_Char_Size(face, 0, 50*64, hdpi,vdpi);
	slot = face->glyph;
	//printf("a is :%i",FT_Get_Char_Index( face, 'h' ));
	printf("color font?: %ld\n",FT_HAS_COLOR(face));

	return 0;
} 

int changeSize(int size){
	return FT_Set_Char_Size(face, 0, size*64,hdpi,vdpi);
}


SDL_Surface * renderText(const char* text, int imageHeight, int imageWidth, unsigned int r, unsigned int g, unsigned int b){
	int           pen_x, pen_y, n;
	pen_x = 0;   /* start at (0,0) */
	pen_y = 0;

	num_glyphs  = 0;
	use_kerning = FT_HAS_KERNING( face );
	FT_UInt previous    = 0;

	glyph = glyphs;
	int num_chars = strlen(text);
	for ( n = 0; n < num_chars; n++ )
	{
		unsigned long character = text[n];
		// printf("text[n] is: %lx\n",character);
 		glyph->index = FT_Get_Char_Index( face, character );
 		if(glyph->index == 0){
 			printf("glyph index is 0\n");
 		}

  		if ( use_kerning && previous && glyph->index )
  		{		
    		FT_Vector  delta;
    		FT_Get_Kerning( face, previous, glyph->index, FT_KERNING_DEFAULT, &delta );
    		pen_x += delta.x;
  		}

		/* store current pen position */
		//printf("pen_y = %i\n",pen_y);
		glyph->pos.x = pen_x;
		glyph->pos.y = pen_y;

		int error = FT_Load_Glyph( face, glyph->index, FT_LOAD_DEFAULT );
		if ( error ) continue;

		error = FT_Get_Glyph( face->glyph, &glyph->image );
		if ( error ) continue;

		/* translate the glyph image now */
		error = FT_Glyph_Transform( glyph->image, 0, &glyph->pos );
		if(error) printf("nonscalable?");

		pen_x   += slot->advance.x;
		previous = glyph->index;

		/* increment number of glyphs */
		glyph++;
	}

	/* count number of glyphs loaded */
	num_glyphs = glyph - glyphs;
	if(imageHeight==0||imageWidth==0){
		FT_BBox    string_bbox;
		/* get bbox of original glyph sequence */
		compute_string_bbox( &string_bbox );

		/* compute string dimensions in integer pixels */
		imageWidth  = (imageWidth!=0)?imageWidth:((string_bbox.xMax - string_bbox.xMin));
		imageHeight = (imageHeight!=0)?imageHeight:((string_bbox.yMax - string_bbox.yMin));
	}

	unsigned char* pixels = renderHelper(0,imageHeight,imageWidth, r,g,b);
	unsigned int Bmask = 0x0000FF00;
	unsigned int Gmask = 0x00FF0000;
	unsigned int Rmask = 0xFF000000;
	unsigned int Amask = 0x000000FF;
	SDL_Surface* surface = SDL_CreateRGBSurfaceFrom(pixels,imageWidth,imageHeight,32,imageWidth*4,Rmask,Gmask,Bmask,Amask);
	return surface;

	
}

unsigned char * renderHelper(int angle, int my_target_height, int my_target_width, unsigned int r, unsigned int g, unsigned int b){
	FT_Vector  start;
	FT_Matrix  matrix;

	FT_Glyph   image;
	FT_Vector  pen;
	FT_BBox    bbox;
	FT_BBox    string_bbox;


	/* get bbox of original glyph sequence */
	compute_string_bbox( &string_bbox );

	/* compute string dimensions in integer pixels */
	int string_width  = (string_bbox.xMax - string_bbox.xMin);
	int string_height = (string_bbox.yMax - string_bbox.yMin);
	//printf("%i",string_width);
	if(my_target_height==0){
		my_target_height=string_height;
	}
	if(my_target_width==0){
		my_target_width=string_width;
	}
	/* set up start position in 26.6 Cartesian space */
	start.x = ( ( my_target_width  - string_width  )/ 2 )*64;
	start.y = ( ( my_target_height - string_height ) / 2 )*64;

	/* set up transform (a rotation here) */
	matrix.xx = (FT_Fixed)( cos( angle ) * 0x10000L );
	matrix.xy = (FT_Fixed)(-sin( angle ) * 0x10000L );
	matrix.yx = (FT_Fixed)( sin( angle ) * 0x10000L );
	matrix.yy = (FT_Fixed)( cos( angle ) * 0x10000L );

	pen = start;
	unsigned char * out = calloc(my_target_width*my_target_height,4);
	// memset(out,0xFF,my_target_width*my_target_height*4);
	int n;
	for ( n = 0; n < num_glyphs; n++ )
	{
	  /* create a copy of the original glyph */
	  int error = FT_Glyph_Copy( glyphs[n].image, &image );
	  if ( error ) continue;

	  /* transform copy (this will also translate it to the */
	  /* correct position                                   */
	  error = FT_Glyph_Transform( image, &matrix, &pen );
	  if(error)
	  	printf("glyph isnt scalable");

	  /* check bounding box; if the transformed glyph image      */
	  /* is not in our target surface, we can avoid rendering it */
	  FT_Glyph_Get_CBox( image, ft_glyph_bbox_pixels, &bbox );
	  if ( bbox.xMax <= 0 || bbox.xMin >= my_target_width  ||
	       bbox.yMax <= 0 || bbox.yMin >= my_target_height )
	    continue;
	  // printf("hmmmm ");
	  // FT_Bitmap bmp = ((FT_BitmapGlyph)image)->bitmap;
	  // printf("memset probably the issue");
	  // memset(bmp.buffer,0xFF,bmp.rows*bmp.width-25);
	  // printf(" or maybe not...\n");	
	  /* convert glyph image to bitmap (destroy the glyph copy!) */
	  error = FT_Glyph_To_Bitmap(
	            &image,
	            FT_RENDER_MODE_NORMAL,
	            0,                  /* no additional translation */
	            1 );                /* destroy copy in "image"   */
	  if ( !error )
	  {
	    FT_BitmapGlyph  bit = (FT_BitmapGlyph)image;


	    my_draw_bitmap( bit->bitmap,
	                    bit->left,
	                    my_target_height - bit->top, out, my_target_height, my_target_width, r,g,b);

	    /* increment pen position --                       */
	    /* we don't have access to a slot structure,       */
	    /* so we have to use advances from glyph structure */
	    /* (which are in 16.16 fixed float format)         */
	    // printf("image->advance.x >> 16 = %ld\n",(image->advance.x>>16));
	    // pen.x += image->advance.x >> 10;
	    // pen.y += image->advance.y >> 10;

	    FT_Done_Glyph( image );
	  }
	}
	return out;
}
void my_draw_bitmap(FT_Bitmap bitmap, FT_Int left, FT_Int top, unsigned char* pixels, int height, int width, unsigned int r, unsigned int g, unsigned int b){
	int i;
	for(i=0; i<bitmap.width; i++){
		int j;
		for(j=0; j<bitmap.rows; j++){
			if(left+i>width-1||top+j>height-1){
				break;
			}
			// printf("left equals %i\n",left);
			unsigned int ratio = bitmap.buffer[i+j*bitmap.width];
			pixels[(left+i+(top+j)*width)*4+3] = ((unsigned char)r);
			pixels[(left+i+(top+j)*width)*4+2] = (unsigned char)(g);
			pixels[(left+i+(top+j)*width)*4+1] = (unsigned char)(b);
			pixels[(left+i+(top+j)*width)*4] = (unsigned char)ratio;
		}
	}
}

void  compute_string_bbox( FT_BBox  *abbox )
{
  FT_BBox  bbox;


  bbox.xMin = bbox.yMin =  32000;
  bbox.xMax = bbox.yMax = -32000;
  int n;
  for ( n = 0; n < num_glyphs; n++ )
  {
    FT_BBox  glyph_bbox;


    FT_Glyph_Get_CBox( glyphs[n].image, ft_glyph_bbox_pixels,
                       &glyph_bbox );
    // printf("%li %li %li %li\n",glyph_bbox.xMin,glyph_bbox.yMin,glyph_bbox.xMax,glyph_bbox.yMax);
    if (glyph_bbox.xMin < bbox.xMin)
      bbox.xMin = glyph_bbox.xMin;

    if (glyph_bbox.yMin < bbox.yMin)
      bbox.yMin = glyph_bbox.yMin;

    if (glyph_bbox.xMax > bbox.xMax)
      bbox.xMax = glyph_bbox.xMax;

    if (glyph_bbox.yMax > bbox.yMax)
      bbox.yMax = glyph_bbox.yMax;
  }

  if ( bbox.xMin > bbox.xMax )
  {
  	printf("something isnt right");
    bbox.xMin = 0;
    bbox.yMin = 0;
    bbox.xMax = 0;
    bbox.yMax = 0;
  }
   // printf("returned bbox: %li %li %li %li\n",bbox.xMin,bbox.yMin,bbox.xMax,bbox.yMax);
  *abbox = bbox;
}