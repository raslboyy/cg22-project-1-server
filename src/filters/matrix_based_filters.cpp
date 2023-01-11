#define _USE_MATH_DEFINES
#include "matrix_based_filters.h"
#include "algorithm"
#include "vector"
#include "math.h"

PNM<ColorSpace::RGB> MedianFilter(PNM<ColorSpace::RGB> image, int radius) {
  int diameter = radius*2;
  std::vector<double> matrix;
  int n = (int)image.height();
  int m = (int)image.width();
  //solving border problem
  double extended_image[n+diameter][m+diameter];
  for(int i = 0; i < n+diameter; i++)
    for(int j = 0; j < m+diameter; j++){
      if(i < radius){
        if(j < radius)
          extended_image[i][j] = image[0][0]
        else if(j > m+radius)
          extended_image[i][j] = image[0][m-1]
        else
          extended_image[i][j] = image[0][j]
      }
      else if(i > n+radius){
        if(j < radius)
          extended_image[i][j] = image[n-1][0]
        else if(j > m+radius)
          extended_image[i][j] = image[n-1][m-1]
        else
          extended_image[i][j] = image[n-1][j]
      }
      else{
        if(j < radius)
          extended_image[i][j] = image[i][0]
        else if(j > m+radius)
          extended_image[i][j] = image[i][m-1]
        else
          extended_image[i][j] = image[i][j]
      }
    }

  for(int i = radius; i < n+radius; i++)
    for(int j = radius; j < m+radius; j++){
      //calculating matrix for pixel
      for(int l = i-radius; l < i+radius; l++){
        for(int k = j-radius; k < j+radius; k++){
          matrix.push_back(extended_image[l][k]);
        }
      }
      //median filtering
      std::sort(matrix.begin(), matrix.end())
      image[i-radius][j-radius] = matrix[radius*radius/2+1];
      matrix.clear();
    }
  return image;
}

PNM<ColorSpace::RGB> BoxFilter(PNM<ColorSpace::RGB> image, int radius) {
  int diameter = radius*2;
  int n = (int)image.height();
  int m = (int)image.width();
  //solving border problem
  double extended_image[n+diameter][m+diameter];
  for(int i = 0; i < n+diameter; i++)
    for(int j = 0; j < m+diameter; j++){
      if(i < radius){
        if(j < radius)
          extended_image[i][j] = image[0][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[0][m-1]
              else
              extended_image[i][j] = image[0][j]
      }
      else if(i > n+radius){
        if(j < radius)
          extended_image[i][j] = image[n-1][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[n-1][m-1]
              else
              extended_image[i][j] = image[n-1][j]
      }
      else{
        if(j < radius)
          extended_image[i][j] = image[i][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[i][m-1]
              else
              extended_image[i][j] = image[i][j]
      }
    }

  double mean = 0;
  for(int i = radius; i < n+radius; i++)
    for(int j = radius; j < m+radius; j++){
      for(int l = i-radius; l < i+radius; l++){
        for(int k = j-radius; k < j+radius; k++){
          mean+=extended_image[l][k];
        }
      }
      //box filtering
      mean/=radius*radius;
      image[i-radius][j-radius] = mean;
      mean = 0;
    }
  return image;
}

PNM<ColorSpace::RGB> BoxFilter(PNM<ColorSpace::RGB> image, double sigma) {
  int radius = int(sigma*3);
  int diameter = radius*2;
  int n = (int)image.height();
  int m = (int)image.width();
  //solving border problem
  double extended_image[n+diameter][m+diameter];
  for(int i = 0; i < n+diameter; i++)
    for(int j = 0; j < m+diameter; j++){
      if(i < radius){
        if(j < radius)
          extended_image[i][j] = image[0][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[0][m-1]
              else
              extended_image[i][j] = image[0][j]
      }
      else if(i > n+radius){
        if(j < radius)
          extended_image[i][j] = image[n-1][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[n-1][m-1]
              else
              extended_image[i][j] = image[n-1][j]
      }
      else{
        if(j < radius)
          extended_image[i][j] = image[i][0]
              else if(j > m+radius)
                  extended_image[i][j] = image[i][m-1]
              else
              extended_image[i][j] = image[i][j]
      }
    }

  double gaussian = 0;
  for(int i = radius; i < n+radius; i++)
    for(int j = radius; j < m+radius; j++){
      for(int l = i-radius; l < i+radius; l++){
        for(int k = j-radius; k < j+radius; k++){
          gaussian+=extended_image[l][k]*
                      //calculating Gauss distribution
                      (1/(sqrt(2*M_PI)*sigma)*pow(M_E, (-(pow(k-radius,2)+pow(l-radius, 2))/(2*sigma*sigma))))
        }
      }
      //Gauss filtering
      image[i-radius][j-radius] = gaussian;
      gaussian = 0;
    }
  return image;
}
