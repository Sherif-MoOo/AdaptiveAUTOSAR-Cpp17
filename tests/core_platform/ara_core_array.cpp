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
 *  \brief      Comprehensive test application for the ara::core::Array template class.
 *
 *  \details    This file contains multiple test functions covering a wide range of scenarios:
 *              1.  Element access (checked vs. unchecked) and iteration
 *              2.  get<I>()
 *              3.  swap() and fill()
 *              4.  Comparison operators
 *              5.  Usage with user-defined class
 *              6.  Usage with user-defined struct
 *              7.  Copy and move semantics
 *              8.  Const correctness
 *              9.  Violation handling (out-of-range)
 *              10. Zero-sized arrays
 *              11. Reverse iterators
 *              12. Partial initialization
 *              13. Negative scenarios (compile-time & run-time) - commented out by default
 *              14. Two-dimensional (nested) arrays
 *
 *  \note       All variables are used, preventing compiler warnings about unused variables.
 *********************************************************************************************************************/

#include "ara/core/array.h" // The custom Array implementation header
#include <iostream>         // For std::cout (demonstrations)
#include <string>           // For std::string
#include <cassert>          // For runtime checks via assert

/**********************************************************************************************************************
 *  FORWARD DECLARATIONS
 *********************************************************************************************************************/
void TestElementAccessAndIterators();  // Test #1
void TestGetFunction();                // Test #2
void TestSwapAndFill();                // Test #3
void TestComparisonOperators();        // Test #4
void TestWithUserDefinedClass();       // Test #5
void TestWithUserDefinedStruct();      // Test #6
void TestCopyAndMoveSemantics();       // Test #7
void TestConstCorrectness();           // Test #8
void TestViolationHandling();          // Test #9
void TestZeroSizedArray();             // Test #10
void TestReverseIterators();           // Test #11
void TestPartialInitialization();      // Test #12
void TestNegativeScenarios();          // Test #13 (commented code)
void TestTwoDimensionalArrays();       // Test #14

/**********************************************************************************************************************
 *  DEMO TYPES FOR TESTING
 *********************************************************************************************************************/

/*!
 * \brief  A sample user-defined class to test copy, move, and comparison inside ara::core::Array
 */
class TestClass
{
public:
    // Default constructor
    TestClass() : value_(0) {
        std::cout << "[TestClass] Default Constructor\n";
    }

    // Parameterized constructor
    explicit TestClass(int value) : value_(value) {
        std::cout << "[TestClass] Param Constructor with value " << value << "\n";
    }

    // Copy constructor
    TestClass(const TestClass& other) : value_(other.value_) {
        std::cout << "[TestClass] Copy Constructor\n";
    }

    // Move constructor
    TestClass(TestClass&& other) noexcept : value_(other.value_) {
        other.value_ = 0;
        std::cout << "[TestClass] Move Constructor\n";
    }

    // Copy assignment
    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value_ = other.value_;
            std::cout << "[TestClass] Copy Assignment\n";
        }
        return *this;
    }

    // Move assignment
    TestClass& operator=(TestClass&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
            std::cout << "[TestClass] Move Assignment\n";
        }
        return *this;
    }

    // Comparisons
    bool operator==(const TestClass& rhs) const { return (value_ == rhs.value_); }
    bool operator!=(const TestClass& rhs) const { return !(*this == rhs); }
    bool operator<(const TestClass& rhs)  const { return value_ < rhs.value_; }
    bool operator>(const TestClass& rhs)  const { return rhs < *this; }
    bool operator<=(const TestClass& rhs) const { return !(rhs < *this); }
    bool operator>=(const TestClass& rhs) const { return !(*this < rhs); }

    // Accessor
    int GetValue() const { return value_; }

private:
    int value_;
};

/*!
 * \brief  A sample user-defined struct to test custom types in ara::core::Array
 */
struct TestStruct
{
    int         id;
    std::string name;

    bool operator==(const TestStruct& rhs) const {
        return (id == rhs.id) && (name == rhs.name);
    }
    bool operator!=(const TestStruct& rhs) const {
        return !(*this == rhs);
    }
    bool operator<(const TestStruct& rhs) const {
        if (id != rhs.id) {
            return (id < rhs.id);
        }
        return (name < rhs.name);
    }
    bool operator>(const TestStruct& rhs) const {
        return rhs < *this;
    }
    bool operator<=(const TestStruct& rhs) const {
        return !(rhs < *this);
    }
    bool operator>=(const TestStruct& rhs) const {
        return !(*this < rhs);
    }
};

