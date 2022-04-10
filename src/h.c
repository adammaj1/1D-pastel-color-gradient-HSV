/*


c console program for testing color gradient
creats
*.txt files with data
* ppm file
 
It converts hsv to rgb ( uses a code from gnuplot)


gcc h.c -lm -Wall
a@zelman:~/c/varia/outor/gradient/hsv$ ./a.out
===========================

h= 360.000000	 s=0.000000	 v=1.000000
r= 1.000000	 g=1.000000	 b=1.000000
-----------------------------------
h= 0.000000	 s=1.000000	 v=1.000000
r= 1.000000	 g=0.000000	 b=0.000000






*/



#include <stdio.h>
#include <string.h> // strncat
#include <stdlib.h> // malloc
#include <math.h>		// floor;  needs -lm also






// size of virtual 2D array of pixels
// each piexel has a RGB color
int iWidth = 600;
int iHeight ; // 


// size of the dynamic 1D array 
unsigned char * data;
size_t ColorSize = 3; // RGB = number of color components = channels
size_t ArrayLength; // number of 1D array's elements = ENumber = iWidth*iHeight*ColorSize
size_t ElementSize; // size of array's element in bytes
size_t ArraySize; // size of array in bytes = ElementSize*ArrayLength 




// ppm P6 file
size_t HeaderSize ; // size of the P6 file  header in bytes
size_t FileSize; // = HeaderSize +ArraySize [bytes]




/* Gradient type is a combination of general and joining gradient type */

const char * sGradientGeneralType[] = {"continous" , "discrete" };
// enum items are contiguous 
typedef enum  {continous = 0 , discrete = 1 } GradientGeneralType; 
size_t gMaxType; // length (size) of the array




const char * sGradientJoiningType[] = {"no" , "steps" , "tubes", "diverging"};
// enum items are contiguous 
typedef enum  {no = 0 , steps = 1 , tubes = 2, diverging = 3} GradientJoiningType; 
size_t jMaxType; // length (size) of the array




// complementary hue for diverging color gradient
double h_complementary;




int segments = 4; // the number should be even, 2 segments create one "wave"}

// discrete = n-colour ordered gradient, 
/*
Number of data classes : https://colorbrewer2.org/
"Choosing the number of data classes is an important part of map design. Increasing the number of data classes will result in a more "information rich" map by decreasing the amount of data generalization. However, too many data classes may overwhelm the map reader with information and distract them from seeing general trends in the distribution. In addition, a large numbers of classes may compromise map legibility—more classes require more colors that become increasingly difficult to tell apart.
Many cartographers advise that you use five to seven classes for a choropleth map. Isoline maps, or choropleth maps with very regular spatial patterns, can safely use more data classes because similar colors are seen next to each other, making them easier to distinguish."
*/


int DataClasses = 7; // the number of columns in the ordered discrete gradients = number of ordered data categories
double p_step; // for discrete gradient












/* 

GNUPLOT : http://gnuplot.sourceforge.net/
file : getoutor.c 

 * Petr Mikulik, December 1998 -- June 1999
 * Copyright: open source as much as possible
 *
*/



// from colour.h
/* Contains a colour in RGB scheme.
   Values of  r, g and b  are all in range [0;1] */
typedef struct {
    double r, g, b;
} rgb;


/* hsv :
 h : deg [0, 1.0]
 s : % [0.0,1.0]
 v : % [0.0,1.0]
*/ 
typedef struct {
    double h, s, v;
} hsv;



// ************************************************** functions *********************************************************************888




















// gives position of 2D point (ix,iy) in 1D array  ; uses also global variables: iWidth , ColorSize
int Give_i ( int iX, int iY)
{
  return (iX + iY * iWidth) * ColorSize;  
}



/*


Lerp
The method named " Lerp " stands for " Linear_intERPolation" 

OldRange = (OldMax - OldMin)  
NewRange = (NewMax - NewMin)  
NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin

*/

double pale(double OldValue){

	double OldMax = 1.0; // high value
	double OldMin = 0.0; // low value 
	double OldRange = (OldMax - OldMin);  
	double NewMax = 0.6; // intermediate value
	double NewMin = 0.0; // low value
	double NewRange = (NewMax - NewMin);  
	double NewValue = (((OldValue - OldMin) * NewRange) / OldRange) + NewMin;
	return NewValue;

}










