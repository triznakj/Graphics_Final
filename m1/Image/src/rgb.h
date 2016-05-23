#ifndef __RGB
#define __RGB

struct rgb
{
    unsigned char c[3];
    
    void set(unsigned char r, unsigned char g, unsigned char b)
    {
        this->c[0] = r;
        this->c[1] = g;
        this->c[2] = b;
    }
    
    void set(unsigned char const * color)
    {
        for(size_t i=0; i<3; i++)
            this->c[i] = color[i];
    }
};

#endif
