#include<stdio.h>
#include<string.h>
#include "zlib.h"
#include "opentiff.h"
#include <assert.h>


#if defined(MSDOS) || defined(OS2) || defined(WIN32) || defined(__CYGWIN__)
#  include <fcntl.h>
#  include <io.h>
#  define SET_BINARY_MODE(file) setmode(fileno(file), O_BINARY)
#else
#  define SET_BINARY_MODE(file)
#endif

#define CHUNK 16384

/* Compress from file source to file dest until EOF on source.
   def() returns Z_OK on success, Z_MEM_ERROR if memory could not be
   allocated for processing, Z_STREAM_ERROR if an invalid compression
   level is supplied, Z_VERSION_ERROR if the version of zlib.h and the
   version of the library linked do not match, or Z_ERRNO if there is
   an error reading or writing the files. */
int def(FILE *source, FILE *dest, int level)
{
    int ret, flush;
    unsigned have;
    z_stream strm;
    unsigned char in[CHUNK];
    unsigned char out[CHUNK];

    /* allocate deflate state */
    strm.zalloc = Z_NULL;
    strm.zfree = Z_NULL;
    strm.opaque = Z_NULL;
    ret = deflateInit(&strm, level);
    if (ret != Z_OK)
        return ret;

    /* compress until end of file */
    do {
        strm.avail_in = fread(in, 1, CHUNK, source);
        if (ferror(source)) {
            (void)deflateEnd(&strm);
            return Z_ERRNO;
        }
        flush = feof(source) ? Z_FINISH : Z_NO_FLUSH;
        strm.next_in = in;

        /* run deflate() on input until output buffer not full, finish
           compression if all of source has been read in */
        do {
            strm.avail_out = CHUNK;
            strm.next_out = out;
            ret = deflate(&strm, flush);    /* no bad return value */
            assert(ret != Z_STREAM_ERROR);  /* state not clobbered */
            have = CHUNK - strm.avail_out;
            if (fwrite(out, 1, have, dest) != have || ferror(dest)) {
                (void)deflateEnd(&strm);
                return Z_ERRNO;
            }
        } while (strm.avail_out == 0);
        assert(strm.avail_in == 0);     /* all input will be used */

        /* done when last data in file processed */
    } while (flush != Z_FINISH);
    assert(ret == Z_STREAM_END);        /* stream will be complete */

    /* clean up and return */
    (void)deflateEnd(&strm);
    return Z_OK;
}

//The struct for the signature of the PNG File
struct PNG_header
{
	unsigned char name[8]; //The signature contains the name PNG and some other variables
};
	
//The function which validates the signature of the PNG file and thus confirms that the formed image is surely
//a PNG file, otherwise an error message will pop out.
void check_png_header(FILE *fp)
{
	//Assigning the decimal values to the signature header of the PNG file.
	struct PNG_header header;
	header.name[0]=137;
	header.name[1]=80;
	header.name[2]=78;
	header.name[3]=71;
	header.name[4]=13;
	header.name[5]=10;
	header.name[6]=26;
	header.name[7]=10;
	
	fread(&header, sizeof(struct PNG_header), 1, fp);

	//Validating the PNG signature header.
	if(header.name[0]!=0x89)
		printf("Not a PNG file!\n");
	else if(header.name[1]!=0x50)
		printf("Not a PNG file!\n");
	else if(header.name[2]!=0x4E)
		printf("Not a PNG file!\n");
	else if(header.name[3]!=0x47)
		printf("Not a PNG file!\n");
	else if(header.name[4]!=0x0D)
		printf("Not a PNG file!\n");
	else if(header.name[5]!=0x0A)
		printf("Not a PNG file!\n");
	else if(header.name[6]!=0x1A)
		printf("Not a PNG file!\n");
	else if(header.name[7]!=0x0A)
		printf("Not a PNG file!\n");
	else
		printf("\nStarting to process the final image in %c%c%c format...\n",header.name[1],header.name[2],header.name[3]);//This will print PNG.
	//printf("Size of struct = %ld\n",sizeof(struct PNG_header));//Size of the signature header for the PNG file.
	//printf("\n");
	fwrite(header.name,8,1,fp);//Writing the data in the new image
}

//Function to convert the int value to a character value
void int_to_char(char *bytes, int n)
{
    bytes[0] = (n >> 24) & 0xFF;
    bytes[1] = (n >> 16) & 0xFF;
    bytes[2] = (n >> 8) & 0xFF;
    bytes[3] = n & 0xFF;
}

//This is the first chunk of a PNG file which stores the height, width, colour type, the compression form of the image
//and the filter method. As the final image will be a grayscale image thus the colour type of the final type has been
//assigned to 0. Also, we are not dealing with any type of compression formats or the filter methods or the interlace 
//methods of the final image, thus they are assigned a value of 0. 
struct IHDR_chunk
{
	unsigned char name[4];		
	unsigned char width[4];
	unsigned char height[4];	
	unsigned char depth;
	unsigned char colour_type;
	unsigned char compression;
	unsigned char filter;
	unsigned char interlace;
	unsigned char chunk_length[4];
	unsigned long crc;
};

