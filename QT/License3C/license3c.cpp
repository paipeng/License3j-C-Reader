#include "license3c.h"
#include <QDebug>
#include <QtEndian>
#include <qrsaencryption.h>
#include <QCryptographicHash>
#include <QDateTime>


void insertFeature(Feature* feature, Feature* nextFeature) {
    //qDebug() << "insertFeature";
    Feature* lastFeature = feature;
    while (lastFeature->next != NULL) {
        lastFeature = lastFeature->next;
    }

    lastFeature ->next = nextFeature;
}

void freeFeatures(Feature* feature) {
    Feature* nextFeature = feature->next;
    if (feature->name != NULL) {
        free(feature->name);
        feature->name = NULL;
    }
    if (feature->value != NULL) {
        free(feature->value);
        feature->value = NULL;
    }
    free(feature);
    feature = NULL;
    while (nextFeature != NULL) {
        Feature* tempFeature = nextFeature;
        if (tempFeature->name != NULL) {
            free(tempFeature->name);
            tempFeature->name = NULL;
        }
        if (tempFeature->value != NULL) {
            free(tempFeature->value);
            tempFeature->value = NULL;
        }
        free(tempFeature);
        tempFeature = NULL;
        nextFeature = nextFeature->next;
    }
}
License3C::License3C()
{
}

bool License3C::checkMagicBytes(const QByteArray magicByte) {
    qDebug() << "checkMagicBytes: " << magicByte.toHex();
    return (((unsigned char)magicByte.at(0) == 0x21) &&
            ((unsigned char)magicByte.at(1) == 0xCE) &&
            ((unsigned char)magicByte.at(2) == 0x4E) &&
            ((unsigned char)magicByte.at(3) == 0x5E));
}

bool License3C::isTypeVariableLength(int type) {
    return (type == Type::BINARY || type == Type::STRING || type == Type::BIGDECIMAL || type == Type::BIGINTEGER);
}

int License3C::getTypeLength(int type) {
    if (type == Type::BYTE) {
        return 1;
    } else if (type == Type::SHORT) {
        return 2;
    } else if (type == Type::INT) {
        return 4;
    } else if (type == Type::LONG) {
        return 8;
    } else if (type == Type::FLOAT) {
        return 4;
    } else if (type == Type::DOUBLE) {
        return 8;
    } else if (type == Type::DATE) {
        return 8;
    } else if (type == Type::UUID) {
        return 16;
    } else {
        return 0;
    }
}

Feature* License3C::parseLicense(const QByteArray& byteArray, const QByteArray& publicKeyBytes) {
    qDebug() << "parseLicense: ";// << byteArray;
    Feature* feature = NULL;
    QByteArray magicBytes = byteArray.mid(0, 4);
    if (checkMagicBytes(magicBytes)) {
        qDebug() << "magic bytes valid!";
        feature = (Feature*) malloc(sizeof (Feature));
        memset(feature, 0, sizeof(Feature));
        int pos = 4;
        do {
            Feature* newFeature = NULL;
            if (pos > 4) {
                qDebug() << "create next node";
                newFeature = (Feature*) malloc(sizeof (Feature));
                memset(newFeature, 0, sizeof(Feature));


            } else {
                newFeature = feature;
            }

            qDebug() << "read binary data";
            // Feature length 4Bytes
            QByteArray featureLength = byteArray.mid(pos, 4);
            pos += 4;
            int featureLen = qFromBigEndian<qint32>(featureLength.data());
            qDebug() << "featureLen: " << featureLen;

            // Feature type:  4 Bytes
            QByteArray featureType = byteArray.mid(pos, 4);
            pos += 4;
            newFeature->type = qFromBigEndian<qint32>(featureType.data());
            qDebug() << "featureType: " << newFeature->type;

            // Name length: 4Bytes
            QByteArray nameLength = byteArray.mid(pos, 4);
            pos += 4;
            newFeature->name_length = qFromBigEndian<qint32>(nameLength.data());
            qDebug() << "nameLen: " << newFeature->name_length;


            // Value Length: 4Bytes optional
            if (isTypeVariableLength(newFeature->type)) {
                QByteArray valueLength = byteArray.mid(pos, 4);
                pos += 4;
                newFeature->value_length = qFromBigEndian<qint32>(valueLength.data());
                qDebug() << "valueLen: " << newFeature->value_length;
            } else {
                newFeature->value_length = getTypeLength(newFeature->type);
            }

            // read name
            QByteArray name = byteArray.mid(pos, newFeature->name_length);
            pos += name.size();
            newFeature->name = (char*)malloc (sizeof(char) * name.size());
            memcpy(newFeature->name, name.data(), sizeof(char) * name.size());

            // read value
            QByteArray value = byteArray.mid(pos, newFeature->value_length);
            pos += value.size();
            newFeature->value = (char*)malloc (sizeof(char) * value.size());
            memcpy(newFeature->value, value.data(), sizeof(char) * value.size());

            if (pos > 40) {
             //   break;
            }

            // insert into node list
            if (newFeature != feature) {
                insertFeature(feature, newFeature);
            }
//            nextFeature = newFeature;
//            nextFeature = nextFeature->next;
        } while (pos < byteArray.size());



    } else {
        qDebug() << "magic bytes invalid!";
    }


    verify(feature, publicKeyBytes);
    return feature;
}

