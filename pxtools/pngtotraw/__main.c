#include "stdio.h"
#include "stdlib.h"
#include "memory.h"
#include "lpng1634/png.h"
#include "../../PainterEngine/Core/PX_Core.h"
#include "../../PainterEngine/platform/windows/windows_display.h"
#pragma comment(lib,"./libpng16.lib")
#pragma comment(lib,"./zlib.lib")

unsigned char* buffer = NULL;  



int getRowBytes(int width){  
	if((width * 3) % 4 == 0){  
		return width * 3;  
	}else{  
		return ((width * 3) / 4 + 1) * 4;  
	}  
}  

px_bool PX_PngToTraw(const px_char pngFile[],const px_char trawFile[],px_int swidth,px_int sheight)
{
	png_uint_32 png_width, png_height, color_type;  
	px_memorypool mp;
	px_texture in_texture,out_texture;
	px_int newwidth,newheight,mpsize;
	px_byte *alloc_buffer;

	png_structp png_ptr;  
	png_infop info_ptr;  
	int bit_depth;  
	unsigned int i,j;  
	FILE *fp;  
	png_bytep* row_pointers;
	unsigned int bufSize = 0;  
	int rowBytes;
	PX_TRaw_Header trawHeader;
	px_color *pcolor;


	if ((fp = fopen(pngFile/*argv[1]*/, "rb")) == NULL) {  
		printf("invalid png file\n");
		return PX_FALSE;  
	}  

	png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);  

	if (png_ptr == NULL)  
	{  
		fclose(fp);  
		printf("invalid png file\n");
		return PX_FALSE;  
	}  
	info_ptr = png_create_info_struct(png_ptr);  
	if (info_ptr == NULL)  
	{  
		fclose(fp);  
		png_destroy_read_struct(&png_ptr, NULL, NULL);  
		printf("invalid png file\n");
		return PX_FALSE;  
	}  
	if (setjmp(png_jmpbuf(png_ptr))) {  
		/* Free all of the memory associated with the png_ptr and info_ptr */  
		png_destroy_read_struct(&png_ptr, &info_ptr, NULL);  
		fclose(fp);  
		/* If we get here, we had a problem reading the file */  
		return PX_FALSE;  
	}  
	/* Set up the input control if you are using standard C streams */  
	png_init_io(png_ptr, fp);  
	png_read_png(png_ptr, info_ptr, PNG_TRANSFORM_EXPAND, 0);  
	png_get_IHDR(png_ptr, info_ptr, &png_width, &png_height, &bit_depth, &color_type,  
		NULL, NULL, NULL);  
	printf("PNG File info -width[%d], height[%d], bit_depth[%d], color_type[%d]\n",png_width, png_height, bit_depth, color_type);  

	
	if (swidth<=0||sheight<=0)
	{
		newwidth=png_width;
		newheight=png_height;
	}
	else
	{
		newwidth=swidth;
		newheight=sheight;
	}

	row_pointers = png_get_rows(png_ptr, info_ptr);  


	if (color_type == PNG_COLOR_TYPE_RGB) {  
		bufSize = getRowBytes(png_width) * png_height;  
	} else if (color_type == PNG_COLOR_TYPE_RGBA) {  
		bufSize = png_width * png_height * 4;  
	} else{  
		printf("invalid png file\n");
		return PX_FALSE;  
	}  
	trawHeader.Magic[0]='T';
	trawHeader.Magic[1]='R';
	trawHeader.Magic[2]='A';
	trawHeader.Magic[3]='W';

	trawHeader.Height=(int)newheight;
	trawHeader.Width=(int)newwidth;

	mpsize=png_height*png_width*4+sizeof(PX_TRaw_Header)+newheight*newwidth*4+sizeof(PX_TRaw_Header)+1024;
	alloc_buffer=(px_byte *)malloc(mpsize);

	mp=MP_Create(alloc_buffer,mpsize);

	PX_TextureCreate(&mp,&in_texture,png_width,png_height);

	pcolor=(px_color *)((unsigned char *)in_texture.surfaceBuffer);

	rowBytes=getRowBytes(png_width);

	if(color_type == PNG_COLOR_TYPE_RGB)
	{
		printf("RGB format\n");
		for (i = 0; i < png_height; i++) 
			for(j=0;j<png_width;j++)
			{
				(*pcolor)._argb.r=row_pointers[i][j*3];
				(*pcolor)._argb.g=row_pointers[i][j*3+1];
				(*pcolor)._argb.b=row_pointers[i][j*3+2];
				(*pcolor)._argb.a=255;
				pcolor++;
			}
	}
	else if(color_type == PNG_COLOR_TYPE_RGBA)
	{
		printf("RGBA format width:%d height:%d\n",newwidth,newheight);
		for (i = 0; i < png_height; i++) 
			for(j=0;j<png_width;j++)
			{
				if (row_pointers[i][j*4+3]!=0)
				{
					(*pcolor)._argb.r=row_pointers[i][j*4];
					(*pcolor)._argb.g=row_pointers[i][j*4+1];
					(*pcolor)._argb.b=row_pointers[i][j*4+2];
					(*pcolor)._argb.a=row_pointers[i][j*4+3];
				}
				else
				{
					(*pcolor)._argb.ucolor=0;
				}

				pcolor++;
			}
	}

	png_destroy_read_struct(&png_ptr, &info_ptr, 0);  
	fclose(fp);  

	if (((newheight==png_height)&&(newwidth==png_width)))
	{
		if (!PX_TextureCopy(&mp,&in_texture,&out_texture))
		{
			printf("memory error\n");
			return PX_FALSE;
		}
		
	}
	else
	{
		if(!PX_TextureCreateScale(&mp,&in_texture,newwidth,newheight,&out_texture))
		{
			printf("memory error\n");
			return PX_FALSE;
		}
	}
	


	fp=fopen(trawFile/*argv[2]*/,"wb");
	if (fp==NULL)
	{
		printf("Generate Traw error\n");
		return PX_FALSE;
	}

	buffer=(unsigned char *)out_texture.surfaceBuffer;
	fwrite(&trawHeader,sizeof(trawHeader),1,fp);

	if(fwrite(buffer,1,newheight*newwidth*4,fp)!=newheight*newwidth*4)
	{
		printf("Generate Traw error\n");
		fclose(fp);
		return PX_FALSE;
	}
	fclose(fp);
	printf("TRAW->%s\n",trawFile/*argv[2]*/);


	PX_TextureFree(&in_texture);
	PX_TextureFree(&out_texture);
	return PX_TRUE;
}

int main( int _Argc,char *argv[])
{
	px_char pngPath[260]={0};
	px_char trawPath[260]={0};
	px_int width=0;
	px_int height=0;
	if (_Argc==3)
	{
		PX_strset(pngPath,argv[1]);
		PX_strset(trawPath,argv[2]);
		PX_PngToTraw(pngPath,trawPath,width,height);
	}
	else if (_Argc==5)
	{
		PX_strset(pngPath,argv[1]);
		PX_strset(trawPath,argv[2]);
		width=PX_atoi(argv[3]);
		height=PX_atoi(argv[4]);
		PX_PngToTraw(pngPath,trawPath,width,height);
	}
	else
	{
		
		px_char *path=PX_OpenFileDialog("png file *.png\0*.png\0");
		if (path)
		{
			PX_strset(pngPath,path);
			PX_strset(trawPath,path);
			PX_strset(trawPath+PX_strlen(trawPath)-3,"traw");
			if(!PX_PngToTraw(pngPath,trawPath,width,height))
			{
				MessageBox(0,"转换失败","error",MB_OK);
			}
			else
			{
				MessageBox(0,"转换完成","error",MB_OK);
			}
		}
		
	}
	return 1;
}