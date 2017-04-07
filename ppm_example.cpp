//#ifndef PPM_EXAMPLE
//#define PPM_EXAMPLE
//#pragma once
/* Unlike Java, the C compiler does not search for external classes or libraries, so
 * it needs to know the function signature (arguments and return type) for every function
 * *BEFORE* the function gets used.  For example, to declare that printf() take a string
 * and a variable number of additional arguments, and returns an int, your program must
 * include the following statemnt before ever using printf:
 *
 * int printf(const char *format, ...);
 *
 * Note the ; at the end of the statment instead of { } characters.
 * This is called a *FUNCTION PROTOTYPE*, and it just declares the function's
 * arguments and return types so the compiler can do type checking.
 *
 * Forunately, you don't have to write out all the function prototypes in
 * every file.  Instead, the convention is to prepare a *HEADER FILE* that
 * customarily ends in ".h" containing all your prototypes.  The, use the
 * C Preprocessor's #include directive to literally paste the contents
 * of any header files you need at the top of your C file.
 *
 * Primitive, but effective.
 */
#include <stdio.h>  // paste the contents of stdio.h here, including the prototype for printf()
#include <stdlib.h> // malloc and free are defined here
#include <string.h> // string.h contains the prototype for memset()
#include <assert.h> // needed to use the assert() function for debugging
#include <math.h>
#include <ppm_example.h>

#define CLAMP(x) ((x) > 255 ? 255 : (x))

/* C doesn't have classes, but it does have structs.  A struct is
 * a sort of "anti-interface".  It can include data fields, but
 * it can't include any methods.  There is also no inheritance.
 */

/* Functions in C look almost identical to methods in Java,
 * except they aren't inside of classes.  They behave exactly
 * like static functions in Java.  (Warning: You can also declare
 * C functions to be static, but it means something different.)
 *
 * C handles strings and arrays differently from Java.  A string
 * in C is just an array of characters stored one after the other
 * in memory, and ending with the special NUL character '\0'.  The
 * "char *" type is a "pointer to char" and holds the memory address
 * of a character.  Since an array of characters is just stored
 * sequentially in memory, a pointer to the first characeter in
 * the array is also a pointer to the whole array.  So pointers
 * and arrays are used almost interchangeably in C, and char *
 * is the basic string type.
 *
 * Things get really fun with "char **argv".  This is a "pointer
 * to a pointer to char" and it holds the memory address of a
 * location that itself holds the memory address where a character
 * is stored.  Alternatively, "char **" is an array
 * of pointers to chars, or just an array of strings.
 *
 * In many respects, pointers behave like reference variables
 * in Java.  The syntax is different, so you don't have to
 * know whether a variable is a primitive or object type to
 * know whether it is passed by value or reference.  You can
 * also directly manipulate the memory addresses stored in pointer
 * variables, not just the data they point to.  This is very
 * powerful, but also very dangerous.
 *
 * In short, argv is an array of strings, and specifically
 * the array of command-line arguments to your program.
 * argv[0] is the name of the program, argv[1] is the first
 * argument, and so on.
 *
 * argc is the total number of arguments provided.  Unlike Java,
 * C does not keep track of the size of an array--that is the
 * programmer's responsibility.
 *
 * Note that in C, main() should always return an int.  The return
 * value of main() becomes the return value of the program.  By
 * convention, a return value of 0 means there was no error.  Any
 * other return value (positive or negative) usually indicates an
 * error.
 */