// fractional part of real (double) number
double frac(double d){

	double fraction = d - ((long)d);
	return fraction;
}


// for discrete gradient, smth like round(double>1.0) but for double in range [0,1]
double double2steps(double p){

	double s = p / p_step;
	s = s - frac(s);
	s *= p_step;
	return s;


}





/* 
   remember to update : 
   *  add new title in titles array
   * titles in plot.gp
*/

int GivePale_HSV_Color( const double h,  const double s, hsv* hsv_color){

	// input position is double number in range [0,1] = normalized
			 
	/* Pastels or pastel colors belong to a pale family of colors, in the HSV color space, have high value and low to intermediate saturation. */
		
	hsv_color->h = h; // h has the full range
	hsv_color->s = pale(s); /* only low to intermediate saturation: limited range = [0.0, 0.6] */
	hsv_color->v = 1.0; /* constant high value */
  		
  
	
	return 0;
}






/*
 *  outor Conversion Algorithms
 *  taken from http://www.cs.rit.edu/~ncs/outor/t_convert.html
 *  each outor model should provide a conversion to RGB.
 *  RGB values are clipped to [0,1] as some outors in some
 *  models have no RGB value.
 * see also https://en.wikipedia.org/wiki/HSL_and_HSV#HSV_to_RGB
 
 */

static void HSV_2_RGB(hsv in, rgb *out)
{
    	double h,s,v, f,p,q,t;
    	int i = 0 ;
	
	// read from input hsv color
    	h = in.h; /* h range in gnuplot is [0,1] and not the usual [0,360] */
    	s = in.s;
    	v = in.v;
    	
    	//
    	if (s == 0) { /* achromatic (gray) */
        	out->r = out->g = out->b = v;
		return;
    		}
	// change range of h from [0, 1.0] to [0, 6.0]
    	h *= 6.;  
    	
    	// divide h into integer and fractional part
    	i = floor(h); // integer part of h
    	f = h - i;   //  fractional part of h
    	
    	//
    	p = v * (1.0 - s);
    	q = v * (1.0 - s*f);
    	t = v * (1.0 - s*(1.0-f));
    	
    	switch(i % 6) {
    		case 0:
			out->r = v;
			out->g = t;
			out->b = p;
			break;
    		
    		case 1:
			out->r = q;
			out->g = v;
			out->b = p;
			break;
    		
    		case 2:
			out->r = p;
			out->g = v;
			out->b = t;
			break;
    		
    		case 3:
			out->r = p;
			out->g = q;
			out->b = v;
			break;
    
    		case 4:
			out->r = t;
			out->g = p;
			out->b = v;
			break;
    
    		default:
			out->r = v;
			out->g = p;
			out->b = q;
			break;
    }
}







void PrintColor( FILE *fp, double position, double modified_position, hsv in, rgb out){
  // normalized to [0.0, 1.0] range RGB color channels
  double R = out.r; 
  double G = out.g;
  double B = out.b;
  // [Relative luminance is formed as a weighted sum of linear RGB components](https://en.wikipedia.org/wiki/Luma_(video)) 
  // 
  //from function test_palette_subcommand from file gnuplot/src/command.c test_palette_subcommand
  //ntsc = 0.299 * rgb.r + 0.587 * rgb.g + 0.114 * rgb.b;
  double Y = 0.299*R + 0.587*G + 0.114*B;
  
  
  //print 1:9 columns
	fprintf(fp, "%f\t%f\t", position, modified_position);
	fprintf(fp, "%f\t %f\t%f\t%f\t", R, G, B, Y);
	fprintf(fp, "%f\t %f\t %f\n", in.h, in.s, in.v);
	
	
  

}