//The function which will read the IHDR chunk of the pixel data to the final image formed.
void read_IHDR_chunk(FILE *fp)
{
	struct IHDR_chunk header;
	fread(&header,sizeof(struct IHDR_chunk),1,fp);		
	
	printf("Beginning the processing of the image chunk wise...\n");
	//printf("%c %c %c %c\n",header.name[0],header.name[1],header.name[2],header.name[3]);
	//printf("Size of struct = %ld\n",sizeof(struct IHDR_chunk));
	//printf("Chunk Length = %ld\n",sizeof(header.width)+sizeof(header.height)+sizeof(header.depth)+sizeof(header.colour_type)+sizeof(header.compression)+sizeof(header.filter)+sizeof(header.interlace));
	//printf("Depth = %d\n",header.depth);
	//printf("Colour Type = %d\n",header.colour_type);
	//printf("Compression = %d\n",header.compression);
	//printf("Filter Method = %d\n",header.filter);
	//printf("Interlace Method = %d\n",header.interlace);
	//printf("\n");
}

//The function which will write the IHDR chunk of the pixel data to the final image formed.
void write_IHDR(FILE *fp)
{
	//Assigning the decimal values to all the variables required to write the IHDR chunk in the PNG image which will
	//be formed.
	struct IHDR_chunk header;
	header.chunk_length[0]=0;
	header.chunk_length[1]=0;
	header.chunk_length[2]=0;
	header.chunk_length[3]=13;

	header.name[0]=73;
	header.name[1]=72;
	header.name[2]=68;
	header.name[3]=82;

	int_to_char(header.width,image_specs.width-1); 
	int_to_char(header.height,image_specs.height);
	header.depth=8;
	header.colour_type=0;
	header.compression=0;
	header.filter=0;
	header.interlace=0;

	header.crc = crc32(0,(char*)&header,17);//This is a function of the zlib itself and without the CRC data the image will
											//not be loaded.
	//Writing the IHDR chunk data to the final image.
	fwrite(header.chunk_length,4,1,fp);
	fwrite(header.name,4,1,fp);
	fwrite(header.width,4,1,fp);
	fwrite(header.height,4,1,fp);
	fwrite(&header.depth,1,1,fp);
	fwrite(&header.colour_type,1,1,fp);
	fwrite(&header.compression,1,1,fp);
	fwrite(&header.filter,1,1,fp);
	fwrite(&header.interlace,1,1,fp);
	fwrite(&header.crc,4,1,fp);
	printf("Examining all the grayscale pixel data and checking for the CRCs\n");
}

//Another chunk of the PNG image which has the CRC data and some other data required to make the PNG image.
struct IDAT_chunk
{
	unsigned char chunk_length[4];
	unsigned char name[4];
	unsigned char crc[4];
		
};

//This function is now used to take the grayscale pixel data from a file that has been formed during the RGB conversion
//to grayscale. The source TIFF image being an RGB image was converted to a grayscale image and the pixel data is stored
//in a file. That file (here "file1") is used to read the data and the write that data in the final PNG image. A temporary 
//file named "temp.png" that is just used to find the length of the file and is the removed.
void check_IDAT_chunk(FILE *fp)
{
	struct  IDAT_chunk header;
	
	FILE* fp1 = fopen("file1","rb");
    FILE* fp2 = fopen("temp.png","wb+");
    fseek(fp1,0,SEEK_SET);
    def(fp1,fp2,3);
    unsigned int final_point = ftell(fp2);
    unsigned int length = ftell(fp2);

    fclose(fp2);
    remove("temp.png");

    int_to_char(header.chunk_length,length);

	header.name[0]='I';
    header.name[1]='D';
    header.name[2]='A';
    header.name[3]='T';
    header.crc[0]=0;
    header.crc[1]=0;
    header.crc[2]=0;
    header.crc[3]=0;

    fwrite(header.chunk_length,sizeof(int),1,fp);
    fwrite(header.name,sizeof(char),4,fp);
    fseek(fp1,0,SEEK_SET);
    def(fp1,fp,3);
    fwrite(header.crc,sizeof(char),4,fp);
    fclose(fp1);
    printf("Reaching the end of processing the final image...\n");
   


}

//This is the last chunk of the image in the PNG format. This is an empty chunk.
struct IEND_chunk
{
	unsigned char size[4];
	unsigned char name[4];
	unsigned char crc[4];
};
	
void write_IEND(FILE *fp)
{
	struct IEND_chunk header;

	//Assigning the decimal values to the variables in the IEND chunk.
	header.size[0]=0;
	header.size[1]=0;
	header.size[2]=0;
	header.size[3]=0;

	header.name[0]='I';
	header.name[1]='E';
	header.name[2]='N';
	header.name[3]='D';

	header.crc[0]=174;
	header.crc[1]=66;
	header.crc[2]=96;
	header.crc[3]=130;

	fwrite(header.size,4,1,fp);
	fwrite(header.name,4,1,fp);
	fwrite(header.crc,4,1,fp);
	printf("Image processing completed successfully.\nFinal image generated successfully!\n");

}
	
	
int main(int argc,char *argv[])
{
	
    	argc=2;
    	open_TIFF_File(argv[1]); //Calling the function that converts the TIFF image into an array of raw grayscale pixel data.

	FILE *fp = fopen("TargetImage.png","wb+");//This is the final output file (image) that will be created in the same directory
											  //and will be of the PNG format.
	
	if(fp==NULL)
		printf("File does not exist");
	else
	{
		//Calling all the functions in a sequential order in which they are required to form the final output image.
		check_png_header(fp);
		write_IHDR(fp);
		read_IHDR_chunk(fp);
		check_IDAT_chunk(fp);
		write_IEND(fp);
	}

	fclose(fp);
	remove("file1");
}	
 