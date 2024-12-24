/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara_core_array.cpp
 *  \brief      Test application for the ara::core::Array template class.
 *
 *  \details    This file contains multiple test functions to validate the functionality of ara::core::Array.
 *              It covers:
 *                - Element access
 *                - Iterators
 *                - get<I>()
 *                - swap/fill
 *                - Comparison operators
 *                - User-defined types (class, struct)
 *                - Copy/move semantics
 *                - Const correctness
 *                - Out-of-range violation handling
 *                - Zero-sized arrays
 *                - Reverse iterators
 *                - Partial initialization
 *                - Negative test scenarios (compile-time & run-time)
 *********************************************************************************************************************/

#include "ara/core/array.h"  // Our array implementation
#include <iostream>
#include <string>
#include <cassert>

/**********************************************************************************************************************
 *  FORWARD DECLARATIONS OF TEST FUNCTIONS
 *********************************************************************************************************************/
void TestElementAccess();
void TestGetFunction();
void TestSwapAndFill();
void TestComparisonOperators();
void TestWithUserDefinedClass();
void TestWithUserDefinedStruct();
void TestCopyAndMoveSemantics();
void TestConstCorrectness();
void TestViolationHandling();
void TestZeroSizedArray();
void TestReverseIterators();
void TestPartialInitialization();

// Negative test demonstrations:
void TestNegativeScenarios();  
//   1) Passing too many arguments to the variadic constructor (compile-time).
//   2) Using get<I>() with I >= N (compile-time).
//   3) Attempting to swap arrays of different sizes (compile-time).
//   4) Attempting at() with an out-of-range index at run time, etc. (some are run-time only).

/**********************************************************************************************************************
 *  DEMO TYPES FOR TESTING
 *********************************************************************************************************************/

//--------------------------------
// 1. User-defined class
//--------------------------------
class TestClass {
public:
    TestClass() : value_(0) {
        std::cout << "TestClass Default Constructor\n";
    }

    explicit TestClass(int value) : value_(value) {
        std::cout << "TestClass Parameterized Constructor with value " << value_ << "\n";
    }

    // Copy Constructor
    TestClass(const TestClass& other) : value_(other.value_) {
        std::cout << "TestClass Copy Constructor\n";
    }

    // Move Constructor
    TestClass(TestClass&& other) noexcept : value_(other.value_) {
        other.value_ = 0;
        std::cout << "TestClass Move Constructor\n";
    }

    // Copy Assignment
    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value_ = other.value_;
            std::cout << "TestClass Copy Assignment\n";
        }
        return *this;
    }

    // Move Assignment
    TestClass& operator=(TestClass&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
            std::cout << "TestClass Move Assignment\n";
        }
        return *this;
    }

    // Comparison operators
    bool operator==(const TestClass& other) const {
        return value_ == other.value_;
    }
    bool operator!=(const TestClass& other) const {
        return !(*this == other);
    }
    bool operator<(const TestClass& other) const {
        return value_ < other.value_;
    }
    bool operator>(const TestClass& other) const {
        return other < *this;
    }
    bool operator<=(const TestClass& other) const {
        return !(other < *this);
    }
    bool operator>=(const TestClass& other) const {
        return !(*this < other);
    }

    int GetValue() const {
        return value_;
    }

private:
    int value_;
};

//--------------------------------
// 2. User-defined struct
//--------------------------------
struct TestStruct {
    int id;
    std::string name;

    // Comparison operators
    bool operator==(const TestStruct& other) const {
        return (id == other.id) && (name == other.name);
    }
    bool operator!=(const TestStruct& other) const {
        return !(*this == other);
    }
    bool operator<(const TestStruct& other) const {
        if (id != other.id) {
            return id < other.id;
        }
        return name < other.name;
    }
    bool operator>(const TestStruct& other) const {
        return other < *this;
    }
    bool operator<=(const TestStruct& other) const {
        return !(other < *this);
    }
    bool operator>=(const TestStruct& other) const {
        return !(*this < other);
    }
};

/**********************************************************************************************************************
 *  MAIN AND TEST MENU
 *********************************************************************************************************************/