/*int main(int argc, char **argv) {
    // read in the input image
    printf("Reading, %s!\n", argv[1]);
    img_t *img = read_ppm(argv[1]);

    for (int i = 2; i < argc-1; i++) {          //Read command line arguments
        if (!strcmp(argv[i], "-grayscale")) {
            grayscale(img);
        }
        else if (!strcmp(argv[i], "-flip")) {
            flip(img);
        }
        else if (!strcmp(argv[i], "-flop")) {
            flop(img);
        }
        else if (!strcmp(argv[i], "-transpose")) {
            transpose(img);
        }
        else if (!strcmp(argv[i], "-boxblur")) {
            int n;
            sscanf(argv[i+1], "%i", &n);
            boxblur(img, n);
            i++;
        }
        else if (!strcmp(argv[i], "-median")) {
            int n;
            sscanf(argv[i+1], "%i", &n);
            median(img, n);
            i++;
        }
        else if (!strcmp(argv[i], "-gaussian")) {
            int n;
            int s;
            sscanf(argv[i+1], "%i", &n);
            sscanf(argv[i+2], "%i", &s);
            gaussian(img, n, s);
            i += 2;
        }
        else if (!strcmp(argv[i], "-sobel")) {
            sobel(img);
        }
        else if (!strcmp(argv[i], "-size")) {
            int x;
            int y;
            sscanf(argv[i+1], "%i", &x);
            sscanf(argv[i+2], "%i", &y);
            size(img, x, y);
            i += 2;
        }
        else if (!strcmp(argv[i], "-scale")) {
            float factor;
            sscanf(argv[i+1], "%f", &factor);
            scale(img, factor);
            i++;
        }
    }

    // now write the image
    printf("Writing %s\n", argv[argc-1]);
    write_ppm(img, argv[argc-1]);

    // free up memory
    destroy_img(&img); // &img is the address in memory where the img variable is stored
    // Since img is of type (img *), &img is of type (img **)

    return 0;
}*/

void grayscale(img_t *img) {
    int h = img->h;
    int w = img->w;
    for (pixel_t *p = img->data; // initialize the loop coutner to the beginning of the array
         p < img->data + h*w;   // p should not contain a memory address past img.img + (size * sizeof(pixel_t)) bytes
         // C knows you mean (size * sizeof(pixel_t)) bytes because img.img is of type (pixel_t *)
         p++) {               // add sizeof(pixel_t) bytes to p so it points to the next element in the array
        unsigned char y = CLAMP(0.299*(p->r) + 0.587*(p->g) + 0.114*(p->b));
        //        y = CLAMP(y);     //CLAMP breaks functionality. Works well without it.
        p->r = y;
        p->g = y;
        p->b = y;// p->r is identical to (*p).r, i.e. the r field of whatever p points to
    }
}

void flip(img_t *img) {
    int h = img->h;
    int w = img->w;
    for (int i = 0; i < (w/2)*h; i++) {
        //Row and column indices start at 0
        int row = i / (w/2);
        int column = i % (w/2);
        pixel_t p = img->data[row * w + column]; //Store old first pixel
        img->data[row * w + column] = img->data[row * w + w - column - 1]; //Replace first pixel
        img->data[row * w + w - column - 1] = p; //Replace second pixel with old first pixel
    }
}

void flop(img_t *img) {
    int h = img->h;
    int w = img->w;
    for (int i = 0; i < w*(h/2); i++) {
        //Row and column indices start at 0
        int row = i / w;
        int column = i % w;
        pixel_t p = img->data[row * w + column]; //Store old first pixel
        img->data[row * w + column] = img->data[(h - 1 - row) * w + column]; //Replace first pixel
        img->data[(h - 1 - row) * w + column] = p; //Replace second pixel with old first pixel
    }
}

void transpose(img_t *img) {
    int h = img->h;
    int w = img->w;
    for (int i = 0; i <= w*(h/2) - 1; i++) {
        //Row and column indices start at 0
        int row = i / w;
        int column = i % w;
        pixel_t p = img->data[row * w + column]; //Store old first pixel
        img->data[row * w + column] = img->data[(h - 1 - row) * w + w - column - 1]; //Replace first pixel
        img->data[(h - 1 - row) * w + w - column - 1] = p; //Replace second pixel with old first pixel
    }
}

