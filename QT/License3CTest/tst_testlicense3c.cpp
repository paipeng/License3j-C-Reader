#include <QtTest>
#include "license3c.h"

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

    License3C::parseLicense(data);
}

QTEST_APPLESS_MAIN(TestLicense3C)

#include "tst_testlicense3c.moc"