static void DisplayUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [test_number]\n"
              << "Available Tests:\n"
              << "  1  - Test Element Access and Iterators\n"
              << "  2  - Test get<I>() Functionality\n"
              << "  3  - Test Swap and Fill Functions\n"
              << "  4  - Test Comparison Operators\n"
              << "  5  - Test with User-Defined Class\n"
              << "  6  - Test with User-Defined Struct\n"
              << "  7  - Test Copy and Move Semantics\n"
              << "  8  - Test Const Correctness\n"
              << "  9  - Test Violation Handling (Out-of-Range Access)\n"
              << " 10  - Test Zero-Sized Array\n"
              << " 11  - Test Reverse Iterators\n"
              << " 12  - Test Partial Initialization\n"
              << " 13  - Test Negative Scenarios (Disabled by default)\n"
              << "Example: " << programName << " 1\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        DisplayUsage(argv[0]);
        return 1;
    }

    std::string testNumber = argv[1];

    if      (testNumber == "1")  TestElementAccess();
    else if (testNumber == "2")  TestGetFunction();
    else if (testNumber == "3")  TestSwapAndFill();
    else if (testNumber == "4")  TestComparisonOperators();
    else if (testNumber == "5")  TestWithUserDefinedClass();
    else if (testNumber == "6")  TestWithUserDefinedStruct();
    else if (testNumber == "7")  TestCopyAndMoveSemantics();
    else if (testNumber == "8")  TestConstCorrectness();
    else if (testNumber == "9")  TestViolationHandling();
    else if (testNumber == "10") TestZeroSizedArray();
    else if (testNumber == "11") TestReverseIterators();
    else if (testNumber == "12") TestPartialInitialization();
    else if (testNumber == "13") {
        // Negative scenario demonstrations 
        TestNegativeScenarios();
    }
    else {
        std::cout << "Invalid test number.\n";
        DisplayUsage(argv[0]);
        return 1;
    }

    return 0;
}

/**********************************************************************************************************************
 *  TEST FUNCTIONS
 *********************************************************************************************************************/

