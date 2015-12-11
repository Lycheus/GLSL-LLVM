#include <stdio.h>
#include <stdlib.h>
#define FORMAT_BMP_PPM 1 //1: bmp 0:ppm

int main()
{
#if FORMAT_BMP_PPM
  FILE *pFile_r=fopen("wood.bmp","r");
#else
  FILE *pFile_r=fopen("wood.ppm","r");
#endif
  FILE *pFile_w=fopen("framebuffer","w");
  char get_char;
  int pic_count=0;
  int pix_count=0;
  int coor_count=65535-255;
  int i;

  for(i=0;i<54;i++)
  getc(pFile_r);
  while(  (get_char=(char)getc(pFile_r))!=EOF)
    {
      unsigned int pixel=0,get_pixel;
#if FORMAT_BMP_PPM
      i=2;
#else
      i=0;
#endif
      get_pixel=(int)get_char;
      pixel+=(get_pixel<<(i*8));
#if FORMAT_BMP_PPM
      for(i=1;i>=0;i--)
#else
	for(i=1;i<=2;i++)
#endif
	{
	  get_char=(char)getc(pFile_r);
	  get_pixel=(int)get_char;
	  pixel+=(get_pixel<<(i*8));
	}
      fprintf(pFile_w,"%d\n%d\n",coor_count*4,pixel);
      
      if(pix_count%256==0)
	coor_count-=511;
      else
	coor_count++;
      pix_count++;
    }
  return 0;
}
