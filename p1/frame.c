/**
   @file frame.c
   @author Prem Subedi
   This programs wraps the image with blue color circular frame. 
 */

#include <math.h>
#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>
#include "frame.h"
#define EXIT_STATUS1 100
#define EXIT_STATUS2 101
#define EXIT_STATUS3 102
#define LOCATION 0.5
#define MAX_INTENSITY 255
#define THREE_VALUE 3

double fRadius; /* Radius of the frame */
double cornerDist;  /* Distance from the center of the image to the pixel at the corner */

/**
   Shades the pixels based on their original color, frame color and distance from their center.
   @param color pixel's original color.
   @param borderColor new frame color to be added.
   @param dist pixel's distance from the center of the frame.
 */
void shade( int color, int borderColor, double dist ) {
   double w = (dist - fRadius) /(cornerDist - fRadius);
   double result = (double)(borderColor * w) + (double)(color * (1 - w)); 
   int newColor = round(result);
   printf("%4d", newColor);
}

/**
   Checks the file type of an image if it's starts with two characters P3 
   or not. It not it exits the program with exit value of 100.
 */
void checkFileType() {
   int c1 = getchar();
   int c2 = getchar();
   if (c1 != 'P' || c2 != '3') {
      exit(EXIT_STATUS1);
   }
   printf("%c", c1);
   printf("%c\n", c2);
}
  
/**
   Starting point of the program, which calls it's helper functions and adds the 
   frame over the image.
 */
int main() {
   checkFileType();
   int ch1;
   int ch2;
   
   if (scanf("%d %d", &ch1, &ch2) != 2) {
      exit(EXIT_STATUS2);
   }
   printf("%d ", ch1);
   printf("%d\n", ch2);
   if (ch1 < 2 || ch2 < 2) {
      exit(EXIT_STATUS2);
   }
   int maxIntensity;
   int value = scanf("%d\n", &maxIntensity);
   if (value != 1){
      exit(EXIT_STATUS2);
   }
   printf("%d\n", maxIntensity);
    
   double x = LOCATION;
   double y = LOCATION;

   double centerX = ch1/ 2;
   double centerY = ch2/ 2;
   if (ch1 <= ch2) {
      fRadius = (ch1 - 1) / 2;
   } else {
      fRadius = (ch2 - 1) / 2;
   }

   cornerDist = sqrt((x - centerX) * (x - centerX) + (y - centerY) * (y - centerY));
   
   for (int i = 0; i < ch2; i++ ) {
      double q = i + LOCATION;
      for (int j = 0; j < ch1; j++) {
         double p = j + LOCATION;
          
         double dist = sqrt((p - centerX) * (p - centerX) + (q - centerY) * (q - centerY));
         int color1;
         int color2;
         int color3;
         int numberOfColors = scanf("%d %d %d", &color1, &color2, &color3);
         if ( numberOfColors != THREE_VALUE) {
            exit(EXIT_STATUS3);
         }
         if (color1 < 0 || color1 > MAX_INTENSITY || color2 < 0 || 
            color2 > MAX_INTENSITY || color3 < 0 || color3 > MAX_INTENSITY) {
            exit(EXIT_STATUS3);
         }
         if (dist > fRadius && dist <= cornerDist) {
            shade(color1, FRAME_RED, dist);
            shade(color2, FRAME_GREEN, dist);
            shade(color3, FRAME_BLUE, dist);
         } else {
            printf("%4d%4d%4d", color1, color2, color3);
         } 
      }  
      printf("\n");
   } 
   return EXIT_SUCCESS;
}