bool License3C::verify(Feature* feature, const QByteArray& publicKeyBytes) {
    qDebug() << "verify pub Hex: " << QString(publicKeyBytes.toHex());

    QRSAEncryption e(QRSAEncryption::Rsa::RSA_2048);

    if (feature != NULL) {
#if 1
        qDebug() << "check feature\n\n\n";
        QByteArray signature;
        QByteArray serialized;
        Feature* nextFeature = feature;
        do {
            qDebug() << "fetureType: " << nextFeature->type;
            qDebug() << "nameLen: " << nextFeature->name_length;
            qDebug() << "valueLen: " << nextFeature->value_length;
            qDebug() << "name: " <<  QString::fromLocal8Bit(nextFeature->name, nextFeature->name_length);
            if (nextFeature->type == 2) {
                qDebug() << "string value: " <<  QString::fromLocal8Bit(nextFeature->value, nextFeature->value_length);
            } else if (nextFeature->type == 11) {
                unsigned long t = 0;
                for (int i = 0; i < nextFeature->value_length; i++) {
                    //qDebug() << "date value: " << ((unsigned int)nextFeature->value[i] & 0xFF);
                    t += (unsigned long)(nextFeature->value[i] & 0xFF) << (nextFeature->value_length - i - 1) * 8;
                }

                qDebug() << "date value: " << t;
                QDateTime createTime;
                createTime.setTime_t((int)(t/1000));

                qDebug() << "expire date: " << createTime.toString("yyyy-MM-dd hh:mm:ss");
            } else if (nextFeature->type == 1) { //binary
                qDebug() << "binary hex: " << QByteArray::fromRawData(nextFeature->value, nextFeature->value_length).toHex();
                if (nextFeature->name_length == 16 && strcmp(nextFeature->name, "licenseSignature") == 0) {
                    signature = QByteArray::fromRawData(nextFeature->value, nextFeature->value_length);
                    qDebug() << "signature found: " << signature.toHex();
                }
            } else if (nextFeature->type == 5) { //int
                unsigned long t = 0;
                for (int i = 0; i < nextFeature->value_length; i++) {
                    //qDebug() << "date value: " << ((unsigned int)nextFeature->value[i] & 0xFF);
                    t += (unsigned long)(nextFeature->value[i] & 0xFF) << (nextFeature->value_length - i - 1) * 8;
                }
                qDebug() << "int value: " << t;
            }
            qDebug() << "\n";
            nextFeature = nextFeature->next;
        } while (nextFeature != NULL);

#if 1
        if (e.checkSignMessagePKCS15(serialized, signature, publicKeyBytes, QRSAEncryption::Auto)) {
            qDebug() << " message signed success";
            return true;
        } else {
            qDebug() << " message signed failed";
            return false;
        }
#endif
        return true;
#endif
    } else {
        return false;
    }




}
