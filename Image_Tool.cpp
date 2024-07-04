#include <stdio.h>
#include <cstring>
#include <string>
#include <iostream>
// #include "encryption.h"
#include "Image.h"

void print_usage(const char *program_name)
{
    printf("Usage: %s [OPTIONS][destination filename][source file]\n", program_name);
    printf("Options:\n");
    // printf("Please write ! before command to overwrite source file [Not recommended: **would cause loss of original data].\n");
    printf("  -h, --help                      Show this help message and exit.\n");
    printf("  --convert,                      To convert the type of image.\n");
    printf("  --identity,                     To detect the type of image.\n");
    printf("  --stegEnc [message],            To steganograph the image.\n");
    printf("  --stegDec [message],            To steganograph the image.\n");
    printf("  --FlipY,                        To flip the image by Y axis .\n");
    printf("  --FlipX,                        To flip the Image by X axis.\n");
    printf("  --B&W,                          To apply GrayScale Filter(Luminosity method) onto image.\n");
    printf("  --B&W --am,                     To apply GrayScale Filter(Average method) onto image.\n");
    printf("  --B&W --lm,                     To apply GrayScale Filter(Lightness method) onto image.\n");
    printf("  --resize,                       To resize the image.\n");
    printf("For Adjustments:\n");
    printf("  --hA ,                          To apply Histogram adjustments on image.\n");
    printf("  --S,                            To apply sharpening filter onto image."); // in progress
    printf("  --cM [R] [G] [B] [strenght],    To apply color-mask on image[Value of Red][Value of Green][Value of Blue][Intensity of color mask].\n");
    printf("  --em,                           To apply Embross filter onto image.");       // in progress
    printf("  --gB,                           To apply Gaussian Blur filter onto image."); // in progress
}

int main(int argc, char **argv)
{
    if (argc != 8 && argc != 5)
        if (argc <= 1 || argc > 4)
        {
            print_usage("Image_Tool");
        }
    if (argc == 8)
    {
        if (strcmp(argv[1], "--cM") == 0)
        {
            int red, green, blue;
            float alpha;
            sscanf(argv[2], "%d", &red);
            sscanf(argv[3], "%d", &green);
            sscanf(argv[4], "%d", &blue);
            sscanf(argv[5], "%f", &alpha);

            ImageProcess img(argv[7]);
            if (alpha < 0 || alpha > 100)
            {
                printf("Please input the value between the scale range 0 to 100.\n");
                EXIT_FAILURE;
            }
            printf("Masking the color on image.....\n");
            printf("**Float values are acceptable and encouraged for precision.");
            img.colorMask(red, green, blue, (alpha / 100.0f));
            img.writeTo(argv[6]);
        }
    }
    if (argc == 5)
    {
        if (strcmp(argv[1], "--stegEnc") == 0)
        {
            ImageProcess img(argv[4]);
            // cryptoGraph crypt;
            unsigned char buffer[256];
            // const char *encrpytedData = crypt.encryptAES(reinterpret_cast<const unsigned char *>(argv[2]), strlen(argv[2]), buffer);
            // img.steganographEnc(encrpytedData);
            img.steganographEnc(argv[2]);
            img.writeTo(argv[3]);
        }
    }
    if (argc == 4)
    {
        if (strcmp(argv[1], "--convert") == 0)
        {
            ImageProcess img(argv[3]);
            img.writeTo(argv[2]);
        }

        if (strcmp(argv[1], "--BW") == 0)
        {
            printf("Converting the file......\n");
            printf("Outputing the image in same format or higher resolution format is recommended.\n");
            ImageProcess image(argv[3]);
            image.GrayScaleLuminosity();
            image.writeTo(argv[2]);
        }
        if (strcmp(argv[1], "--BWlm") == 0)
        {
            printf("Converting the file......\n");
            printf("Outputing the image in same format or higher resolution format is recommended.\n");
            ImageProcess image(argv[3]);
            image.GrayScaleLightness();
            image.writeTo(argv[2]);
        }
        if (strcmp(argv[1], "--BWam") == 0)
        {
            ImageProcess image(argv[3]);
            printf("Converting the file......\n");
            printf("**Outputing the image in same format or higher resolution format is recommended.\n");
            image.GrayScaleAverage();
            image.writeTo(argv[2]);
        }
        if (strcmp(argv[1], "--FlipX") == 0)
        {
            ImageProcess image(argv[3]);
            printf("Flipping the file by X-axis......\n");
            printf("**Outputing the image in same format or higher resolution format is recommended.\n");
            image.FlipX();
            image.writeTo(argv[2]);
        }
        if (strcmp(argv[1], "--FlipY") == 0)
        {
            ImageProcess image(argv[3]);
            printf("Flipping the file by Y-axis......\n");
            printf("**Outputing the image in same format or higher resolution format is recommended.\n");
            image.FlipY();
            image.writeTo(argv[2]);
        }

        if (strcmp(argv[1], "--stegDec") == 0)
        {
            // printf("Recahed here");
            ImageProcess img(argv[3]);
            // size_t size = reinterpret_cast<size_t>(argv[2]);
            // char *message = img.steganographDec(size);
            // std::cout << message;
            size_t dataLengthBits = (7 * std::stoi(argv[2]));
            // std::stoi(argv[2]) * 7;
            //  char message;
            //  std::cout << dataLengthBits;
            char *decodedMessage = img.steganographDec(dataLengthBits);
            std::cout << "Decoded message: " << decodedMessage << std::endl;
            delete[] decodedMessage;
        }
    }
    if (argc == 3)
    {
        if (strcmp(argv[1], "--identify") == 0)
        {
            ImageProcess img(argv[2]);
            printf("Images File Type may be False positive, **can't guarantee\n");
            printf("Detecting the image......\n");
            img.identifyImageType(argv[2]);
        }

        else
        {
            print_usage("./Image_tool");
        }
    }
}