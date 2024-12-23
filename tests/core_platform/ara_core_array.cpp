// ara_core_array.cpp
#include "ara/core/array.h"
#include <iostream>
#include <string>
#include <cassert>

// User-defined class to test Array with complex types
class TestClass {
public:
    TestClass() : value_(0) {
        std::cout << "TestClass Default Constructor\n";
    }

    TestClass(int value) : value_(value) {
        std::cout << "TestClass Parameterized Constructor with value " << value_ << "\n";
    }

    TestClass(const TestClass& other) : value_(other.value_) {
        std::cout << "TestClass Copy Constructor\n";
    }

    TestClass(TestClass&& other) noexcept : value_(other.value_) {
        other.value_ = 0;
        std::cout << "TestClass Move Constructor\n";
    }

    TestClass& operator=(const TestClass& other) {
        if (this != &other) {
            value_ = other.value_;
            std::cout << "TestClass Copy Assignment\n";
        }
        return *this;
    }

    TestClass& operator=(TestClass&& other) noexcept {
        if (this != &other) {
            value_ = other.value_;
            other.value_ = 0;
            std::cout << "TestClass Move Assignment\n";
        }
        return *this;
    }

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

// User-defined struct to test Array with structs
struct TestStruct {
    int id;
    std::string name;

    bool operator==(const TestStruct& other) const {
        return id == other.id && name == other.name;
    }

    bool operator!=(const TestStruct& other) const {
        return !(*this == other);
    }

