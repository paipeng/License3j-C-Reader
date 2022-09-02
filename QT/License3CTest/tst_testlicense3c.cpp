#include <QtTest>
#include "license3c.h"
#include <QDebug>

// add necessary includes here

class TestLicense3C : public QObject
{
    Q_OBJECT

public:
    TestLicense3C();
    ~TestLicense3C();

private slots:
    void initTestCase();
    void cleanupTestCase();
    void test_case1();

};

TestLicense3C::TestLicense3C()
{

}

TestLicense3C::~TestLicense3C()
{

}

void TestLicense3C::initTestCase()
{

}

void TestLicense3C::cleanupTestCase()
{

}

void TestLicense3C::test_case1()
{
    QString fileName = "/Users/paipeng/Downloads/898b0954-2504-46d9-aa67-c531f4577589.license";
    QByteArray data;

    QFile keyFile(fileName);

    if (keyFile.open(QIODevice::ReadOnly)) {
        data = keyFile.readAll();
        keyFile.close();
    }

    Feature* feature = License3C::parseLicense(data);
    if (feature != NULL) {
#if 1
        qDebug() << "check feature\n\n\n";
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


        freeFeatures(feature);
#endif
    }
}

QTEST_APPLESS_MAIN(TestLicense3C)

#include "tst_testlicense3c.moc"