double ModifyPosition(const double position, const GradientGeneralType gTypeVar, const GradientJoiningType jTypeVar){
	
	// input position  should be in  [0,1] range 
	double p = position; // p = local copy of position
	// if position > 1 then we have repetition of colors = periodic function = wave   
	 
	
	switch(jTypeVar){
		// simple monotone function 
		case no : { break;} // return input position without modifications
		
		// periodic waves with different joinings
		case steps : {	p = p * segments; // periodic  = change range
				p = frac(p); 
				
    				break;}
    				
		case tubes : {	p = p * segments; // periodic = change range
				int ip = (int)p;
      				p = p-ip; // fractional part 
      				if (ip % 2) {p = 1.0-p;} // reverse gradient
      				
				break;}
				
		case diverging : { 	
					// 2 parts 
					p *= 2.0; // change range 
					if (p>1.0) // upper part 
						{p = 2.0 - p;} 	 
					 p = 1.0 - p; // reverse gradient of both parts
										
					break;}
		
		default:{}
	}
	
	if ( gTypeVar == discrete ) { p = double2steps(p); }
	
	return p; // output in [0,1] range = modified position

}










/* 
	position -> hsv -> rgb 
 input :  position  double in [0.0, 1.0]
 output c =  unsigned char c array
 

*/
void ComputeAndPrintColor( FILE *fp, const double position, const double h, GradientGeneralType gType, GradientJoiningType jType, unsigned char c[]){

	
  
        hsv in;   // set palette model HSV functions gray,1,1
	rgb out;
	
	
	double mh = h; // local copy of h for modification
	if ( jType == diverging && position > 0.5 ) 
		{ mh = h_complementary;} // diverging is made from 2 single hue gradients 
		
	double s = ModifyPosition( position, gType, jType); // saturation = modified position
	
	
	
	
	
	// "colormap traversing HSV color space. The map is obtained by linearly varying the value through all possible values while keeping constant saturation and hue."
	GivePale_HSV_Color(mh, s, &in); // compute pale hsv color in
	HSV_2_RGB(in, &out); // convert hsv to rgb 
	
	//
	PrintColor( fp, position, s, in, out);
  
  	//  change range ant number type  of channels from double in [0, 1.0] to unsigned char in [0,255] range, so the output image must have 24 bit color or lower 
  	c[0] = 255*out.r; //R
  	c[1] = 255*out.g; // G
  	c[2] = 255*out.b; // B	
	

}













int PlotPoint(unsigned char A[], int i, unsigned char color[]){

  A[i]   = color[0]; /* Red*/
  A[i+1] = color[1]; /* Green */
  A[i+2] = color[2];  /* Blue */
  return 0;


}



// --------------------
int FillArrayAndSaveTxtFile (unsigned char A[] , double h, GradientGeneralType gType, GradientJoiningType jType){

  int iX;
  int iXmax = iWidth;
  int iY;
  int iYmax = iHeight;
  int i; //  index of 1D array  
  double position; // number form 0.0 to 1.0 used for color gradient = relative ( normalised to [0,1] range ) value
  unsigned char color[3]; // 
	
  // text file used by the gnuplot for creating images
 	char name [120]; /* name of file */
 	snprintf (name, sizeof name, "%f_%s_%s",  h, sGradientGeneralType[gType], sGradientJoiningType[jType]); // the same as in SaveArray2PPM functiona  and the h.gp file
  char *filename =strcat(name,".txt");
  FILE *fp = fopen(filename, "w");
  if (fp==NULL) {
    printf("Error opening text file!\n");
    return 1;
  }
  fprintf(fp, "#p \t\t mp\t\t R \t\t G \t\tB \t\tY \t h\ts\tv\n"); // header of the text file
	
	
  // horizostal move : each row of the picels has different color proportional to x ( position)
  for(iX=0; iX<iXmax; ++iX){    
    position = (double) iX / iXmax;
    ComputeAndPrintColor(fp, position, h, gType, jType, color);
    
    // vertical move : each pixeal has the smae color  		
    for(iY=0; iY<iYmax; ++iY){
      i = Give_i(iX, iY);
      PlotPoint(A, i , color);
    }
        		
         			
  }

	printf ("File %s saved. \n", filename);
  	fclose(fp);
  
  
  return 0;
}