/**********************************************************************************************************************
 *  MAIN AND MENU
 *********************************************************************************************************************/
static void PrintUsage(const char* prog) {
    std::cout << "Usage: " << prog << " [test_number]\n"
              << "List of Available Tests:\n"
              << "  1  - Element Access and Iterators\n"
              << "  2  - get<I>() Functionality\n"
              << "  3  - Swap and Fill\n"
              << "  4  - Comparison Operators\n"
              << "  5  - User-Defined Class\n"
              << "  6  - User-Defined Struct\n"
              << "  7  - Copy and Move Semantics\n"
              << "  8  - Const Correctness\n"
              << "  9  - Violation Handling (Out-of-Range)\n"
              << " 10  - Zero-Sized Array\n"
              << " 11  - Reverse Iterators\n"
              << " 12  - Partial Initialization\n"
              << " 13  - Negative Scenarios (commented out)\n"
              << " 14  - Two-Dimensional Arrays\n";
}

int main(int argc, char* argv[])
{
    if (argc != 2) {
        PrintUsage(argv[0]);
        return 1;
    }

    std::string choice = argv[1];
    if      (choice == "1")  TestElementAccessAndIterators();
    else if (choice == "2")  TestGetFunction();
    else if (choice == "3")  TestSwapAndFill();
    else if (choice == "4")  TestComparisonOperators();
    else if (choice == "5")  TestWithUserDefinedClass();
    else if (choice == "6")  TestWithUserDefinedStruct();
    else if (choice == "7")  TestCopyAndMoveSemantics();
    else if (choice == "8")  TestConstCorrectness();
    else if (choice == "9")  TestViolationHandling();
    else if (choice == "10") TestZeroSizedArray();
    else if (choice == "11") TestReverseIterators();
    else if (choice == "12") TestPartialInitialization();
    else if (choice == "13") TestNegativeScenarios();
    else if (choice == "14") TestTwoDimensionalArrays();
    else {
        std::cout << "Invalid test number: " << choice << "\n";
        PrintUsage(argv[0]);
        return 1;
    }

    return 0;
}

/**********************************************************************************************************************
 *  TEST DEFINITIONS
 *********************************************************************************************************************/

/*!
 * \brief Test #1: Element Access and Iterators (both forward and range-based)
 */
void TestElementAccessAndIterators()
{
    std::cout << "\n=== Test 1: Element Access and Iterators ===\n";
    ara::core::Array<int,5> arr = {10, 20, 30, 40, 50};

    // at() => checked access
    std::cout << "arr.at(2) = " << arr.at(2) << " (expected 30)\n";
    assert(arr.at(2) == 30);

    // operator[] => unchecked
    std::cout << "arr[0] = " << arr[0] << " (expected 10)\n";
    assert(arr[0] == 10);

    // Forward iteration using iterators
    std::cout << "Forward iteration: ";
    int sum = 0;
    for (auto it = arr.begin(); it != arr.end(); ++it) {
        std::cout << *it << " ";
        sum += *it;
    }
    std::cout << "\nSum of elements = " << sum << " (expected 150)\n";
    assert(sum == 150);
}

/*!
 * \brief Test #2: get<I>() functionality
 */
void TestGetFunction()
{
    std::cout << "\n=== Test 2: get<I>() Functionality ===\n";
    ara::core::Array<std::string,3> strArr = {"Alpha", "Beta", "Gamma"};

    std::cout << "get<0>(strArr) => " << ara::core::get<0>(strArr) << "\n";
    assert(ara::core::get<0>(strArr) == "Alpha");

    std::cout << "get<2>(strArr) => " << ara::core::get<2>(strArr) << "\n";
    assert(ara::core::get<2>(strArr) == "Gamma");
}

/*!
 * \brief Test #3: Swap and Fill
 */
void TestSwapAndFill()
{
    std::cout << "\n=== Test 3: Swap and Fill ===\n";
    ara::core::Array<int,4> arr1 = {1,2,3,4};
    ara::core::Array<int,4> arr2 = {5,6,7,8};

    std::cout << "arr1 before swap: ";
    for (auto i : arr1) std::cout << i << " ";
    std::cout << "\narr2 before swap: ";
    for (auto i : arr2) std::cout << i << " ";

    // do swap
    swap(arr1, arr2);

    std::cout << "\narr1 after swap: ";
    for (auto i : arr1) std::cout << i << " ";
    std::cout << "\narr2 after swap: ";
    for (auto i : arr2) std::cout << i << " ";

    // fill arr1 with 100
    arr1.fill(100);
    std::cout << "\narr1 after fill(100): ";
    for (auto i : arr1) {
        std::cout << i << " ";
        assert(i == 100);
    }
    std::cout << "\n";
}

