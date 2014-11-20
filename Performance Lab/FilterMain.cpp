#include <stdio.h>
#include "cs1300bmp.h"
#include <iostream>
#include <fstream>
#include "Filter.h"

using namespace std;

#include "rtdsc.h"

//
// Forward declare the functions
//
Filter * readFilter(string filename);
double applyFilter(Filter *filter, cs1300bmp *input, cs1300bmp *output);

int main(int argc, char **argv)
{

  if ( argc < 2) {
    fprintf(stderr,"Usage: %s filter inputfile1 inputfile2 .... \n", argv[0]);
  }

  //
  // Convert to C++ strings to simplify manipulation
  //
  string filtername = argv[1];

  //
  // remove any ".filter" in the filtername
  //
  string filterOutputName = filtername;
  string::size_type loc = filterOutputName.find(".filter");
  if (loc != string::npos) {
    //
    // Remove the ".filter" name, which should occur on all the provided filters
    //
    filterOutputName = filtername.substr(0, loc);
  }

  Filter *filter = readFilter(filtername);

  double sum = 0.0;
  int samples = 0;

  for (int inNum = 2; inNum < argc; inNum++) {
    string inputFilename = argv[inNum];
    string outputFilename = "filtered-" + filterOutputName + "-" + inputFilename;
    struct cs1300bmp *input = new struct cs1300bmp;
    struct cs1300bmp *output = new struct cs1300bmp;
    int ok = cs1300bmp_readfile( (char *) inputFilename.c_str(), input);

		if ( ok ) {
			double sample = applyFilter(filter, input, output);
			sum += sample;
			samples++;
			cs1300bmp_writefile((char *) outputFilename.c_str(), output);
		}
    delete input;
    delete output;
  }
  fprintf(stdout, "Average cycles per sample is %f\n", sum / samples);

}

struct Filter *
readFilter(string filename)
{
  ifstream input(filename.c_str());

  if ( ! input.bad() ) {
    int size = 0;
    input >> size;
    Filter *filter = new Filter(size);
    int div;
    input >> div;
    filter -> setDivisor(div);
    for (int i=0; i < size; i++) {
      for (int j=0; j < size; j++) {
		int value;
		input >> value;
		filter -> set(i,j,value);
      }
    }
    return filter;
  }
}


double
applyFilter(struct Filter *filter, cs1300bmp *input, cs1300bmp *output)
{

  long long cycStart, cycStop; //def keep
  cycStart = rdtscll();	//def keep

  output -> width = input -> width; //def keep
  output -> height = input -> height; //def keep
  
  //int f = filter -> getSize();
  int h = (input -> height) - 1; //def keep, faster because doesn't have to calculate in loop every time
  int w = (input -> width) - 1; //def keep
  int outcolor;
  unsigned int a, b, c, d;

  unsigned int div = filter -> getDivisor();

//makes a local array of filter->get, less time going into memory to retrieve values
  int filt[3][3]; //3 because they are always 3 by 3
  for (int j = 0; j < 3; j++) {
    for (int i = 0; i < 3; i++) {	
      filt[i][j] = filter -> get(i, j);
	}
  }

  if (filt[1][0] == 0 && filt[1][1] == 0 && filt[1][2] == 0) { //hline filter
    for(int plane = 0; plane < 3; plane++) { //faster to go plane, row, col. (spatial locality)
      for(int row = h-1; row != 0 ; row--) {
        a = row-1;
        for(int col = w-1; col != 0 ; col--) {
			b = col-1;

			int outcolor = 0;

			for (int j = 0; j < 3; j++) {
				c = b+j;
				outcolor += (input -> color[plane][a][c] * filt[0][j] ) + (input -> color[plane][a+2][c] * filt[2][j] );
			}

			if ( outcolor < 0 ) { //min 0
				outcolor = 0;
			}
			if ( outcolor > 255 ) { //max 255
				outcolor = 255;
			}

			output -> color[plane][row][col] = outcolor; //sets the output to the value of outcolor (0 - 255)
        }
      }
    }
  }


	  else if (filt[0][0] == 1 && filt[1][1] == 1 && filt[2][2] == 1){  //avg filter
		for(int plane = 0; plane < 3; plane++) {
			for(int row = h-1; row != 0 ; row--) {
				a = row-1;
				for(int col = w-1; col != 0 ; col--) {
					b = col-1;

					outcolor = 0;

					for (int i = 0; i < 3; i++) {	
					d = a+i;
						for (int j = 0; j < 3; j++) {	
							outcolor += (input -> color[plane][d][b+j]); // drop the *filer->get(i,j) because it is always 1
						}
					}

					outcolor /= div;

					if ( outcolor < 0 ) {
						outcolor = 0;
					}

					if ( outcolor > 255 ) {
						outcolor = 255;
					}

					output -> color[plane][row][col] = outcolor;
        }
      }
    }
  }

  else if (div == 1) { //emboss filter?
    for(int plane = 0; plane < 3; plane++) {
      for(int row = h-1; row != 0 ; row--) {
        a = row-1;
        for(int col = w-1; col != 0 ; col--) {
			b = col-1;

			outcolor = 0;

          for (int i = 0; i < 3; i++) {	
				d = a+i;
				for (int j = 0; j < 3; j++) {
					c = b+j;

					outcolor += (input -> color[plane][d][c] * filt[i][j] );
				}
			}

		if ( outcolor < 0 ) {
			outcolor = 0;
		}

		if ( outcolor > 255 ) {
			outcolor = 255;
        }

        output -> color[plane][row][col] = outcolor;
        }
      }
    }
  }

  else {  //all other filters
    for(int plane = 0; plane < 3; plane++) {
		for(int row = h-1; row != 0 ; row--) {
			a = row-1;
			for(int col = w-1; col != 0 ; col--) {
				b = col-1;

				outcolor = 0;

				for (int j = 0; j < 3; j++) {
					c = b+j;
					for (int i = 0; i < 3; i++) {	
						d = a+i;
						outcolor += (input -> color[plane][d][c] * filt[i][j] );
					}
				}

				outcolor /= div;

				if ( outcolor < 0 ) {
					outcolor = 0;
				}
				if ( outcolor > 255 ) {
					outcolor = 255;
				}

				output -> color[plane][row][col] = outcolor;
        }
      }
    }
  }

  cycStop = rdtscll();
  double diff = cycStop - cycStart;
  double diffPerPixel = diff / (output -> width * output -> height);
  fprintf(stderr, "Took %f cycles to process, or %f cycles per pixel\n",
	  diff, diff / (output -> width * output -> height));
  return diffPerPixel;
}
