
unsigned short int endiation;
unsigned int offset;

unsigned int IFD_entry_tracker;

int data_read_modify_function(unsigned int bytes_to_proceed,FILE* fp);
void open_TIFF_File();
void IFD_structure(FILE *fp);
void IFD_tag_cases(FILE *fp);
unsigned char* grayscale;

struct image_array
{
 									//We give a pointer to a memory location as we don't know exactly the size of array required. Also we store RGB seperately
    unsigned char* R;
    unsigned char* G;
    unsigned char* B;
    
};											

struct image_specifications
{
	
	unsigned short height;
	unsigned short width;
};

struct image_specifications image_specs;

struct image_array copyrgb(struct image_array image, FILE *fp, unsigned int offset);

unsigned char* convertrgb(struct image_array image, unsigned int offset);