/*!
 * \brief Test #4: Comparison Operators (==, !=, <, <=, >, >=)
 */
void TestComparisonOperators()
{
    std::cout << "\n=== Test 4: Comparison Operators ===\n";
    ara::core::Array<int,3> arrayA = {1,2,3};
    ara::core::Array<int,3> arrayB = {1,2,3};
    ara::core::Array<int,3> arrayC = {1,2,4};

    // Checking equality
    std::cout << "arrayA == arrayB => " << (arrayA == arrayB) << " (expected true)\n";
    assert(arrayA == arrayB);

    std::cout << "arrayA != arrayC => " << (arrayA != arrayC) << " (expected true)\n";
    assert(arrayA != arrayC);

    // Checking < and <=
    std::cout << "arrayA < arrayC  => " << (arrayA < arrayC) << " (expected true)\n";
    assert(arrayA < arrayC);

    std::cout << "arrayA <= arrayB => " << (arrayA <= arrayB) << " (expected true)\n";
    assert(arrayA <= arrayB);

    // Checking > and >=
    std::cout << "arrayC > arrayA  => " << (arrayC > arrayA) << " (expected true)\n";
    assert(arrayC > arrayA);

    std::cout << "arrayC >= arrayA => " << (arrayC >= arrayA) << " (expected true)\n";
    assert(arrayC >= arrayA);
}

/*!
 * \brief Test #5: Usage with a user-defined class
 */
void TestWithUserDefinedClass()
{
    std::cout << "\n=== Test 5: User-Defined Class ===\n";
    ara::core::Array<TestClass,3> classArr = { TestClass(10), TestClass(20), TestClass(30) };

    // Check middle element
    auto middleVal = classArr.at(1).GetValue();
    std::cout << "Middle element's value => " << middleVal << " (expected 20)\n";
    assert(middleVal == 20);

    // Check first element
    assert(classArr[0].GetValue() == 10);

    // Summation
    int sum = 0;
    for (auto& obj : classArr) {
        sum += obj.GetValue();
    }
    std::cout << "Sum of all values => " << sum << " (expected 60)\n";
    assert(sum == 60);
}

/*!
 * \brief Test #6: Usage with a user-defined struct
 */
void TestWithUserDefinedStruct()
{
    std::cout << "\n=== Test 6: User-Defined Struct ===\n";
    ara::core::Array<TestStruct,3> structArr = {
        TestStruct{1, "Alice"},
        TestStruct{2, "Bob"},
        TestStruct{3, "Charlie"}
    };

    // Verify second element
    TestStruct& secondRef = structArr.at(1);
    std::cout << "structArr[1] => ID=" << secondRef.id << ", name=" << secondRef.name << "\n";
    assert(secondRef.id == 2 && secondRef.name == "Bob");

    // Print all
    for (size_t i = 0; i < structArr.size(); ++i) {
        std::cout << "structArr[" << i << "] => (ID=" 
                  << structArr[i].id << ", Name=" << structArr[i].name << ")\n";
    }
}

/*!
 * \brief Test #7: Copy and Move Semantics
 */
