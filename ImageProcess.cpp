#define STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "Image.h"
#include "stb_image.h"
#include "stb_image_write.h"
#include <vector>
#include <fstream>
#include <algorithm>
#include <iostream>
#include <cmath>
// #include "encryption.h"

template <typename T>
T clamp(T value, T min, T max)
{
    if (value < min)
        return min;
    if (value > max)
        return max;
    return value;
}

ImageProcess::ImageProcess(const char *filename)
{
    printf("Reading %s\n", filename);

    if (read(filename))
    {

        printf("Done reading...... \n");
        size = width * height * components;
    }
    else
    {
        printf("Failed to read %s\n", filename);
    }
}
bool ImageProcess::read(const char *filename)
{
    data = stbi_load(filename, &width, &height, &components, 0);
    return (data == nullptr) ? nullptr : data;
}

void ImageProcess::identifyImageType(const char *filename)
{
    ImageType type = extractedFirst8bits(filename);
    switch (type)
    {
    case PNG:
        printf("The file type detected is PNG.");
        break;
    case BMP:
        printf("The file type detected is BMP.");
        break;
    case JPEG:
        printf("The file type detected is JPEG.");
        break;
    case HDR:
        printf("The file type detected is HDR.");
        break;
    case WEBP_RIFF:
        printf("The file type detected is WEBP(RIFF)");
    default:
        printf("Unknown file type detected.");
        break;
    }
}
ImageType ImageProcess::fetchFileType(const char *filename)
{
    std::string s = filename;
    std::string delimiter = ".";
    std::string token = s.substr(s.find(delimiter), s.length() - 1);
    // if (token == npos) // https://en.cppreference.com/w/cpp/string/basic_string/npos
    {
        if (token == ".png")
        {
            return PNG;
        }
        if (token == ".png")
        {
            return PNG;
        }
        if (token == ".jpeg" || token == ".jpg")
        {
            return JPEG;
        }
        if (token == ".hdr" || token == ".avif")
        {
            return HDR;
        }
        if (token == ".bmp")
        {
            return BMP;
        }
    }

    return UNKNOWN;
}

bool ImageProcess::writeTo(const char *filename)
{
    ImageType type = fetchFileType(filename);
    int ImageData = 0;
    switch (type)
    {

    case PNG:
        ImageData = stbi_write_png(filename, width, height, components, data, width * components);
        break;
    case BMP:
        ImageData = stbi_write_bmp(filename, width, height, components, data);
        break;
    case JPEG:
        ImageData = stbi_write_jpg(filename, width, height, components, data, 100);
        break;
    case HDR:
        ImageData = stbi_write_hdr(filename, width, height, components, (float *)data); // make data float later
        break;

    default:
        printf("Unknown file type provided.");
        break;
    }
    if (ImageData != 0)
        printf("Done converting.");
    return ImageData != 0;
}

ImageType ImageProcess::extractedFirst8bits(char const *filename)
{
    uint8_t headerSize = 45;
    std::vector<char> bytes(headerSize);
    std::ifstream file;
    file.open(filename, std::ifstream::binary);

    if (file)
    {
        file.read(reinterpret_cast<char *>(bytes.data()), headerSize);
    }

    if (bytes[0] == '\x89' && bytes[1] == '\x50' && bytes[2] == '\x4E' && bytes[3] == '\x47' && bytes[4] == '\x0D' && bytes[5] == '\x0A' && bytes[6] == '\x1A' && bytes[7] == '\x0A')
    {
        return PNG;
    }
    else if (bytes[0] == '\xFF' && bytes[1] == '\xD8' && bytes[2] == '\xFF' &&
             (bytes[3] == '\xE0' || bytes[3] == '\xE1') && bytes[6] == 'J' && bytes[7] == 'F' && bytes[8] == 'I' && bytes[9] == 'F')
    {
        return JPEG;
    }

    else if (bytes[0] == '\x00' && bytes[1] == '\x00' && bytes[2] == '\x00' && bytes[3] == '\x0C' && bytes[4] == '\x6A' && bytes[5] == '\x50' && bytes[6] == '\x20' && bytes[7] == '\x20' && bytes[8] == '\x0D' && bytes[9] == '\x0A' && bytes[10] == '\x87' && bytes[11] == '\x0A')
    {
        return JPEG_2000_STANDARD;
    }
    else if (bytes[0] == '\x52' && bytes[1] == '\x49' && bytes[2] == '\x46' && bytes[3] == '\x46' && bytes[8] == '\x57' && bytes[9] == '\x45' && bytes[10] == '\x42' && bytes[11] == '\x50')
    {
        return WEBP_RIFF;
    }
    else if (bytes[0] == '\x42' && bytes[1] == '\x4D')
    {
        return BMP;
    }
    else if (bytes[0] == '\x23' && bytes[1] == '\x3F' && bytes[2] == '\x52' && bytes[3] == '\x41' && bytes[4] == '\x44' && bytes[5] == '\x49' && bytes[6] == '\x41' && bytes[6] == '\x4E' && bytes[7] == '\x43' && bytes[8] == '\x45' && bytes[9] == '\x0A')
    {
        return HDR;
    }
    return UNKNOWN;
}

