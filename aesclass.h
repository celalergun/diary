#ifndef AESCLASS_H
#define AESCLASS_H

#include <QByteArray>
extern "C"
{
    #include "tiny-AES128-C/aes.h"
}

class AesClass
{
public:
    AesClass();
    QByteArray Encrypt(QByteArray Source, QByteArray Key);
    QByteArray Decrypt(QByteArray Source, QByteArray Key);
};

#endif // AESCLASS_H
