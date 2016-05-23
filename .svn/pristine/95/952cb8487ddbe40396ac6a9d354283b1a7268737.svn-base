#include <vector>
#include <assert.h>
#include <stdio.h>
#include "rgb.h"
#include "Colorer.h"
#include "ImageBuilder.h"
#include "simplePNG.h"

//You need to change some code in ImageBuilder.h

int main(int argc, char const** argv)
{
    std::vector<Colorer*> imageColorers;
    std::vector<rgb> rgbColors;
    
    imageColorers.resize(RES*RES);
    rgbColors.resize(RES*RES);
    
    ImageBuilder builder;
    builder.setup(imageColorers);
    builder.color(imageColorers, rgbColors);
    
    simplePNG_write("image.png", RES, RES, (uint8_t*)&rgbColors[0]);
    printf("Wrote your image!\n");
    
    return 0;
}