void boxblur(img_t *img, int n) {
    int h = img->h;
    int w = img->w;
    img_t *oldCopy = new_img(img->w, img->h);
    memcpy(oldCopy->data, img->data, h*w*sizeof(pixel_t));   //Use memcpy to store old img so that information is not replaced as the image is altered
    for (int i = 0; i < h * w; i++) {
        int row = i / w;
        int column = i % w;
        int avgR = 0;
        int avgG = 0;
        int avgB = 0;
        if (column - n < 0 || column + n >= w || row + n >= h || row - n < 0) {     //Check to make sure box fits in image boundaries
            avgR += 0;
            avgG += 255;
            avgB += 0;
        }
        else {
            int pixcount = 0;
            for (int x = -n; x <= n; x++) {
                for (int y = -n; y <= n; y++) {
                    int pixIndex = (row+y) * w + column + x;
                    pixel_t currPix = oldCopy->data[pixIndex];
                    avgR += currPix.r;
                    avgG += currPix.g;
                    avgB += currPix.b;  //Add all rgb values
                    pixcount++;
                }
            }
            if (pixcount <= 1) pixcount = 1;
            avgR /= pixcount;
            avgG /= pixcount;
            avgB /= pixcount;   //Pixcount increments for every pixel included, so this step divides by the number of pixels
        }
        img->data[i].r = CLAMP(avgR);
        img->data[i].g = CLAMP(avgG);
        img->data[i].b = CLAMP(avgB);
    }
    destroy_img(&oldCopy);
}


void shiftList(unsigned char *ref, int lastElement, int index) {     //shift all elements after element at 'index' one index higher
    for (int i = lastElement; i > index; i--) {
        ref[i] = ref[i-1];
    }
}

void insertSorted(unsigned char *ref, int lastElement, unsigned char toInsert) {
    int i;
    if (lastElement == 0) {     //If first element, just insert it
        ref[0] = toInsert;
        return;
    }
    for (i = 0; i < lastElement; i++) {     //Find a place in the list for this element
        if (ref[i] >= toInsert) {
            shiftList(ref, lastElement, i);
            ref[i] = toInsert;
            return;
        }
    }
    ref[lastElement+1] = toInsert;      //If entire list is less than element, just insert at the end
}

void median(img_t *img, int n) {
    int h = img->h;
    int w = img->w;
    img_t *oldCopy = new_img(img->w, img->h);
    memcpy(oldCopy->data, img->data, h*w*sizeof(pixel_t));   //Use memcpy to store old img so that information is not replaced as the image is altered
    for (int i = 0; i < h * w; i++) {
        int row = i / w;
        int column = i % w;
        unsigned char Rlist[(2*n+1)*(2*n+1)];
        unsigned char Glist[(2*n+1)*(2*n+1)];
        unsigned char Blist[(2*n+1)*(2*n+1)];
        if (column - n < 0 || column + n >= w || row + n >= h || row - n < 0) {     //Check to make sure box fits in image boundaries
            img->data[i].r = 0;
            img->data[i].g = 255;
            img->data[i].b = 0;
        }
        else {
            int pixcount = 0;
            for (int x = -n; x <= n; x++) {
                for (int y = -n; y <= n; y++) {
                    int pixIndex = (row+y) * w + column + x;
                    pixel_t currPix = oldCopy->data[pixIndex];
                    insertSorted(Rlist, pixcount, currPix.r);
                    insertSorted(Glist, pixcount, currPix.g);
                    insertSorted(Blist, pixcount, currPix.b);       //Compile sorted lists of R, G, and B values in the box
                    pixcount++;
                }
            }
            unsigned char R = Rlist[pixcount/2];
            unsigned char G = Glist[pixcount/2];
            unsigned char B = Blist[pixcount/2];        //Find the value in the middle of each list.
            img->data[i].r = CLAMP(R);
            img->data[i].g = CLAMP(G);
            img->data[i].b = CLAMP(B);
        }
    }
    destroy_img(&oldCopy);
}

