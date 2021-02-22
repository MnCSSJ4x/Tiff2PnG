#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include"opentiff.h"

void open_TIFF_File(char *source)
{
    struct image_array image;
    FILE *fp = fopen(source,"rb");
    
    printf("File Opened Succesfully\n");

    fread(&endiation,1,1,fp);		     						//Check endiation first as everything depends on this
    
    if(endiation == 73)                       						//49 hex small endian 
            endiation = 0;
    else
            endiation = 1;  							 	    //big endiation              
               						 
    printf("Endiation Check ... Done !");
                                 		  						 //we move ahead 1 byte to next location
    data_read_modify_function(1,fp);							 //we move ahead from the endiation part to now the file identification 
    
    if(data_read_modify_function(2,fp)!=42)					     //42 is standard value for a tiff
    {
    	printf("NOT A VALID TIFF");
    	return ;
    }
    
    printf("Starting to Read offset data\n");
    
    offset = data_read_modify_function(4,fp);  					    // We read the offset and shift 4 bytes in in memory of the image file.Now the image data begins(pixel data) 	  											       which we store in an array with help of pointers

    printf("Offset Reading Done!\n");
    printf("Memory allocation for R,G,B reading and storing data ....\n");

    image = copyrgb(image,fp,offset);

    grayscale = convertrgb(image,offset);

  
    
    printf("\nProceeding to read IFDs and Tags\n");
    										 //Now our file poiner has reached the end of image data and we start with the IFD detection
    IFD_structure(fp);								//IFD_structure would get us the number of IFD entries of the TIFF image (IFD_entry_tracker is hence made global)
    													
    while(IFD_entry_tracker>0)							//Now we go through each tags and extract needed info like width,height...
    {
    	IFD_tag_cases(fp);
    	
    	IFD_entry_tracker--;
 
    }

    offset=data_read_modify_function(4,fp);
    

}

int data_read_modify_function(unsigned int bytes_to_proceed,FILE* fp)	//this helps us to convert the data from endiation as well as 
{										//shifts the address of fp ahead by the number of bytes required 
    unsigned int data = 0;
    unsigned int byte = 0;

    int i = 0;
    
    while(i<bytes_to_proceed)
    {
        fread(&byte,1,1,fp);

        if(endiation == 0)
            data += byte*pow(256,i);        					//small endiation we need to reverse the data stored .256 as 1 byte has 8 bits with 2^8 values
        else
            data = data*256+byte;             					//big endiation the reversal is not necessary of data 
        i++;                                        
    }
    return data;                           					//we return the coverted data which can be now modified (RAW DATA)


}
void IFD_structure(FILE* fp)
{
	short number_of_entries = data_read_modify_function(2,fp);		/*An IFD consists of two bytes indicating the number of entries followed by the entries themselves. The IFD is terminated 											with 4 byte offset to the next IFD or 0 if there are none. A TIFF file must contain at least one IFD!*/
	IFD_entry_tracker = number_of_entries ;
	
}

void IFD_tag_cases(FILE* fp)
{		short tag = data_read_modify_function(2,fp);
		int datatype_of_field = data_read_modify_function(2,fp);
		
		switch(tag)
		{
			case 256:
				
				data_read_modify_function(4,fp);
				image_specs.width = data_read_modify_function(2,fp);
				printf("Image Width detected = %d\n",image_specs.width);
				data_read_modify_function(2,fp);
				break; 
				
			case 257:
				
				data_read_modify_function(4,fp);
				image_specs.height = data_read_modify_function(2,fp);
				printf("Image Height detected = %d\n",image_specs.height);
				data_read_modify_function(2,fp);
				break;
			default:
                		data_read_modify_function(8,fp);
                		break;
		
		}
				
		
									//IFD basicaly has 2 bytes to detect the tag , the next 4 for the datatype of information, then the last 4 bytes has the 
}									//value or offset to the value 

struct image_array copyrgb(struct image_array image, FILE *fp, unsigned int offset)
{
    image.R = (unsigned char*)malloc((offset-8)*sizeof(char)/3);
    image.G = (unsigned char*)malloc((offset-8)*sizeof(char)/3);
    image.B = (unsigned char*)malloc((offset-8)*sizeof(char)/3);  //our intention is to read all data after the header i.e byte 4 to the offset of the first image tag
                                                                                 //We dont know the size of data so we are dynamically allocating memory for the image data array
    for(unsigned int i=0; i<offset-8; i++)
    {
        if(i%3 == 2)
        {
        	*(image.B+i/3) = data_read_modify_function(1,fp);
        }
        else if(i%3 == 1)
        {
        	*(image.G+i/3) = data_read_modify_function(1,fp);
        }
        else if(i%3 == 0)
        {
        	*(image.R+i/3) = data_read_modify_function(1,fp);
        }
    } 
    printf("Storing RGB values Done!\n");
    return image; 
}

unsigned char* convertrgb(struct image_array image, unsigned int offset)
{
    
    FILE *fp=fopen("file1","wb+");
    grayscale = (unsigned char*)malloc((offset-8)*sizeof(char)/3);
    for(unsigned int i=0; i<(offset-8)*sizeof(char)/3; i++)
    {
        *(grayscale + i) = (0.3*(image.R[i])) + (0.59*(image.G[i])) + (0.11*(image.B[i]));
        fwrite(grayscale+i,1,sizeof(unsigned char),fp);
    }
    printf("Converting RGB values into grayscale Done! Stored grayscale values successfully.");
    return grayscale;
}