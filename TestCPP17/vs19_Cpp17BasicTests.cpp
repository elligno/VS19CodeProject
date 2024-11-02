
#include "../Template17/vs19_Utilities.hpp"

namespace vsc19 
{
    void testShrPtrCount(const std::shared_ptr<int>& aShrPtr)
    {
        std::cout << "Count of the passed shared pointer is: " << aShrPtr.use_count() << "\n";
        std::shared_ptr<int> w_cpyShrPtr = aShrPtr;
        std::cout << "Count of the passed shared pointer (after copy) is: " << aShrPtr.use_count() << "\n";
        std::cout << "Count of the copy shared pointer is: " << w_cpyShrPtr.use_count() << "\n";
    }
    void copyShrPtr(std::shared_ptr<int> aShrPtr) 
    {
        // check count of the passed shared pointer

        // cheap operation when moving shared ptr 
        std::shared_ptr<int> w_mvdShrPtr = std::move(aShrPtr);

        // check count of the moved shared pointer
    }
    void SomeCppBasicTests() 
    { 
        // surprise that this work, let's check!!
        auto static constexpr count = 5;
        std::shared_ptr<int[]> w_testShrCArr {new int[count] {1,2,3,4,5}};
        for (auto i = 0; i < count; i++)
        {
            std::cout << "Value is: " << w_testShrCArr[i] << "\n";
        }

        std::shared_ptr<int> w_testShrCount {new int[count] {1,2,3,4,5}};
        for (auto i = 0; i < count; i++)
        {
            std::cout << "Value is: " << *w_testShrCount << "\n";
            (*w_testShrCount)++;
        }
        
        if (nullptr != w_testShrCount)
        {
            testShrPtrCount(w_testShrCount); // is that work
        }

        std::cout << "Count exited shared pointer is: " << w_testShrCount.use_count() << "\n";

        // count shall be set to 0 after move operation (very cheap operation)
        auto w_testAnotherShr = std::move(w_testShrCount);

        std::cout << "Count of the move shared pointer is: " << w_testShrCount.use_count() << "\n";

        std::cout << "End of Some basics trests\n";

        // test pass by-value
    }
} // End of namespace