void TestCopyAndMoveSemantics()
{
    std::cout << "\n=== Test 7: Copy and Move Semantics ===\n";
    ara::core::Array<TestClass, 2> original = { TestClass(100), TestClass(200) };

    // Copy constructor
    std::cout << "[Copy Constructor]\n";
    ara::core::Array<TestClass, 2> copied = original;
    // *** Use 'copied' to avoid warnings ***
    std::cout << "copied[0].GetValue() => " << copied[0].GetValue()
              << " (expected 100)\n";
    std::cout << "copied[1].GetValue() => " << copied[1].GetValue()
              << " (expected 200)\n";
    assert(copied[0].GetValue() == 100);
    assert(copied[1].GetValue() == 200);

    // Move constructor
    std::cout << "[Move Constructor]\n";
    ara::core::Array<TestClass, 2> moved = std::move(original);
    std::cout << "moved[0].GetValue() => " << moved[0].GetValue()
              << " (expected 100)\n";
    std::cout << "moved[1].GetValue() => " << moved[1].GetValue()
              << " (expected 200)\n";
    assert(moved[0].GetValue() == 100);
    assert(moved[1].GetValue() == 200);

    // original might now be in a “moved-from” state; 
    // optionally, check or print original’s contents if you wish.

    // Copy assignment
    std::cout << "[Copy Assignment]\n";
    ara::core::Array<TestClass, 2> copyAssigned;
    copyAssigned = moved;
    std::cout << "copyAssigned[0].GetValue() => " << copyAssigned[0].GetValue()
              << " (expected 100)\n";
    std::cout << "copyAssigned[1].GetValue() => " << copyAssigned[1].GetValue()
              << " (expected 200)\n";
    assert(copyAssigned[0].GetValue() == 100);
    assert(copyAssigned[1].GetValue() == 200);

    // Move assignment
    std::cout << "[Move Assignment]\n";
    ara::core::Array<TestClass, 2> moveAssigned;
    moveAssigned = std::move(copyAssigned);
    std::cout << "moveAssigned[0].GetValue() => " << moveAssigned[0].GetValue()
              << " (expected 100)\n";
    std::cout << "moveAssigned[1].GetValue() => " << moveAssigned[1].GetValue()
              << " (expected 200)\n";
    assert(moveAssigned[0].GetValue() == 100);
    assert(moveAssigned[1].GetValue() == 200);
}


/*!
 * \brief Test #8: Const Correctness
 */
void TestConstCorrectness()
{
    std::cout << "\n=== Test 8: Const Correctness ===\n";
    const ara::core::Array<int,3> constArr = {7, 8, 9};

    std::cout << "constArr.at(1) => " << constArr.at(1) << " (expected 8)\n";
    assert(constArr.at(1) == 8);

    int val2 = ara::core::get<2>(constArr);
    std::cout << "get<2>(constArr) => " << val2 << " (expected 9)\n";
    assert(val2 == 9);

    // iterate
    int sum = 0;
    for (auto it = constArr.begin(); it != constArr.end(); ++it) {
        sum += *it;
    }
    std::cout << "sum of constArr => " << sum << " (expected 24)\n";
    assert(sum == 24);

    // Attempting to modify => would be a compile error
    // constArr[0] = 999;
}

/*!
 * \brief Test #9: Violation Handling (Out-of-Range)
 */
void TestViolationHandling()
{
    std::cout << "\n=== Test 9: Violation Handling ===\n";
    ara::core::Array<int,3> arr = {10, 20, 30};

    // valid
    assert(arr.at(2) == 30);

    // This next call should trigger a violation (and terminate the process)
    std::cout << "Attempting arr.at(3) => out-of-range => violation.\n";
    arr.at(3); 
}

/*!
 * \brief Test #10: Zero-Sized Array
 */
void TestZeroSizedArray()
{
    std::cout << "\n=== Test 10: Zero-Sized Array ===\n";
    ara::core::Array<int,0> emptyArr;

    std::cout << "emptyArr.size() => " << emptyArr.size() << " (expected 0)\n";
    assert(emptyArr.size() == 0);

    std::cout << "emptyArr.empty() => " << emptyArr.empty() << " (expected 1/true)\n";
    assert(emptyArr.empty());

    assert(emptyArr.begin() == emptyArr.end());
    assert(emptyArr.data() == nullptr);

    // fill => no-op
    emptyArr.fill(42);
    assert(emptyArr.data() == nullptr);
    std::cout << "Called fill(42) on zero-sized => no effect.\n";
}

/*!
 * \brief Test #11: Reverse Iterators
 */
void TestReverseIterators()
{
    std::cout << "\n=== Test 11: Reverse Iterators ===\n";
    ara::core::Array<int,5> arr = {100, 200, 300, 400, 500};

    // forward iteration
    std::cout << "Forward: ";
    for (auto x : arr) {
        std::cout << x << " ";
    }
    std::cout << "\n";

    // reverse iteration
    std::cout << "Reverse: ";
    for (auto rit = arr.rbegin(); rit != arr.rend(); ++rit) {
        std::cout << *rit << " ";
    }
    std::cout << "\n";

    // const reverse iteration
    const auto& cRef = arr;
    std::cout << "Const Reverse: ";
    for (auto crit = cRef.crbegin(); crit != cRef.crend(); ++crit) {
        std::cout << *crit << " ";
    }
    std::cout << "\n";
}

/*!
 * \brief Test #12: Partial Initialization
 */
