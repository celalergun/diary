#include "aesclass.h"

int AesClass::Div2Ceil(int x, int y)
{
    return ((x + y - 1) / y);
}

AesClass::AesClass()
{
}

const uint32_t KEYSIZE = 16;

extern "C" void AES128_ECB_decrypt(const uint8_t* input, const uint8_t* key, uint8_t *output);
extern "C" void AES128_ECB_encrypt(const uint8_t* input, const uint8_t* key, uint8_t* output);

QByteArray AesClass::Encrypt(QByteArray Source, QByteArray Key)
{
    int steps = Div2Ceil(Source.count(), KEYSIZE);
    int fullSize = steps * KEYSIZE;
    int diff = steps * KEYSIZE - Source.count();
    for (int i = 0; i < diff; ++i)
    {
        Source.append((char)0x00);
    }
    QByteArray result;
    result.reserve(fullSize);
    result.fill(0, fullSize);
    QByteArray iv("9876543210akemil");
    AES128_CBC_encrypt_buffer((uint8_t*)result.data(), (uint8_t*)Source.data(), fullSize, (uint8_t*)Key.data(), (uint8_t*) iv.data());
    return result;
}

QByteArray AesClass::Decrypt(QByteArray Source, QByteArray Key)
{
    int steps = Div2Ceil(Source.count(), KEYSIZE);
    int fullSize = steps * KEYSIZE;

    QByteArray result;
    result.reserve(fullSize);
    result.fill(0, fullSize);
    QByteArray iv("9876543210akemil");
    AES128_CBC_decrypt_buffer((uint8_t*)result.data(), (uint8_t*)Source.data(), fullSize, (uint8_t*)Key.data(), (uint8_t*) iv.data());
    return result;
}