void gaussian(img_t *img, int n, int s) {
    int h = img->h;
    int w = img->w;
    //Blur along x
    img_t *oldCopy = new_img(img->w, img->h);
    memcpy(oldCopy->data, img->data, h*w*sizeof(pixel_t));   //Use memcpy to store old img so that information is not replaced as the image is altered
    for (int i = 0; i < h * w; i++) {
        int row = i / w;
        int column = i % w;
        int r = 0;
        int g = 0;
        int b = 0;
        float weightsum = 0;
        if (column - n < 0 || column + n >= w || row + n >= h || row - n < 0) {     //Check to make sure box fits in image boundaries
            r += 0;
            g += 255;
            b += 0;
        }
        else {
            for (int x = -n; x <= n; x++) {
                int pixIndex = row * w + column + x;
                pixel_t currPix = oldCopy->data[pixIndex];
                r += currPix.r * exp((-x*x)/(2*s*s));   //Multiply by weighting formula before adding
                g += currPix.g * exp((-x*x)/(2*s*s));
                b += currPix.b * exp((-x*x)/(2*s*s));
                weightsum += exp((-x*x)/(2*s*s));   //store total weight to divide by later
            }
            r /= weightsum;
            g /= weightsum;
            b /= weightsum;
        }
        img->data[i].r = CLAMP(r);
        img->data[i].g = CLAMP(g);
        img->data[i].b = CLAMP(b);
    }
    //Blur along y
    memcpy(oldCopy->data, img->data, h*w*sizeof(pixel_t));   //Use memcpy to store x blurred image so that information is not replaced as the image is altered
    for (int i = 0; i < h * w; i++) {
        int row = i / w;
        int column = i % w;
        int r = 0;
        int g = 0;
        int b = 0;
        float weightsum = 0;
        if (column - n < 0 || column + n >= w || row + n >= h || row - n < 0) {     //Check to make sure box fits in image boundaries
            r += 0;
            g += 255;
            b += 0;
        }
        else {
            for (int y = -n; y <= n; y++) {
                int pixIndex = (row+y) * w + column;
                pixel_t currPix = oldCopy->data[pixIndex];
                r += currPix.r * exp((-y*y)/(2*s*s));   //Multiply by weighting formula before adding
                g += currPix.g * exp((-y*y)/(2*s*s));
                b += currPix.b * exp((-y*y)/(2*s*s));
                weightsum += exp((-y*y)/(2*s*s));   //store total weight to divide by later
            }
            r /= weightsum;
            g /= weightsum;
            b /= weightsum;
        }
        img->data[i].r = CLAMP(r);
        img->data[i].g = CLAMP(g);
        img->data[i].b = CLAMP(b);
    }
    destroy_img(&oldCopy);   //HELP Attempting to destroy causes a crash
}

/*How to detect an edge
Move a box along with [-1, 0, 1]
get vertical and horizontal
get edge strength - sqrt(horiz^2 + vert^2) - divide 510 and multiply by 255 to get within 0-255 range
get edge angle - atan - do not worry about this
*/

void sobel(img_t *img) {
    int h = img->h;
    int w = img->w;
    grayscale(img);     //grayscale the image
    img_t *oldCopy = new_img(img->w, img->h);
    memcpy(oldCopy->data, img->data, h*w*sizeof(pixel_t));   //Use memcpy to store old img so that information is not replaced as the image is altered
    for (int i = 0; i < h * w; i++) {
        int row = i / w;
        int column = i % w;
        if (column - 1 < 0 || column + 1 >= w || row + 1 >= h || row - 1 < 0) {     //Check to make sure box fits in image boundaries
            img->data[i].r = 0;
            img->data[i].g = 255;
            img->data[i].b = 0;
        }
        else {
            int dx, dy;
            //Horizontal calculation
            unsigned char pixBefore = oldCopy->data[row * w + column - 1].r;
            unsigned char pixAfter = oldCopy->data[row * w + column + 1].r;
            dx = pixAfter - pixBefore;      //Get the horizontal difference in brightness
            //Vertical calculation
            pixBefore = oldCopy->data[(row-1) * w + column].r;
            pixAfter = oldCopy->data[(row+1) * w + column].r;
            dy = pixAfter - pixBefore;      //Get the vertical difference in brightness
            unsigned char strength = 255 * (sqrtf(dx*dx + dy*dy)/510);  //Calculate magnitude and fit into range 0-255 to work as pixel value
            img->data[i].r = CLAMP(strength);
            img->data[i].g = CLAMP(strength);
            img->data[i].b = CLAMP(strength);
        }
    }
    destroy_img(&oldCopy);
}