void TestPartialInitialization()
{
    std::cout << "\n=== Test 12: Partial Initialization ===\n";
    ara::core::Array<int,5> partialArr = {1, 2}; // rest default => 0,0,0

    for (size_t i = 0; i < partialArr.size(); ++i) {
        std::cout << "Index " << i << " => " << partialArr[i] << "\n";
    }
    // Checks
    assert(partialArr[0] == 1);
    assert(partialArr[1] == 2);
    assert(partialArr[2] == 0);
    assert(partialArr[3] == 0);
    assert(partialArr[4] == 0);
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

    // -------------------------------------------------------------------------
    // 26) Negative SWAP scenarios
    // -------------------------------------------------------------------------
    /*
    // (A) Attempting to swap Arrays of the same T but different sizes => compile-time error
    // Expected Outcome: static_assert or no matching call to swap(...).
    {
        std::cout << "[NEGATIVE] swap: Attempt to swap array of size 3 with array of size 4 => compile-time error.\n";
        
        ara::core::Array<int,3> arrSize3 = {1, 2, 3};
        ara::core::Array<int,4> arrSize4 = {4, 5, 6, 7};

        // The below line should fail to compile:
        swap(arrSize3, arrSize4); 
        // or arrSize3.swap(arrSize4);
    }
    */

    /*
    // (B) Attempting to swap Arrays with different T => compile-time error
    // Expected Outcome: static_assert or no matching call to swap(...).
    {
        std::cout << "[NEGATIVE] swap: Attempt to swap array<int,3> with array<double,3> => compile-time error.\n";

        ara::core::Array<int,3>  arrInt  = {1,2,3};
        ara::core::Array<double,3> arrDbl = {1.1,2.2,3.3};

        // The below line should fail to compile:
        swap(arrInt, arrDbl);
    }
    */

    // -------------------------------------------------------------------------
    // 27) Negative FILL scenarios
    // -------------------------------------------------------------------------
    /*
    // (A) Attempting to fill an Array of const int => compile-time error
    // Because fill(...) requires copy assignment, but const int is not assignable.
    {
        std::cout << "[NEGATIVE] fill: Attempting to fill an array of const int => compile-time error.\n";

        ara::core::Array<const int,3> constArr = {1, 2, 3};
        // The below line should fail to compile, as fill requires T& to be assignable:
        constArr.fill(42);
    }
    */


    /*
    // (B) Attempting to fill an Array whose T is not copy-assignable => compile-time error
    // For instance, we define a type with deleted operator=, then try to fill it.
    {
        std::cout << "[NEGATIVE] fill: Attempting to fill an array with a non-assignable type => compile-time error.\n";

        struct NoAssign {
            NoAssign() = default;
            NoAssign(const NoAssign&) = default;
            NoAssign& operator=(const NoAssign&) = delete; // deleted copy assignment
        };

        ara::core::Array<NoAssign,2> nonAssignableArr;
        // The below line should fail to compile, because fill(...) internally does operator=:
        nonAssignableArr.fill(NoAssign{});
    }
    */
    
    std::cout << "(All negative scenarios are currently commented out. "
                 "Uncomment each one individually to observe the intended compile-time or run-time failures.)\n"; 
}


/*!
 * \brief Test #14: Two-Dimensional Arrays
 */
void TestTwoDimensionalArrays()
{
    std::cout << "\n=== Test 14: Two-Dimensional Arrays ===\n";
    // We'll define a 2x3 matrix
    ara::core::Array<ara::core::Array<int,3>, 2> matrix = {
        ara::core::Array<int,3>{1,2,3},
        ara::core::Array<int,3>{4,5} // => partial => {4,5,0}
    };

    // Check row 0
    assert(matrix[0][0] == 1);
    assert(matrix[0][1] == 2);
    assert(matrix[0][2] == 3);

    // Check row 1
    assert(matrix[1][0] == 4);
    assert(matrix[1][1] == 5);
    assert(matrix[1][2] == 0);

    // fill first row with 99
    matrix[0].fill(99);
    assert(matrix[0][0] == 99 && matrix[0][1] == 99 && matrix[0][2] == 99);

    // swap row 0 and row 1
    swap(matrix[0], matrix[1]);
    // now row 0 => {4,5,0}, row 1 => {99,99,99}
    assert(matrix[0][0] == 4 && matrix[1][0] == 99);

    // Print final 2D array
    for (size_t r = 0; r < matrix.size(); ++r) {
        std::cout << "Row " << r << ": ";
        for (size_t c = 0; c < matrix[r].size(); ++c) {
            std::cout << matrix[r][c] << " ";
        }
        std::cout << "\n";
    }
}