ImageProcess &ImageProcess::GrayScaleLuminosity()
{
    for (int i = 0; i < size; i += components)
    {
        unsigned char grayData = ((data[i + 0] * 0.2126) + (data[i + 1] * 0.7152) + (data[i + 2] * 0.0722));
        memset(data + i, grayData, 3);
    }
    return *this;
}
ImageProcess &ImageProcess::GrayScaleLightness()
{
    for (int i = 0; i < size; i += components)
    {
        unsigned char grayData = (std::max({data[i + 0], data[i + 1], data[i + 2]}) + std::min({data[i + 0], data[i + 1], data[i + 2]})) / 2;
        memset(data + i, grayData, 3);
    }
    return *this;
}
ImageProcess &ImageProcess::GrayScaleAverage()
{
    for (int i = 0; i < size; i += components)
    {
        unsigned char grayData = ((data[i + 0]) + (data[i + 1]) + (data[i + 2])) / 3;
        memset(data + i, grayData, 3);
    }
    return *this;
}
ImageProcess &ImageProcess::FlipX()
{
    unsigned char *temp[4];
    unsigned char *pixelLeft;
    unsigned char *pixelRight;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            pixelLeft = &data[(j + i * width) * components];
            pixelRight = &data[((width - 1 - j) + i * width) * components];
            memcpy(temp, pixelRight, components);
            memcpy(pixelRight, pixelLeft, components);
            memcpy(pixelLeft, temp, components);
        }
    }
    return *this;
}
ImageProcess &ImageProcess::FlipY()
{
    unsigned char *temp[4];
    unsigned char *pixelTop;
    unsigned char *pixelBottom;
    for (int i = 0; i < width; i++)
    {
        for (int j = 0; j < height / 2; j++)
        {
            pixelTop = &data[(i + j * width) * components];
            pixelBottom = &data[(i + (height - 1 - j) * width) * components];
            memcpy(temp, pixelTop, components);
            memcpy(pixelTop, pixelBottom, components);
            memcpy(pixelBottom, temp, components);
        }
    }
    return *this;
}
ImageProcess &ImageProcess::steganographEnc(const char *message)
{
    int messageLen = strlen(message) * 8;
    int dataIndex = 0;
    if (messageLen > size * 8)
    {
        printf("**Operation failed\n");
        printf("Either due to:\n");
        printf("1. Image doesnt exist.\n");
        printf("1. Either text is too large or image is too small.\n");
        exit(EXIT_FAILURE);
    }
    for (int i = 0; i < messageLen; i++)
    {
        if (messageLen > dataIndex)
        {
            unsigned char messageBit = (message[dataIndex / 8] >> (7 - (dataIndex % 8))) & 1; // extracting evry bit from message[n]
            data[i] = (data[i] & 0xFE) | messageBit;                                          // doing or to store messageBit
        }
        dataIndex++;
    }
    printf("Encryption successful.\n");
    return *this;
}
char *ImageProcess::steganographDec(size_t messageLength)
{
    size_t messageSize = (messageLength + 7) / 8; // total number of bytes needed
    char *message = new char[messageSize + 1];    // \0
    memset(message, 0, messageSize + 1);
    size_t dataIndex = 0;
    messageLength = messageLength * 8;
    unsigned char messageBit;
    for (int i = 0; i < size * 8; i++)
    {
        if (dataIndex < messageLength)
        {
            unsigned char messageBit = (data[i] & 1);
            message[dataIndex / 8] |= (messageBit << (7 - (dataIndex % 8)));
            dataIndex++;
        }
    }
    message[messageSize] = '\0';
    // printf("%s", message);
    printf("Decryption successful.\n");
    return message;
}
ImageProcess &ImageProcess::HistogramAdjustments()
{
    const unsigned char *GrayScale = data;
    unsigned char histogram[256] = {0};
    double pdf[256] = {0.0};
    for (int i = 0; i < size; i++)
    {
        int intensity = GrayScale[i];
        histogram[intensity]++;
    }

    for (int i = 0; i <= 255; i++)
    {
        pdf[i] = static_cast<double>(histogram[i]) / (width * height); //(Number of pixel in that intesity level)/(Total number of pixel)
    } // = Probability Density Function
    int sum = 0;
    for (int i = 1; i <= 256; i++)
    {
        pdf[i] += pdf[i - 1]; // cdf= Previous cdf + next pdf
    }
    for (int i = 0; i <= 255; i++)
    {
        data[i] = pdf[i] * 256; // histogram equalizer
    }
    return *this;
}
ImageProcess &ImageProcess::Convolution()
{

    return *this;
}
float toLinear(float value)
{
    return std::pow(value / 255.0f, 2.2f);
}

