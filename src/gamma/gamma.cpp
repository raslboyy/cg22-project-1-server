#include "gamma/gamma.h"
#include "math.h"

namespace server::core::gamma {

PNM<ColorSpace::RGB> ConvertGamma(PNM<ColorSpace::RGB> image, double gamma) {
  if(gamma==image.gamma())
    return image;
  if(image.gamma()!=0){
    //from linear gamma to linear
    if(gamma!=0){
      int n = (int)image.height();
      int m = (int)image.width();
      for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
          image[i][j].red=pow(image[i][j].red,gamma/image.gamma());
          image[i][j].green=pow(image[i][j].green,gamma/image.gamma());
          image[i][j].blue=pow(image[i][j].blue,gamma/image.gamma());
        }
      image.set_gamma(gamma);
      return image;
    }
    //from linear to sRGB
    else{
      image = ConvertGamma(image, 1);
      int n = (int)image.height();
      int m = (int)image.width();
      for (int i = 0; i < n; i++)
        for (int j = 0; j < m; j++) {
          if(image[i][j].red<=0.0031308)
            image[i][j].red=image[i][j].red*12.92;
          else
            image[i][j].red=1.055*pow(image[i][j].red,1/2.4)-0.055;

          if(image[i][j].green<=0.0031308)
            image[i][j].green=image[i][j].green*12.92;
          else
            image[i][j].green=1.055*pow(image[i][j].green,1/2.4)-0.055;

          if(image[i][j].blue<=0.0031308)
            image[i][j].blue=image[i][j].blue*12.92;
          else
            image[i][j].blue=1.055*pow(image[i][j].blue,1/2.4)-0.05;
        }
      image.set_gamma(0);
      return image;
    }
  }
  //from sRGB to linear
  else{
    int n = (int)image.height();
    int m = (int)image.width();
    for (int i = 0; i < n; i++)
      for (int j = 0; j < m; j++) {
        if(image[i][j].red<=0.04045)
          image[i][j].red=image[i][j].red/12.92;
        else
          image[i][j].red=pow((image[i][j].red+0.055)/1.055,2.4);

        if(image[i][j].green<=0.04045)
          image[i][j].green=image[i][j].green/12.92;
        else
          image[i][j].green=pow((image[i][j].green+0.055)/1.055,2.4);
        if(image[i][j].blue<=0.04045)
          image[i][j].blue=image[i][j].blue/12.92;
        else
          image[i][j].blue=pow((image[i][j].blue+0.055)/1.055,2.4);
      }
    image.set_gamma(1);
    return ConvertGamma(image, gamma);
  }
}

}  // namespace server::core::gamma