#include <cstdint>
#include <cstdio>
#define STEG_HEADER_SIZE size * 8

enum ImageType
{
    PNG,
    JPEG,
    BMP,
    HDR,
    JPEG_2000_STANDARD,
    WEBP_RIFF,
    UNKNOWN
};

struct ImageProcess
{
    unsigned char *data;

    size_t size = 0;
    int height;
    int width;
    int components;

    bool read(const char *filename);
    bool writeTo(const char *filename);
    ImageProcess(const char *filename);
    ImageProcess(int w, int h, int channels);
    ImageProcess();

    ImageType fetchFileType(const char *filename);
    void identifyImageType(const char *filename);
    ImageType extractedFirst8bits(const char *filename);
    ImageProcess &GrayScaleLuminosity();
    ImageProcess &GrayScaleLightness();
    ImageProcess &GrayScaleAverage();
    ImageProcess &FlipX();
    ImageProcess &FlipY();
    ImageProcess &steganographEnc(const char *message);
    char *steganographDec(size_t mesageLength);
    ImageProcess &HistogramAdjustments();
    ImageProcess &Convolution();
    ImageProcess &colorMask(int r, int g, int b, float alpha);
    ImageProcess &Invert();
};