void sizePPM(img_t *img, int width, int height) {
    int h = img->h;
    int w = img->w;
    img_t *newImg = new_img(width, height);
    for (int i = 0; i < height * width; i++) {
        int newRow = i / width;
        int newColumn = i % width;
        float posX = (((float)newColumn)/((float)(width-1))) * (w-1);
        float posY = (((float)newRow)/((float)(height-1))) * (h-1);
        if (posX < 0.0000001) posX = 0;
        if (posY < 0.0000001) posY = 0;
        unsigned char R;
        unsigned char G;
        unsigned char B;
        if ((posX == 0 && posY == 0) || (posX == w-1 && posY == h-1) || (posX == 0 && posY == h-1) || (posX == w-1 && posY == 0)) {  //If its a corner pixel
            R = img->data[(int)(posY * w + posX)].r;      //Set it to be the corner pixel of the original image
            G = img->data[(int)(posY * w + posX)].g;
            B = img->data[(int)(posY * w + posX)].b;
        }
        else if (posY == 0 || posY == h-1) {    //If it's a horizontal edge pixel
            int leftPos = (int)(floor(posX));     //Linear interpolation
            int rightPos = (int)(ceil(posX));
            float leftWeight = rightPos - posX;
            R = img->data[(int)(posY * w + leftPos)].r * leftWeight + img->data[(int)(posY * w + rightPos)].r * (1-leftWeight);
            G = img->data[(int)(posY * w + leftPos)].g * leftWeight + img->data[(int)(posY * w + rightPos)].g * (1-leftWeight);
            B = img->data[(int)(posY * w + leftPos)].b * leftWeight + img->data[(int)(posY * w + rightPos)].b * (1-leftWeight);
        }
        else if (posX == 0 || posX == w-1) {    //If it's a vertical edge pixel
            int topPos = (int)(floor(posY));     //Linear interpolation
            int bottomPos = (int)(ceil(posY));
            float topWeight = bottomPos - posY;
            R = img->data[(int)(topPos * w + posX)].r * topWeight + img->data[(int)(bottomPos * w + posX)].r * (1-topWeight);
            G = img->data[(int)(topPos * w + posX)].g * topWeight + img->data[(int)(bottomPos * w + posX)].g * (1-topWeight);
            B = img->data[(int)(topPos * w + posX)].b * topWeight + img->data[(int)(bottomPos * w + posX)].b * (1-topWeight);
        }
        else {  //Otherwise
            int topLeftIndex = (int)(floor(posY) * w + floor(posX));     //Bilinear interpolation!
            int bottomLeftIndex = (int)(ceil(posY) * w + floor(posX));
            int topRightIndex = (int)(floor(posY) * w + ceil(posX));
            int bottomRightIndex = (int)(ceil(posY) * w + ceil(posX));
            float leftWeight = ceil(posX) - posX;
            float topWeight = ceil(posY) - posY;
            float topAvgR = img->data[topLeftIndex].r * leftWeight + img->data[topRightIndex].r * (1-leftWeight);
            float bottomAvgR = img->data[bottomLeftIndex].r * leftWeight + img->data[bottomRightIndex].r * (1-leftWeight);
            R = topAvgR * topWeight + bottomAvgR * (1-topWeight);
            float topAvgG = img->data[topLeftIndex].g * leftWeight + img->data[topRightIndex].g * (1-leftWeight);
            float bottomAvgG = img->data[bottomLeftIndex].g * leftWeight + img->data[bottomRightIndex].g * (1-leftWeight);
            G = topAvgG * topWeight + bottomAvgG * (1-topWeight);
            float topAvgB = img->data[topLeftIndex].b * leftWeight + img->data[topRightIndex].b * (1-leftWeight);
            float bottomAvgB = img->data[bottomLeftIndex].b * leftWeight + img->data[bottomRightIndex].b * (1-leftWeight);
            B = topAvgB * topWeight + bottomAvgB * (1-topWeight);
        }
        newImg->data[i].r = R;
        newImg->data[i].g = G;
        newImg->data[i].b = B;
    }
    free(img->data);     //Discard original images's data
    *img = *newImg;       //Set original image equal to the new one
    free(newImg);   //Destroy old image
    //HELP: Behaves differently in debug. Crashes if given certain dimensions (If square, crashes with 117x117 and above). Premature end to file if given certain large dimensions (If square, 393x393 and above).
}

