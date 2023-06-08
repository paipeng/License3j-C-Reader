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
    fileName = "/Users/paipeng/Development/Java/SoftwareLicense/d74c6b96-7814-463e-83dd-d66a39ebb765.license";
    QByteArray data;

    QFile keyFile(fileName);

    if (keyFile.open(QIODevice::ReadOnly)) {
        data = keyFile.readAll();
        keyFile.close();
    }

    QString publicKeyHexString ="007d5a9553241327d95ff3542bbb809f165123858133f19d985a1aa2820dd7d9bf82d6b6b4c27cc3b4ac3388cbc56702c454ad778f284a0119c3ff8f73d159571dee70390e0c8f6779f4bd22c744869e982574143c78e73da265900ae9af54e036147c9b219b7fc72b08d1fc475dc3b5a4b9c7531a05c2e9a9dd4e232eb49b07b70597509884d0a623f892d0205e5eee35af2a25745ca2ce9042ec4ca4e86c12a4312bf8ee4036ccd8210fcfbeb94ae4dad1ad1402d6ca882bf1051d331e8d54ead8d4e19cb71e151bb80a6476b5ac60a62095ce31134e213304b0a9ac448c57a8838c153a81123d4ddc8e179466930819483f7128aefca408e588c7f7faeaed0f6093f7ff25e0943eb7482cbfffb1834b82166d90a263f9aecb48e1aaaa7551f28df99afaa3bffabe60b97eb251c3153d4b2ab8cbb405fd336a11cd6144caafdaa7dec8038553009e0d0dfdea6732d7a2c03fe42da5ab72647b6853140ab74b3c9f252469891bfebcb8a16f67ed1f67f2c87c4e54a2e9d10f3c81ac98cfc072609a69438c234bb73cbb44d6fc95feb63276ed597d3c7bdc3cea8ea461e09895d3bf83d0a148a213a00f6ebc2e8ae6540d4b2e8df47a64fdd82cce4f3a79771c86051309142d88efb1c1706865bb07a8eefe2914d242581e671bbd25bb3bfcc2bf94387b5b25172aa587f17a2bf7fcfcab89fe7f3ca8523320f7ffae4a1179af";
    QByteArray pub = QByteArray::fromHex(publicKeyHexString.toUtf8());


    Feature* feature = License3C::parseLicense(data, pub);
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
