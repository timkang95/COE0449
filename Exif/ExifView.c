#include <stdio.h>
#include <stdlib.h>
#include <string.h>
struct header
{
	short starter;
	short app1;
	short block;
	char exif_string[4];
	short nulterm;
	char endian[2]; 
	short version;
	long tiffblock;
};

struct tif_tag
{
	short ident;
	short data;
	int items;	
	int offset;
};

int main(int argc, char **argv)
{
	struct header head;
	struct tif_tag tif_tag;	
        unsigned short num;
	char str[4];
	char manf[50]; /* = "Manufacturer: ";
	char camMod[25] = "Camera Model: ";
	char exifSub[25] = "Exif sub block address: ";
	char width[25] = "Width in pixels: ";
	char height[25] = "Height in pixels: ";
	char iso[25] = "ISO speed: ";
	char expSpd[25] = "Exposure speed: ";
	char fstop[25] = "F-stop: ";
	char lens[25] = "Lens focal length: ";
	char date[25] = "Date taken: ";
	int manfLength = strlen(manf);*/
	
	FILE *f = fopen(argv[1], "r");
	fread(&head, 20, 1, f);
	printf("0x%hx 0x%hx %d %s 0x%hx %s %d %d", head.starter, head.app1, head.block, head.exif_string, head.nulterm, head.endian, head.version, head.tiffblock);
	fread(&num, 2, 1, f); /*counter*/
	char* tif_tagstr;
	int i;
	int x;
	int size;
	char random[50];
	unsigned short counter;
	unsigned int c, d;
	double a, b;
	for(i = 0; i < num; i++)/* reads and saves tif_tags */
	{	
		struct tif_tag *tif_tagstr = (struct tif_tag*) malloc(sizeof(struct tif_tag));
		printf("size of struct=%d\n", sizeof(struct tif_tag));
		//printf("%d", x);
		fread(&tif_tag, 12, 1, f);
		printf("\n 0x%hx %hx %hx %hx \n ", tif_tag.ident, tif_tag.data, tif_tag.items, tif_tag.offset);
		
		if(tif_tag.ident == 0x010F)
		{
			
			size = ftell(f);		
			fseek(f, tif_tag.offset + 12, SEEK_SET);
			//fread(&tif_tagstr, 2, 1, f);
			fread(&random, sizeof(random), 1, f);	
			printf("Manufacturer: %s ", random);
			/*strcpy(manf[manfLength], tif_tagstr);*/
			fseek(f, size, SEEK_SET);
			//printf("%s", tif_tagstr);
		}
		if(tif_tag.ident == 0x0110)
		{
			size = ftell(f);
			fseek(f, tif_tag.offset + 12, SEEK_SET);
			fread(&random, sizeof(random), 1, f);
			fseek(f, size, SEEK_SET);
			printf("Model: %s", random);
			//fread(&tif_tagstr, 2, 1, f);
		}
		if(tif_tag.ident == (short)0x8769)
		{
			size = ftell(f);
			fseek(f, size - 12, SEEK_SET);
			fread(&tif_tag, 12, 1, f);
			//fseek(f, size, SEEK_SET);
			//fseek(&random, sizeof(
			printf("offset %hx", tif_tag.offset);
			//fseek(f, size + tif_tag.offset - 12, SEEK_SET);
			
			
			printf("whereeee %hx", tif_tag.ident);
			fseek(f, tif_tag.offset + 12, SEEK_SET);	
			fread(&counter, 2, 1, f);
			printf(" count == %d \n", counter);			

			for(x = 0; x < counter; x++)
			{ 	
				fread(&tif_tag, 12, 1, f);
		//printf("\n 0x%hx %hx %hx %hx \n ", tif_tag.ident, tif_tag.data, tif_tag.items, tif_tag.offset);
		
				if(tif_tag.ident == (short)0xA002)
				{
					//fread(&bullshit, sizeof(bullshit), 1, f);
					printf("Width in pixels: %d pixels \n", tif_tag.offset);

				}
				if(tif_tag.ident == (short)0xA003)
				{
					printf("Height in pixels: %d pixels \n", tif_tag.offset);
				}
				if(tif_tag.ident == (short)0x8827)
				{
					printf("ISO: ISO %d \n", tif_tag.offset);
				}
				if(tif_tag.ident == (short)0x829a)
				{
					
					size = ftell(f);
					fseek(f, tif_tag.offset+12, SEEK_SET);
					fread(&c, sizeof(d), 1, f);
					//fseek(f, 2, SEEK_CUR);
					fread(&d, 2, 1, f);
					fseek(f, size, SEEK_SET);
					printf("Exposure: %3.0f/%3.0f seconds\n", (double)c, (double)d);
				
				}
				
				if(tif_tag.ident == (short)0x829d)
				{
					//size = ftell(f);
					
					fseek(f, tif_tag.offset, SEEK_SET);
					fread(&a, 8, 1, f);
					fread(&b, 8, 1, f);
					fseek(f, size + 12, SEEK_SET);
					printf("F-Stop: f/%4.1f\n", (b/a/10));
				}
				
				if(tif_tag.ident == (short)0x920A)
				{
					size = ftell(f);
					fseek(f, tif_tag.offset+12, SEEK_SET);
					fread(&c, sizeof(d), 1, f);
					//fseek(f, 2, SEEK_CUR);
					fread(&d, 2, 1, f);
					fseek(f, size, SEEK_SET);
					printf("Focal Length: %3.0f mm\n", (double)(c/d));
				}
				if(tif_tag.ident == (short)0x9003)
				{
					size = ftell(f);		
					fseek(f, tif_tag.offset + 12, SEEK_SET);
					//fread(&tif_tagstr, 2, 1, f);
					fread(&random, sizeof(random), 1, f);	
					printf("Date Taken: %s \n", random);
					fseek(f, size, SEEK_SET);
			
				}				
	
			}
		}
			
			
	}

			


	return 0;

}