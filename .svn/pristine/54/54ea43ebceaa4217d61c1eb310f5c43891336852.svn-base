#ifndef __IMAGE_BUILDER
#define __IMAGE_BUILDER

#define RES 100
#include <iostream>
class ImageBuilder
{
public:
    void setup(std::vector<Colorer*> & imageColorers)
    {

        int count = 0;
        //fill some with black
	int gradient = 20;
	for(size_t i = 0; i < gradient; i++){
		
		size_t start = (i*imageColorers.size()/gradient);
		size_t end = ((i+1)*imageColorers.size()/gradient);
		for(size_t j = start; j <end;j++){
			count+=1;
			//std::cout << "Index: " << j << "\n\0";
			imageColorers.at(j) = new TrickyColorer(count%(i+1));
		}	


	}

    }
    
    void color(std::vector<Colorer*> const & imageColorers, std::vector<rgb> & rgbColors)
    {
        bool sizesMatch = imageColorers.size() == rgbColors.size();
        bool hasPixels = rgbColors.size() > 0;
        assert(sizesMatch);
        assert(hasPixels);
        
        for(size_t i=0; i<rgbColors.size(); i++)
            imageColorers.at(i)->colorPixel( rgbColors.at(i) );
    }
};

#endif
