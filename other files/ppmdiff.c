#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "assert.h"
#include "a2methods.h"
#include "a2plain.h"
#include "pnm.h"

float compare(FILE *ppm1, FILE *ppm2, A2Methods_T methods);

int main(int argc, char *argv[])
{
        assert(argc == 3);

        FILE *fp_a = fopen(argv[1], "r");
        FILE *fp_b = fopen(argv[2], "r");

        if (fp_a == NULL || fp_b == NULL) {
            fprintf(stderr, 
                    "Could not open one of the files for reading\n");
            exit(1);
        }
        
        A2Methods_T methods = uarray2_methods_plain; 
        assert(methods);
        
        float difference = compare(fp_a, fp_b, methods);

        printf("%f\n", difference);
        
        fclose(fp_a);
        fclose(fp_b);

        return EXIT_SUCCESS;
}

float compare(FILE *ppm1, FILE *ppm2, A2Methods_T methods)
{
        Pnm_ppm first_ppm = Pnm_ppmread(ppm1, methods);
        Pnm_ppm second_ppm = Pnm_ppmread(ppm2, methods);

        A2Methods_UArray2 first_array = first_ppm->pixels;
        A2Methods_UArray2 second_array = second_ppm->pixels;

        unsigned smaller_width;
        unsigned smaller_height;

        if (first_ppm->width < second_ppm->width) {
            smaller_width = first_ppm->width;
        } else {
            smaller_width = second_ppm->width;
        }
        
        float rgb_diff = 0;

        if (first_ppm->height < second_ppm->height) {
            smaller_height = first_ppm->height;
        } else {
            smaller_height = second_ppm->height;
        }
            
        for (unsigned i = 0; i < smaller_width; i++) {
            for (unsigned j = 0; j < smaller_height; j++) {
                Pnm_rgb first_colors = (Pnm_rgb)methods->at(first_array, i, j);
                Pnm_rgb second_colors = (Pnm_rgb)methods->at(second_array, i, j);

                float red_value = ((float)first_colors->red / (float)first_ppm->denominator - (float)second_colors->red / (float)second_ppm->denominator);
                red_value *= red_value;

                float green_value = ((float)first_colors->green / (float)first_ppm->denominator - (float)second_colors->green / (float)second_ppm->denominator);
                green_value *= green_value;

                float blue_value = ((float)first_colors->blue / (float)first_ppm->denominator - (float)second_colors->blue / (float)second_ppm->denominator);
                blue_value *= blue_value;

                rgb_diff += (red_value + green_value + blue_value);
            }
        }
        
        return sqrt(rgb_diff / (3 * smaller_width * smaller_height));
}