void scale(img_t *img, float factor) {
    sizePPM(img, round(img->w * factor), round(img->h * factor));
}

/* Since img_t doesn't contain a constructor, destructor/finalize, or any other methods,
 * we need to build up a library of functions to create, destroy and manipulate images.
 */

// Create a new image of specified size.
img_t *new_img(int w, int h) {
    /* Crash if width or height is 0 or negative.  This is pretty drastic,
   * but very convenient for debugging.  You normally use it as a sanity
   * check for errors that should not occur in production.
   *
   * In the debugger, your program will break at the line with the failed assertion,
   * so you can your variables and figure out what went wrong.  Outside the debugger
   * your program will generally crash with an error that states the failed assertion's
   * condition and its file and line number in the source code.
   */
    assert(w > 0);
    assert(h > 0);

    // Allocate enough memory to hold an img_t struct
    // sizeof(img_t) returns the number of bytes required
    // for an img_t struct.  malloc allocates memory, similar
    // to new in Java/C++, except that *MALLOC DOES NOT INITIALIZE THE MEMORY*!!!
    // The block of memory returned by malloc is allowed to
    // contain garbage, or even a password or other sensitive
    // information left over in memory from a previous process.
    // It is up to you to initialize the memory's contents
    // in subsequent statements.
    //
    // Note that malloc's return type is "void *", which means
    // "pointer to a blcok of memory of no particular type".  You
    // need to explicitly cast this to (img_t *) to say that this
    // memory will be used to contain an img_t.
    //
    // (C doesn't require you to include the cast, but C++ does.)
    img_t *img = (img_t *) malloc(sizeof(img_t));

    // now initialize img appropriately
    img->w = w;
    img->h = h;

    // allocate memory for the image pixels
    img->data = (pixel_t *) malloc(w * h * sizeof(pixel_t));

    // zero out all the image pixels so they don't contain garbage
    // memset is quite handy for this
    memset(img->data, 0, w * h * sizeof(pixel_t));

    return img;
}

// Destroy an image and free up its memory
// Unlike Java, C does not release memory when you are no longer using it;
// you have to explicitly free the memory yourself.
//
// C also doesn't prevent you from accessing memory that you have freed, but the
// behavior is undefined when you do.  If you're lucky, your program will crash immediately,
// but the problems can be much subtler.  Your program might work find in the debugger, but
// crash outside the debugger.  It might work fine on your computer, but crash when your TA is grading, ...
//
// One way to reduce the likelihood of accessing freed memory is to set any pointer to NULL immediately after
// freeing it.
void destroy_img(img_t **img) {
    // step 1: free the image pixels
    free((*img)->data); // as long as you allocated img->data with malloc, free knows how big it is
    (*img)->data = NULL; // this is a sanity check to make sure we don't try and access img->data after deleting it
    free(*img); // now free the img_t structure
    *img = NULL; // finally, set the img pointer provided by the caller to NULL so the caller
    // can't accidentally access the freed memory
}

// Read in a PPM file
img_t *read_ppm(const char *fname) {
    int w, h;

    assert(fname != NULL); // crash if fname is NULL
    FILE *f = fopen(fname, "rb"); // open the ppm for reading in binary mode
    assert(f != NULL); // crash if the file didn't open

    fscanf(f, "P6 %d %d 255%*c", &w, &h); // read in the header and image width and height

    img_t *img = new_img(w, h); // create an empty image of the correct size

    fread(img->data, 3, w * h, f); // read the image data into img->data

    fclose(f); // close the file

    return img;
}

// Write out a PPM file
void write_ppm(const img_t *img, const char *fname) {
    assert(img != NULL); // crash if img is NULL
    assert(fname != NULL); // crash if fname is NULL

    FILE *f = fopen(fname, "wb"); // open fname for writing in binary mode; clobbers any existing file
    assert(f != NULL); // crash if file did not open

    fprintf(f, "P6\n%d %d 255\n", img->w, img->h); // write the image header
    fwrite(img->data, img->w * img->h, 3, f); // write the image data

    fclose(f); // close the file
}
// #endif // PPM_EXAMPLE
