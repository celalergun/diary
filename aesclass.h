#ifndef AESCLASS_H
#define AESCLASS_H

#include <QByteArray>
extern "C"
{
    #include "tiny-AES128-C/aes.h"
}

class AesClass
{
private:
    static int Div2Ceil(int x, int y);
public:
    AesClass();
    static QByteArray Encrypt(QByteArray Source, QByteArray Key);
    static QByteArray Decrypt(QByteArray Source, QByteArray Key);
};

#endif // AESCLASS_H