unsigned char toSRGB(float value)
{
    return static_cast<unsigned char>(clamp(std::pow(value, 1.0f / 2.2f) * 255.0f, 0.0f, 255.0f)); // value overflowing
}
ImageProcess &ImageProcess::colorMask(int r, int g, int b, float alpha)
{
    if (components < 3)
    {
        printf("Image has less channels than expected.");
    }
    else
    {
        for (int i = 0; i < size; i += 3)
        {
            float srcr = toLinear(data[i]);
            float srcg = toLinear(data[i + 1]);
            float srcb = toLinear(data[i + 2]);
            float dstr = toLinear(r);
            float dstg = toLinear(g);
            float dstb = toLinear(b);

            float coutr = srcr * alpha + dstr * (1 - alpha);
            float coutg = srcg * alpha + dstg * (1 - alpha);
            float coutb = srcb * alpha + dstb * (1 - alpha);

            data[i] = toSRGB(coutr);
            data[i + 1] = toSRGB(coutg);
            data[i + 2] = toSRGB(coutb);
        }
    }
    return *this;
}
ImageProcess &ImageProcess::Invert()
{
    for (int i = 0; i < size; i += 3)
    {
        data[i] = 255 - data[i];
        data[i + 1] = 255 - data[i + 1];
        data[i + 2] = 255 - data[i + 2];
    }

    return *this;
}
/*
cryptoGraph::cryptoGraph()
{
    // generateRandom();
}
void cryptoGraph::generateRandom()
{
    printf("Generating a random key.....\n");
    RAND_bytes(key, AES_BLOCK_SIZE);
    printf("Generating a random IV.....\n");
    RAND_bytes(iv, AES_BLOCK_SIZE);
    printf("Done.\n");
    printf("The key: %s\n", key);
    printf("The IV: %s\n", iv);
    printf("**Do NOT share these for the ones not intended**\n **Keeping them safe is your responsiblity, \nirresponsiblity regarding it may cause data to get encrypted forever.**\n");
}
const char *cryptoGraph::encryptAES(const unsigned char *message, int messageLength, unsigned char *buffer)
{
    AES_KEY aesKey;
    AES_set_encrypt_key(reinterpret_cast<const unsigned char *>(key), 256, &aesKey);
    AES_cbc_encrypt(message, buffer, messageLength, &aesKey, iv, AES_ENCRYPT);
    return reinterpret_cast<const char *>(buffer);
}

const char *cryptoGraph::decryptAES(const unsigned char *buffer, int messageLength, unsigned char *key, unsigned char *iv, unsigned char *cipherText)
{
    AES_KEY aesKey;
    AES_set_decrypt_key(reinterpret_cast<const unsigned char *>(key), 256, &aesKey);
    AES_cbc_encrypt(buffer, cipherText, messageLength, &aesKey, iv, AES_DECRYPT);
    return reinterpret_cast<const char *>(cipherText);
}
*/