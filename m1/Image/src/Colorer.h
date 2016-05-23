#ifndef __COLORER
#define __COLORER

#include "rgb.h"

struct Colorer
{
    virtual void colorPixel(rgb & pixel) = 0; //this is an abstract method
};

struct BlackColorer : Colorer
{
    virtual void colorPixel(rgb & pixel)
    { pixel.set(0,0,0); }
};

struct WhiteColorer : Colorer
{
    virtual void colorPixel(rgb & pixel)
    { pixel.set(255,255,255); }
};

struct TrickyColorer : Colorer
{
    unsigned int offset;
    unsigned int val;
    
    TrickyColorer(unsigned int seed = 1)
    {
        this->val = seed;
        this->offset = 997;
        this->getNext();
    }
    
    virtual void colorPixel(rgb & pixel)
    {
        unsigned char r = getNext();
        unsigned char g = getNext();
        unsigned char b = getNext();
        
        pixel.set(r,g,b);
    }
    
    unsigned char getNext()
    {
        this->val += offset*this->val;
        return this->val % 255;
    }
};

//TODO 2: Add as many of these Colorers as you need

#endif