    bool operator<(const TestStruct& other) const {
        if (id != other.id)
            return id < other.id;
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

// Test Function Declarations
void TestElementAccess();
void TestGetFunction();
void TestSwapAndFill();
void TestComparisonOperators();
void TestWithUserDefinedClass();
void TestWithUserDefinedStruct();
void TestCopyAndMoveSemantics();
void TestConstCorrectness();
void TestViolationHandling();

// Helper Function to Display Test Options
void DisplayUsage(const char* programName) {
    std::cout << "Usage: " << programName << " [test_number]\n"
              << "Available Tests:\n"
              << "  1 - Test Element Access and Iterators\n"
              << "  2 - Test get<I>() Functionality\n"
              << "  3 - Test Swap and Fill Functions\n"
              << "  4 - Test Comparison Operators\n"
              << "  5 - Test with User-Defined Class\n"
              << "  6 - Test with User-Defined Struct\n"
              << "  7 - Test Copy and Move Semantics\n"
              << "  8 - Test Const Correctness\n"
              << "  9 - Test Violation Handling (Out-of-Range Access)\n"
              << "Example: " << programName << " 1\n";
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        DisplayUsage(argv[0]);
        return 1;
    }

    std::string testNumber = argv[1];

    if (testNumber == "1") {
        TestElementAccess();
    }
    else if (testNumber == "2") {
        TestGetFunction();
    }
    else if (testNumber == "3") {
        TestSwapAndFill();
    }
    else if (testNumber == "4") {
        TestComparisonOperators();
    }
    else if (testNumber == "5") {
        TestWithUserDefinedClass();
    }
    else if (testNumber == "6") {
        TestWithUserDefinedStruct();
    }
    else if (testNumber == "7") {
        TestCopyAndMoveSemantics();
    }
    else if (testNumber == "8") {
        TestConstCorrectness();
    }
    else if (testNumber == "9") {
        TestViolationHandling();
    }
    else {
        std::cout << "Invalid test number.\n";
        DisplayUsage(argv[0]);
        return 1;
    }

    return 0;
}

// Test Function Definitions

// 1. Test Element Access and Iterators
void TestElementAccess() {
    std::cout << "\n=== Test 1: Element Access and Iterators ===\n";
    ara::core::Array<int, 5> intArray = {10, 20, 30, 40, 50};

    // Access using at()
    std::cout << "Accessing at(2): " << intArray.at(2) << " (Expected: 30)\n";

    // Access using operator[]
    std::cout << "Accessing [0]: " << intArray[0] << " (Expected: 10)\n";

    // Iterate using iterators
    std::cout << "Iterating using iterators: ";
    for(auto it = intArray.begin(); it != intArray.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop: ";
    for(const auto& elem : intArray) {
        std::cout << elem << " ";
    }
    std::cout << "\n";
}

// 2. Test get<I>() Functionality
void TestGetFunction() {
    std::cout << "\n=== Test 2: get<I>() Functionality ===\n";
    ara::core::Array<std::string, 3> stringArray = {"Alpha", "Beta", "Gamma"};

    // Access using get<I>()
    std::cout << "get<0>(stringArray): " << ara::core::get<0>(stringArray) << " (Expected: Alpha)\n";
    std::cout << "get<2>(stringArray): " << ara::core::get<2>(stringArray) << " (Expected: Gamma)\n";

    // Uncommenting the following line will cause a compile-time error
    // std::cout << "get<3>(stringArray): " << ara::core::get<3>(stringArray) << "\n";
}

// 3. Test Swap and Fill Functions
void TestSwapAndFill() {
    std::cout << "\n=== Test 3: Swap and Fill Functions ===\n";
    ara::core::Array<int, 4> array1 = {1, 2, 3, 4};
    ara::core::Array<int, 4> array2 = {5, 6, 7, 8};

    std::cout << "Before swap:\n";
    std::cout << "array1: ";
    for(const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\narray2: ";
    for(const auto& elem : array2) std::cout << elem << " ";
    std::cout << "\n";

    swap(array1, array2);

    std::cout << "After swap:\n";
    std::cout << "array1: ";
    for(const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\narray2: ";
    for(const auto& elem : array2) std::cout << elem << " ";
    std::cout << "\n";

    // Test fill()
    array1.fill(100);
    std::cout << "After fill(100) on array1: ";
    for(const auto& elem : array1) std::cout << elem << " ";
    std::cout << "\n";
}

// 4. Test Comparison Operators
void TestComparisonOperators() {
    std::cout << "\n=== Test 4: Comparison Operators ===\n";
    ara::core::Array<int, 3> arrayA = {1, 2, 3};
    ara::core::Array<int, 3> arrayB = {1, 2, 3};
    ara::core::Array<int, 3> arrayC = {1, 2, 4};

    // Test ==
    std::cout << "arrayA == arrayB: " << (arrayA == arrayB) << " (Expected: 1)\n";
    std::cout << "arrayA == arrayC: " << (arrayA == arrayC) << " (Expected: 0)\n";

    // Test !=
    std::cout << "arrayA != arrayB: " << (arrayA != arrayB) << " (Expected: 0)\n";
    std::cout << "arrayA != arrayC: " << (arrayA != arrayC) << " (Expected: 1)\n";

    // Test <
    std::cout << "arrayA < arrayC: " << (arrayA < arrayC) << " (Expected: 1)\n";
    std::cout << "arrayC < arrayA: " << (arrayC < arrayA) << " (Expected: 0)\n";

    // Test <=
    std::cout << "arrayA <= arrayB: " << (arrayA <= arrayB) << " (Expected: 1)\n";
    std::cout << "arrayA <= arrayC: " << (arrayA <= arrayC) << " (Expected: 1)\n";
    std::cout << "arrayC <= arrayA: " << (arrayC <= arrayA) << " (Expected: 0)\n";

    // Test >
    std::cout << "arrayA > arrayC: " << (arrayA > arrayC) << " (Expected: 0)\n";
    std::cout << "arrayC > arrayA: " << (arrayC > arrayA) << " (Expected: 1)\n";

    // Test >=
    std::cout << "arrayA >= arrayB: " << (arrayA >= arrayB) << " (Expected: 1)\n";
    std::cout << "arrayC >= arrayA: " << (arrayC >= arrayA) << " (Expected: 1)\n";
    std::cout << "arrayA >= arrayC: " << (arrayA >= arrayC) << " (Expected: 0)\n";
}

// 5. Test with User-Defined Class
void TestWithUserDefinedClass() {
    std::cout << "\n=== Test 5: Testing with User-Defined Class ===\n";
    ara::core::Array<TestClass, 3> classArray = {TestClass(10), TestClass(20), TestClass(30)};

    // Access using at()
    std::cout << "Accessing at(1): " << classArray.at(1).GetValue() << " (Expected: 20)\n";

    // Access using operator[]
    std::cout << "Accessing [0]: " << classArray[0].GetValue() << " (Expected: 10)\n";

    // Iterate using iterators
    std::cout << "Iterating using iterators: ";
    for(auto it = classArray.begin(); it != classArray.end(); ++it) {
        std::cout << it->GetValue() << " ";
    }
    std::cout << "\n";

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop: ";
    for(const auto& elem : classArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";
}

// 6. Test with User-Defined Struct
void TestWithUserDefinedStruct() {
    std::cout << "\n=== Test 6: Testing with User-Defined Struct ===\n";
    ara::core::Array<TestStruct, 3> structArray = {TestStruct{1, "Alice"}, TestStruct{2, "Bob"}, TestStruct{3, "Charlie"}};

    // Access using at()
    std::cout << "Accessing at(1): ID=" << structArray.at(1).id << ", Name=" << structArray.at(1).name << " (Expected: ID=2, Name=Bob)\n";

    // Access using operator[]
    std::cout << "Accessing [0]: ID=" << structArray[0].id << ", Name=" << structArray[0].name << " (Expected: ID=1, Name=Alice)\n";

    // Iterate using iterators
    std::cout << "Iterating using iterators:\n";
    for(auto it = structArray.begin(); it != structArray.end(); ++it) {
        std::cout << "ID=" << it->id << ", Name=" << it->name << "\n";
    }

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop:\n";
    for(const auto& elem : structArray) {
        std::cout << "ID=" << elem.id << ", Name=" << elem.name << "\n";
    }
}

// 7. Test Copy and Move Semantics
void TestCopyAndMoveSemantics() {
    std::cout << "\n=== Test 7: Copy and Move Semantics ===\n";
    ara::core::Array<TestClass, 2> originalArray = {TestClass(100), TestClass(200)};

    // Test Copy Constructor
    std::cout << "\n-- Testing Copy Constructor --\n";
    ara::core::Array<TestClass, 2> copiedArray = originalArray;

    // Test Move Constructor
    std::cout << "\n-- Testing Move Constructor --\n";
    ara::core::Array<TestClass, 2> movedArray = std::move(originalArray);

    // Verify moved array contents
    std::cout << "\nMoved Array Contents:\n";
    for(const auto& elem : movedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";

    // Verify original array after move (should be default-initialized)
    std::cout << "Original Array Contents after Move (Should be default-initialized):\n";
    for(const auto& elem : originalArray) {
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
    for(const auto& elem : moveAssignedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";

    // Verify copied array after move assignment (should be default-initialized)
    std::cout << "Copied Array Contents after Move Assignment (Should be default-initialized):\n";
    for(const auto& elem : copiedArray) {
        std::cout << elem.GetValue() << " ";
    }
    std::cout << "\n";
}

// 8. Test Const Correctness
void TestConstCorrectness() {
    std::cout << "\n=== Test 8: Const Correctness ===\n";
    const ara::core::Array<int, 3> constArray = {7, 8, 9};

    // Access using at()
    std::cout << "Accessing at(1): " << constArray.at(1) << " (Expected: 8)\n";

    // Access using get<I>()
    std::cout << "Using get<2>: " << ara::core::get<2>(constArray) << " (Expected: 9)\n";

    // Iterate using iterators
    std::cout << "Iterating using const_iterators: ";
    for(auto it = constArray.begin(); it != constArray.end(); ++it) {
        std::cout << *it << " ";
    }
    std::cout << "\n";

    // Iterate using range-based for loop
    std::cout << "Iterating using range-based for loop: ";
    for(const auto& elem : constArray) {
        std::cout << elem << " ";
    }
    std::cout << "\n";

    // Attempting to modify elements should result in compilation errors
    // Uncommenting the following lines will cause compilation errors
    /*
    ara::core::get<0>(constArray) = 100;
    constArray.at(1) = 200;
    */
}

// 9. Test Violation Handling (Out-of-Range Access)
void TestViolationHandling() {
    std::cout << "\n=== Test 9: Violation Handling (Out-of-Range Access) ===\n";
    ara::core::Array<int, 3> violationArray = {10, 20, 30};

    // Valid access
    std::cout << "Accessing at(2): " << violationArray.at(2) << " (Expected: 30)\n";

    // Out-of-range access to trigger violation
    std::cout << "Accessing at(3) (Out-of-Range): ";
    violationArray.at(3); // This should trigger violation and terminate the program
}