// --------------- save dynamic "A" array of uinsigned char to the binary ppm file ( P6 ) --------------------------------
int SaveArray2PPM (unsigned char A[], size_t ASize,   double h, GradientGeneralType gType, GradientJoiningType jType)
{

  FILE *fp;
  const unsigned char MaxColorComponentValue = 255;	/* color component is coded from 0 to 255 ;  it is 8 bit color file */
  
  char name [120]; /* name of file */
  snprintf (name, sizeof name, "%f_%s_%s",  h, sGradientGeneralType[gType], sGradientJoiningType[jType]); // the same as in FillArrayAndSaveTxtFile function and the h.gp file
  char *fileName =strcat(name,".ppm");
  
  

  /* save image to the pgm file  */
  fp = fopen (fileName, "wb");	/*create new file,give it a name and open it in binary mode  */
  if (fp == NULL) 
    {	printf("File open error"); 
      return 1;}
  else {
    fprintf (fp, "P6\n%u %u\n%u\n", iWidth, iHeight, MaxColorComponentValue);	/*write header to the file */
    fwrite (A, ASize, 1, fp);	// write dynamic A array to the binary file in one step  
 
    printf ("File %s saved. \n", fileName);
    fclose (fp);
    return 0;}
  
}



// h from hsv
int MakeGradientImage( double h, GradientGeneralType gType, GradientJoiningType jType){

	// compute complementary color for diverging gradient
	if ( jType == diverging  ) { // diverging is made from 2 single hue gradients 
		
		h_complementary = h + 0.5;
		if (h_complementary > 1.0) { h_complementary = h_complementary - 1.0 ;}
		} 
  	
  	FillArrayAndSaveTxtFile(data,  h, gType, jType);
  	SaveArray2PPM(data, ArraySize, h, gType, jType);
  	return 0;

}



int setup(){

  	iHeight = iWidth/3; 
  	// 1D array
  	ArrayLength = iWidth*iHeight*ColorSize;
  	ElementSize = sizeof(unsigned char);
  	ArraySize = ElementSize*ArrayLength ; 
  	HeaderSize = 11 + (size_t) (log10(iHeight) +log10(iWidth)); 
	
  	FileSize = HeaderSize + ArraySize; 
	
  	/* create dynamic 1D array for RGB colors  */
  	data = malloc (ArraySize);
  	if (data == NULL ){
    		printf ( "Could not allocate memory for the array\n");
    		return 1;}

	
	// gradient types
	gMaxType =  sizeof(sGradientGeneralType)/sizeof(sGradientGeneralType[0]); // 
	jMaxType =  sizeof(sGradientJoiningType)/sizeof(sGradientJoiningType[0]); 
	
	p_step = 1.0 / DataClasses; // for discrete gradient
	
  	return 0;
}


void info(){

  	printf("ppm (P6) header size = %zu bytes\n", HeaderSize);
  	printf("Array Size  = %zu bytes\n", ArraySize);
  	printf("PPM file size  = %zu bytes\n", FileSize);
  	// ??????
	printf(" HSV color space \n");
	printf("h = Hue is in  [0.0, 1.0] range\n");
	printf("s = Saturation is in  [0.0, 1.0] range\n");
	printf("v = value is in  [0.0, 1.0] range. Here it is a variable  = gradient position \n");
	// 
	printf(" segments = %d ;  int value, the number should be even, 2 segments create one \"wave\" \n" , segments); 
	printf(" DataClasses = %d ;  int value , the number of columns in the ordered discrete gradients = number of ordered data categories \n" , DataClasses); 
	printf(" p_step = %f (double value) , steps of gradient position  \n" , p_step); 

	
}



int end(){


  	printf (" allways free memory (deallocate )  to avoid memory leaks \n"); // https://en.wikipedia.org/wiki/C_dynamic_memory_allocation
  	free (data);
  	info();
  	return 0;

}



int main(){

	
	GradientGeneralType gType;
  	GradientJoiningType jType;
  	//
  	
	double h;  // h from hsv 
	double dh = 0.123; // h step 
	
	setup();
	
	
	
	// make gradient image for every type  : h_g_j
	h = 0.00;
	while (h < 1.15){ // full range of hue from 0.0 to 1.0
		for (gType = 0; gType< gMaxType; ++gType){
  			for (jType = 0; jType < jMaxType; ++jType){
				MakeGradientImage( h, gType, jType);}}
		h += dh;}
	
  	
  	
  	end();

	
	return 0;
}


