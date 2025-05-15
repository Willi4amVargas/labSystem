#include "include/LabTests.h"

int LabTest::cantLabTests = 0;

LabTest::LabTest(string testName, string testDescription, string testValue, float testPrice, vector<pair<Products *, int>> usageProducts, vector<pair<int, int>> testRanges)
{
    this->testName = testName;
    this->testDescription = testDescription;
    this->testValue = testValue;
    this->testPrice = testPrice;
    this->usageProducts = usageProducts;
    this->testRanges = testRanges;

    LabTest::cantLabTests++;
}

void LabTest::getLabTests()
{
    int testsRangesSize = this->testRanges.size();
    int usageProductsSize = this->usageProducts.size();
    cout << "\x1b[0J" << this->testName << "\t" << this->testDescription << "\t" << this->testPrice << "\tVALOR: " << this->testValue << "\n\t\x1b[0JPRODUCTOS USADOS: \n";
    // for (int a = 0; a < usageProductsSize; a++)
    // {
    //     Products *p = (*this->labProducts)this->usageProducts[a].first;
    //     cout << "\x1b[0J\t\t" << a << "\t" << this->usageProducts[a].first.getName() << " -> " << this->usageProducts[a].second << "\n";
    // }
    cout << "\t\x1b[0JRANGOS NORMALES: \n";
    for (int i = 0; i < testsRangesSize; i++)
    {
        cout << "\x1b[0J\t\t" << i << "\t" << this->testRanges[i].first << this->testValue << " -> " << this->testRanges[i].second << this->testValue << "\n";
    }
}