//--------------------------------------------------------------------------------------------------
// 1. Test Element Access and Iterators
//--------------------------------------------------------------------------------------------------
void TestElementAccess() {
    std::cout << "\n=== Test 1: Element Access and Iterators ===\n";

    // Using variadic constructor to place exactly 5 ints:
    ara::core::Array<int, 5> intArray{10, 20, 30, 40, 50};

    // Access using at()
    std::cout << "Accessing at(2): " << intArray.at(2) << " (Expected: 30)\n";

    // Access using operator[]
    std::cout << "Accessing [0]: " << intArray[0] << " (Expected: 10)\n";

    // Iterate using iterators
    std::cout << "Iterating using iterators: ";
    for (auto it = intArray.begin(); it != intArray.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop: ";
    for (const auto& elem : intArray) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------
// 2. Test get<I>() Functionality
//--------------------------------------------------------------------------------------------------
void TestGetFunction() {
    std::cout << "\n=== Test 2: get<I>() Functionality ===\n";
    ara::core::Array<std::string, 3> stringArray = {"Alpha", "Beta", "Gamma"};

    // Access using get<I>
    std::cout << "get<0>(stringArray): " << ara::core::get<0>(stringArray) << " (Expected: Alpha)\n";
    std::cout << "get<2>(stringArray): " << ara::core::get<2>(stringArray) << " (Expected: Gamma)\n";
}

//--------------------------------------------------------------------------------------------------
// 3. Test Swap and Fill Functions
//--------------------------------------------------------------------------------------------------
void TestSwapAndFill() {
    std::cout << "\n=== Test 3: Swap and Fill Functions ===\n";
    ara::core::Array<int, 4> array1 = {1, 2, 3, 4};
    ara::core::Array<int, 4> array2 = {5, 6, 7, 8};

    std::cout << "Before swap:\n";
    std::cout << "array1: ";
    for (const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\narray2: ";
    for (const auto& elem : array2) std::cout << elem << " ";
    std::cout << "\n";

    swap(array1, array2);

    std::cout << "After swap:\n";
    std::cout << "array1: ";
    for (const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\narray2: ";
    for (const auto& elem : array2) std::cout << elem << " ";
    std::cout << "\n";

    // Test fill()
    array1.fill(100);
    std::cout << "After array1.fill(100): ";
    for (const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------
// 4. Test Comparison Operators
//--------------------------------------------------------------------------------------------------
void TestComparisonOperators() {
    std::cout << "\n=== Test 4: Comparison Operators ===\n";
    ara::core::Array<int, 3> arrayA = {1, 2, 3};
    ara::core::Array<int, 3> arrayB = {1, 2, 3};
    ara::core::Array<int, 3> arrayC = {1, 2, 4};

    // == 
    std::cout << "arrayA == arrayB: " << (arrayA == arrayB) << " (Expected: 1)\n";
    std::cout << "arrayA == arrayC: " << (arrayA == arrayC) << " (Expected: 0)\n";

    // !=
    std::cout << "arrayA != arrayB: " << (arrayA != arrayB) << " (Expected: 0)\n";
    std::cout << "arrayA != arrayC: " << (arrayA != arrayC) << " (Expected: 1)\n";

    // <
    std::cout << "arrayA < arrayC: " << (arrayA < arrayC) << " (Expected: 1)\n";
    std::cout << "arrayC < arrayA: " << (arrayC < arrayA) << " (Expected: 0)\n";

    // <=
    std::cout << "arrayA <= arrayB: " << (arrayA <= arrayB) << " (Expected: 1)\n";
    std::cout << "arrayA <= arrayC: " << (arrayA <= arrayC) << " (Expected: 1)\n";

    // >
    std::cout << "arrayA > arrayC: " << (arrayA > arrayC) << " (Expected: 0)\n";
    std::cout << "arrayC > arrayA: " << (arrayC > arrayA) << " (Expected: 1)\n";

    // >=
    std::cout << "arrayA >= arrayB: " << (arrayA >= arrayB) << " (Expected: 1)\n";
    std::cout << "arrayC >= arrayA: " << (arrayC >= arrayA) << " (Expected: 1)\n";
}

//--------------------------------------------------------------------------------------------------
// 5. Test with User-Defined Class
//--------------------------------------------------------------------------------------------------
void TestWithUserDefinedClass() {
    std::cout << "\n=== Test 5: Testing with User-Defined Class ===\n";
    // Demonstrate variadic constructor usage:
    ara::core::Array<TestClass, 3> classArray{TestClass(10), TestClass(20), TestClass(30)};

    // Access using at()
    std::cout << "classArray.at(1).GetValue(): " 
              << classArray.at(1).GetValue() << " (Expected: 20)\n";

    // Access using operator[]
    std::cout << "classArray[0].GetValue(): " 
              << classArray[0].GetValue() << " (Expected: 10)\n";

    // Iterate using iterators
    std::cout << "Iterating using iterators: ";
    for (auto it = classArray.begin(); it != classArray.end(); ++it) {
        std::cout << it->GetValue() << " ";
    }
    std::cout << "\n";

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop: ";
    for (const auto& elem : classArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------
// 6. Test with User-Defined Struct
//--------------------------------------------------------------------------------------------------
void TestWithUserDefinedStruct() {
    std::cout << "\n=== Test 6: Testing with User-Defined Struct ===\n";
    ara::core::Array<TestStruct, 3> structArray = {
        TestStruct{1, "Alice"}, 
        TestStruct{2, "Bob"}, 
        TestStruct{3, "Charlie"}
    };

    // Access using at()
    auto& refAt1 = structArray.at(1);
    std::cout << "structArray.at(1): ID=" << refAt1.id 
              << ", Name=" << refAt1.name << " (Expected: ID=2, Name=Bob)\n";

    // Access using operator[]
    std::cout << "structArray[0]: ID=" 
              << structArray[0].id << ", Name=" << structArray[0].name 
              << " (Expected: ID=1, Name=Alice)\n";

    // Iteration
    std::cout << "Iterating using iterators:\n";
    for (auto it = structArray.begin(); it != structArray.end(); ++it) {
        std::cout << "ID=" << it->id << ", Name=" << it->name << "\n";
    }

    // Range-based for
    std::cout << "Iterating using range-based for:\n";
    for (const auto& elem : structArray) {
        std::cout << "ID=" << elem.id << ", Name=" << elem.name << "\n";
    }
}

//--------------------------------------------------------------------------------------------------
// 7. Test Copy and Move Semantics
//--------------------------------------------------------------------------------------------------
void TestCopyAndMoveSemantics() {
    std::cout << "\n=== Test 7: Copy and Move Semantics ===\n";
    ara::core::Array<TestClass, 2> originalArray{TestClass(100), TestClass(200)};

    // Test Copy Constructor
    std::cout << "\n-- Testing Copy Constructor --\n";
    ara::core::Array<TestClass, 2> copiedArray = originalArray;

    // Test Move Constructor
    std::cout << "\n-- Testing Move Constructor --\n";
    ara::core::Array<TestClass, 2> movedArray = std::move(originalArray);

    // Verify moved array contents
    std::cout << "Moved Array Contents:\n";
    for (const auto& elem : movedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";

    // Verify original array after move
    std::cout << "Original Array after Move (Should be defaulted):\n";
    for (const auto& elem : originalArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";

    // Test Copy Assignment
    std::cout << "\n-- Testing Copy Assignment --\n";
    ara::core::Array<TestClass, 2> copyAssignedArray;
    copyAssignedArray = copiedArray;

    // Test Move Assignment
    std::cout << "\n-- Testing Move Assignment --\n";
    ara::core::Array<TestClass, 2> moveAssignedArray;
    moveAssignedArray = std::move(copiedArray);

    // Verify move-assigned array contents
    std::cout << "Move Assigned Array Contents:\n";
    for (const auto& elem : moveAssignedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";

    // copiedArray after move assignment
    std::cout << "Copied Array after Move Assignment (Should be defaulted):\n";
    for (const auto& elem : copiedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------
// 8. Test Const Correctness
//--------------------------------------------------------------------------------------------------
void TestConstCorrectness() {
    std::cout << "\n=== Test 8: Const Correctness ===\n";
    const ara::core::Array<int, 3> constArray = {7, 8, 9};

    // Access using at()
    std::cout << "constArray.at(1): " << constArray.at(1) << " (Expected: 8)\n";

    // Access using get<I>()
    std::cout << "get<2>(constArray): " 
              << ara::core::get<2>(constArray) << " (Expected: 9)\n";

    // Iterate using const_iterators
    std::cout << "Iterating using const_iterators: ";
    for (auto it = constArray.begin(); it != constArray.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Range-based for
    std::cout << "Iterating using range-based for: ";
    for (const auto& elem : constArray) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Attempting modifications => compilation error if uncommented
    // constArray.at(0) = 999; // not allowed
}

//--------------------------------------------------------------------------------------------------
// 9. Test Violation Handling (Out-of-Range Access)
//--------------------------------------------------------------------------------------------------
void TestViolationHandling() {
    std::cout << "\n=== Test 9: Violation Handling (Out-of-Range Access) ===\n";
    ara::core::Array<int, 3> violationArray = {10, 20, 30};

    // Valid
    std::cout << "violationArray.at(2): " 
              << violationArray.at(2) << " (Expected: 30)\n";

    // Out-of-range => triggers violation => terminates
    std::cout << "Attempting at(3) => should trigger violation.\n";
    violationArray.at(3);  // This triggers the violation + process termination
}

//--------------------------------------------------------------------------------------------------
// 10. Test Zero-Sized Array
//--------------------------------------------------------------------------------------------------
void TestZeroSizedArray() {
    std::cout << "\n=== Test 10: Zero-Sized Array ===\n";
    // If your array implementation supports N=0
    ara::core::Array<int, 0> zeroArray;

    std::cout << "zeroArray.size(): " << zeroArray.size() << " (Expected: 0)\n";
    std::cout << "zeroArray.empty(): " << zeroArray.empty() << " (Expected: 1)\n";

    // begin() == end() for zero-size arrays
    std::cout << "zeroArray.begin() == zeroArray.end() ? " 
              << (zeroArray.begin() == zeroArray.end()) << " (Expected: 1)\n";

    // data() => returns nullptr if N==0
    if (zeroArray.data() == nullptr) {
        std::cout << "zeroArray.data() is nullptr as expected.\n";
    }

    // fill() has no effect
    zeroArray.fill(42);
    std::cout << "After zeroArray.fill(42), still no elements to show.\n";

    // front(), back() => compile-time error if you try to call them.
    // zeroArray.front(); 
    // zeroArray.back();
}

//--------------------------------------------------------------------------------------------------
// 11. Test Reverse Iterators
//--------------------------------------------------------------------------------------------------
void TestReverseIterators() {
    std::cout << "\n=== Test 11: Reverse Iterators ===\n";
    ara::core::Array<int, 5> arr = {100, 200, 300, 400, 500};

    std::cout << "Forward iteration: ";
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    std::cout << "Reverse iteration using rbegin/rend: ";
    for (auto rit = arr.rbegin(); rit != arr.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    std::cout << "Const reverse iteration using crbegin/crend: ";
    const auto& constArr = arr; // example of const
    for (auto rit = constArr.crbegin(); rit != constArr.crend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";
}

//--------------------------------------------------------------------------------------------------
// 12. Test Partial Initialization
//--------------------------------------------------------------------------------------------------
void TestPartialInitialization() {
    std::cout << "\n=== Test 12: Partial Initialization ===\n";

    // Suppose array of length 5, but initializer_list has only 2 elements
    ara::core::Array<int, 5> partialInitArr = {1, 2};

    std::cout << "partialInitArr contents after 2-element init:\n";
    for (auto i = 0U; i < partialInitArr.size(); ++i) {
        std::cout << "Index " << i << ": " 
                  << partialInitArr[i] << "\n";
    }
    std::cout << "(Expected: first two are 1,2 and rest default-initialized to 0)\n";
}

//--------------------------------------------------------------------------------------------------
// 13. Negative Scenarios (some compile-time, some run-time)
//--------------------------------------------------------------------------------------------------
void TestNegativeScenarios() {
    std::cout << "\n=== Test 13: Negative Scenarios ===\n";

    // ------------------------------------------------------------------------------
    // 1) Too many arguments to constructor => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to initialize an Array with more elements than its size.
    // Expected Outcome:
    // Compile-time error due to exceeding the maximum number of allowed arguments.
    ara::core::Array<int,3> tooManyArgs(1, 2, 3, 4); // Error: Too many arguments
    */


    // ------------------------------------------------------------------------------
    // 2) get<I> with I >= N => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to access an element with a compile-time index out of bounds.
    // Expected Outcome:
    // Compile-time error triggered by static_assert in get<I>().
    ara::core::Array<int,3> myArray = {10, 20, 30};
    int x = ara::core::get<3>(myArray); // Error: get<3>() out of range
    */
    // ------------------------------------------------------------------------------
    // 3) Attempting swap with different-sized arrays => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to swap two Arrays of different sizes.
    // Expected Outcome:
    // Compile-time error triggered by static_assert in swap function.
    ara::core::Array<int,3> array3 = {1, 2, 3};
    ara::core::Array<int,4> array4 = {1, 2, 3, 4};
    swap(array3, array4); // Error: Cannot swap arrays of different type or size
    */

    // ------------------------------------------------------------------------------
    // 4) Out-of-range index on at() => run-time violation:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to access an element with a run-time index out of bounds using at().
    // Expected Outcome:
    // Runtime termination with a violation message.
    ara::core::Array<int,2> arrSmall = {5, 6};
    std::cout << "arrSmall.at(2) => should trigger out-of-range violation...\n";
    arrSmall.at(2); // Runtime Error: Array access out of range
    */
    
    // ------------------------------------------------------------------------------
    // 5) Wrong data type in constructor => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to initialize an Array with elements of an incompatible type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,2> typeMismatch("Hello", "World"); // Error: Cannot convert std::string to int
    */

    // ------------------------------------------------------------------------------
    // 6) Copy-constructing an Array from a different T => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to copy-construct an Array of one type from an Array of a different type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> intArray3 = {1, 2, 3};
    ara::core::Array<double,3> copyOfInts(intArray3); // Error: No matching constructor
    */
    // ------------------------------------------------------------------------------
    // 7) Copy-assigning an Array from a different T => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to copy-assign an Array of one type from an Array of a different type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> intArrayA = {10, 20, 30};
    ara::core::Array<double,3> dblArrayB = {1.5, 2.5, 3.5};
    dblArrayB = intArrayA;  // Error: Cannot assign Array<int,3> to Array<double,3>
    */

    // ------------------------------------------------------------------------------
    // 8) Copy-constructing or assigning an Array from a different size => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to copy-construct or copy-assign an Array from an Array of the same type but different size.
    // Expected Outcome:
    // Compile-time error due to size mismatch.
    ara::core::Array<int,3> arrSize3 = {1, 2, 3};
    ara::core::Array<int,4> arrSize4 = {5, 6, 7, 8};
    
    // Copy constructor scenario:
    ara::core::Array<int,4> copyFrom3(arrSize3);  // Error: No matching constructor
    
    // Copy assignment scenario:
    arrSize4 = arrSize3;  // Error: Cannot assign Array<int,3> to Array<int,4>
    */

    // ------------------------------------------------------------------------------
    // 9) Move-constructing an Array from a different T => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-construct an Array of one type from an Array of a different type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> intArray3 = {1, 2, 3};
    ara::core::Array<int,3> doubleArray3 = std::move(intArray3);
    ara::core::Array<double,3> moveCopy(std::move(doubleArray3)); // Error: No matching constructor
    */

    // ------------------------------------------------------------------------------
    // 10) Move-assigning an Array from a different T => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-assign an Array of one type from an Array of a different type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> intArrayA = {10, 20, 30};
    ara::core::Array<double,3> dblArrayB = {1.5, 2.5, 3.5};
    dblArrayB = std::move(intArrayA); // Error: Cannot assign Array<int,3> to Array<double,3>
    */
    
    // ------------------------------------------------------------------------------
    // 11) Move-constructing or assigning an Array from a different size => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-construct or move-assign an Array from an Array of the same type but different size.
    // Expected Outcome:
    // Compile-time error due to size mismatch.
    ara::core::Array<int,3> arrSize3 = {1, 2, 3};
    ara::core::Array<int,4> arrSize4 = {5, 6, 7, 8};
    
    // Move constructor scenario:
    ara::core::Array<int,4> moveCopyFrom3(std::move(arrSize3));  // Error: No matching constructor
    
    // Move assignment scenario:
    arrSize4 = std::move(arrSize3);  // Error: Cannot assign Array<int,3> to Array<int,4>
    */

    // ------------------------------------------------------------------------------
    // 12) Swapping Arrays of Different Types or Sizes => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to swap two Arrays where either the element types or sizes differ.
    // Expected Outcome:
    // Compile-time error due to type or size mismatch.
    ara::core::Array<int,3> array3 = {1, 2, 3};
    ara::core::Array<double,4> array4 = {1.1, 2.2, 3.3, 4.4};
    
    // Different type:
    swap(array3, array4); // Error: Cannot swap arrays of different type or size
    
    // Different size with same type:
    ara::core::Array<int,4> array4SameType = {4, 5, 6, 7};
    swap(array3, array4SameType); // Error: Cannot swap arrays of different type or size
    */

    // ------------------------------------------------------------------------------
    // 13) Accessing front() or back() on zero-sized Array => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to access front() or back() on an Array with size 0.
    // Expected Outcome:
    // Compile-time error triggered by static_assert in front() and back().
    ara::core::Array<int,0> emptyArray;
    emptyArray.front(); // Error: front() called on zero-sized Array
    emptyArray.back();  // Error: back() called on zero-sized Array
    */
    // ------------------------------------------------------------------------------
    // 14) Initializing Array with incompatible types via std::array-like initialization => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Using initializer lists with incompatible types.
    // Expected Outcome:
    // Compile-time error due to type mismatch and narrowing.
    ara::core::Array<int,4> array1 = {1.2, 2.3, 3.5, 4.8}; // Error: Narrowing conversion from double to int
    */
    // ------------------------------------------------------------------------------
    // 15) Attempting to initialize with fewer arguments and accessing uninitialized elements => run-time violation:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array with fewer arguments than its size; remaining elements are default-initialized.
    // Accessing uninitialized elements via at() to trigger run-time violation.
    ara::core::Array<int,3> partialInit = {10}; // Elements: {10, 0, 0}
    std::cout << "Accessing index 2 via at() (should be valid, but value is default-initialized): " << partialInit.at(2) << "\n";
    
    std::cout << "Accessing index 3 via at() (should trigger run-time violation)...\n";
    partialInit.at(3); // Runtime Error: Array access out of range
    */

    // ------------------------------------------------------------------------------
    // 16) Attempting to assign to Array with incompatible types using move semantics => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-assign an Array of one type to an Array of another type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> intArray = {1, 2, 3};
    ara::core::Array<std::string,3> strArray;
    strArray = std::move(intArray); // Error: Cannot assign Array<int,3> to Array<std::string,3>
    */

    // ------------------------------------------------------------------------------
    // 17) Attempting to initialize Array with non-convertible types => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array<int,2> with std::string, which is not convertible to int.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,2> invalidInit = {std::string("Hello"), std::string("World")}; // Error: Cannot convert std::string to int
    */

    // ------------------------------------------------------------------------------
    // 18) Attempting to initialize Array with mix of convertible and non-convertible types => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array<int,3> with a mix of convertible (double to int) and non-convertible (std::string to int) types.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> mixedInit = {1, 2.5, std::string("Three")}; // Error: std::string cannot be converted to int
    */
    // ------------------------------------------------------------------------------
    // 19) Attempting to move-construct Array with incompatible sizes and types => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-construct an Array from another Array with different size and type.
    // Expected Outcome:
    // Compile-time error due to type and size mismatch.
    ara::core::Array<int,3> intArray = {1, 2, 3};
    ara::core::Array<double,4> moveConstructArray(std::move(intArray)); // Error: No matching constructor
    */

    // ------------------------------------------------------------------------------
    // 20) Attempting to move-assign Array with different types and sizes => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to move-assign an Array of one type and size to an Array of another type and size.
    // Expected Outcome:
    // Compile-time error due to type and size mismatch.
    ara::core::Array<int,3> intArray = {1, 2, 3};
    ara::core::Array<double,4> moveAssignArray;
    moveAssignArray = std::move(intArray); // Error: Cannot assign Array<int,3> to Array<double,4>
    */

    // ------------------------------------------------------------------------------
    // 21) Attempting to initialize Array with initializer list exceeding N => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array<int,3> with an initializer list that has more elements than N.
    // Expected Outcome:
    // Compile-time error due to exceeding the maximum number of elements.
    ara::core::Array<int,3> arrayExceed = {1, 2, 3, 4}; // Error: Too many arguments
    */
    // ------------------------------------------------------------------------------
    // 22) Attempting to initialize Array with initializer list causing narrowing conversions => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array<unsigned int,3> with negative values which cause narrowing.
    // Expected Outcome:
    // Compile-time error due to narrowing conversions.
    ara::core::Array<unsigned int,3> arrayNegative = {-1, -2, -3}; // Error: Narrowing conversion from int to unsigned int
    */
    
    // ------------------------------------------------------------------------------
    // 23) Attempting to initialize Array with fewer arguments and accessing beyond initialized elements => run-time violation:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Initializing Array<int,5> with 3 elements; accessing index 4 via at() should be safe (default-initialized).
    // Accessing index 5 should trigger run-time violation.
    ara::core::Array<int,5> partialInit = {10, 20, 30};
    std::cout << "Accessing index 4 via at() (should be default-initialized): " << partialInit.at(4) << "\n";
    std::cout << "Accessing index 5 via at() (should trigger run-time violation)...\n";
    partialInit.at(5); // Runtime Error: Array access out of range
    */

    // ------------------------------------------------------------------------------
    // 24) Attempting to assign Array with incompatible types using copy semantics => compile-time error:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Attempting to copy-assign an Array of one type to an Array of another type.
    // Expected Outcome:
    // Compile-time error due to type mismatch.
    ara::core::Array<int,3> sourceArray = {1, 2, 3};
    ara::core::Array<std::string,3> targetArray;
    targetArray = sourceArray; // Error: Cannot assign Array<int,3> to Array<std::string,3>
    */

    // ------------------------------------------------------------------------------
    // 25) Attempting to initialize Array with default constructor and then accessing elements via at() => run-time behavior:
    // ------------------------------------------------------------------------------
    /*
    // Description:
    // Default-initializing an Array and accessing its elements via at().
    // Expected Outcome:
    // Accessing any element within bounds returns default-initialized value.
    // Accessing any element out of bounds triggers run-time violation.
    ara::core::Array<int,3> defaultInit;
    std::cout << "Accessing default-initialized index 0: " << defaultInit.at(0) << "\n"; // Should be 0
    std::cout << "Accessing default-initialized index 1: " << defaultInit.at(1) << "\n"; // Should be 0
    std::cout << "Accessing default-initialized index 2: " << defaultInit.at(2) << "\n"; // Should be 0
    std::cout << "Accessing default-initialized index 3 (out of bounds)...\n";
    defaultInit.at(3); // Runtime Error: Array access out of range
    */

    std::cout << "(All negative scenarios are currently commented out. "
                 "Uncomment each one individually to observe the intended compile-time or run-time failures.)\n"; 
}
