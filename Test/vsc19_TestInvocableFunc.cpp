
#include "../Template17/vs19_Utilities.hpp"

namespace vsc19 
{
    void testCallIfCallable()
    {
        // Lambda function
        auto printMessage = []()
        {
            std::cout << "Hello, C++20 Concepts!n";
        };

        // Using the function that accepts a std::regular_invocable
      //  computeAndPrint(squareRoot, 16.0); // Valid, as squareRoot is side-effect free and consistent

        // Lambda example
        auto lambda = [](double x)
        { return x * x; };
        computeAndPrint(lambda, 4.0); // Also valid for the same reasons

        // Pass the lambda to the function template
        myFunction(printMessage);

        // Function pointer
        void (*funcPtr)() = []() { 
           std::cout << "Function pointer called.n"; 
       };

       // Pass the function pointer to the function template
       myFunction(funcPtr);

    // Uncommenting the following line will cause a compile-time error
    // myFunction(5); // Error: '5' does not satisfy std::invocable
    }
} // End of namespace