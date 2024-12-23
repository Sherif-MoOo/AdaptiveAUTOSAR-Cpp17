/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/core/array.h
 *  \brief      Definition and implementation of the ara::core::Array template class.
 *
 *  \details    This file defines and implements the ara::core::Array template class, a fixed-size array container
 *              designed for the OpenAA project. It provides functionalities similar to std::array with additional customization
 *              to meet Adaptive AUTOSAR requirements, including violation handling and optimized memory allocation.
 *********************************************************************************************************************/

#ifndef OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_
#define OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>        // For std::size_t, std::ptrdiff_t
#include <iterator>       // For std::reverse_iterator
#include <algorithm>      // For std::copy, std::swap, std::lexicographical_compare
#include <type_traits>    // For std::is_nothrow_copy_assignable
#include <utility>        // For std::declval, std::move

// Placeholder for AUTOSAR's Diagnostic Log and Trace (DLT) system
// Replace with actual DLT integration as per AUTOSAR specifications

#include <iostream>       // For demonstration purposes (std::cerr)

/**********************************************************************************************************************
 *  NAMESPACE ara::core
 *********************************************************************************************************************/
namespace ara {
namespace core {

/**********************************************************************************************************************
 *  Violation Handling
 *********************************************************************************************************************/
namespace detail {

/*!
 * \brief  Aborts the process execution.
 *
 * \details
 * This function should be implemented to terminate the process in a manner consistent with the AUTOSAR specifications.
 * Replace this with the actual implementation as per your system's requirements.
 */
inline void Abort() noexcept {
    std::cerr << "Process aborted due to a critical violation." << std::endl;
    std::terminate();
}

/*!
 * \brief  Triggers a violation with a specified message and aborts the process.
 * \param  processIdentifier  Identifier of the process causing the violation.
 * \param  location           Location where the violation was detected (e.g., filename:line).
 * \param  indexValue         The index value that was accessed.
 * \param  arraySize          The size of the array.
 *
 * \details
 * This function logs the violation message as per [SWS_CORE_13017] and then aborts the process.
 */
inline void TriggerViolation(const char* processIdentifier, const char* location, std::size_t indexValue, std::size_t arraySize) noexcept {
    // Construct the violation message as per [SWS_CORE_13017]
    std::cerr << "Violation detected in " << processIdentifier << " at " << location
              << ": Array access out of range: Tried to access " << indexValue
              << " in array of size " << arraySize << "." << std::endl;
    // Abort the process as per [SWS_CORE_00090]
    Abort();
}

} // namespace detail

/**********************************************************************************************************************
 *  Internal Macros (Not Exposed to Users)
 *********************************************************************************************************************/

/*!
 * \brief  Helper macros for stringification.
 *
 * \details
 * These macros are used internally to convert macro arguments to string literals.
 */
#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)

/*!
 * \brief  Internal macro to trigger a violation with automatic location information.
 * \param  processId  Identifier of the process causing the violation.
 * \param  index      The index value that was accessed.
 * \param  size       The size of the array.
 *
 * \details
 * This macro captures the file name and line number where the violation is triggered.
 * It is intended for internal use only and should not be exposed to users.
 */
#define ARA_INTERNAL_TRIGGER_VIOLATION(processId, index, size) \
    ara::core::detail::TriggerViolation(processId, __FILE__ ":" TOSTRING(__LINE__), index, size)

/**********************************************************************************************************************
 *  Class: Array
 *********************************************************************************************************************/

/*!
 * \brief  Fixed-size array template for ara::core.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 *
 * \details
 * The ara::core::Array class template provides a fixed-size array container that encapsulates fixed-size arrays.
 * It mirrors the behavior of std::array with modifications to meet AUTOSAR requirements, such as:
 * - Using Violations instead of exceptions for error handling in certain functions.
 * - Providing specialized overloads for std::get within the ara::core namespace.
 * 
 * \pre    N must be greater than 0.
 * \note   This class does not support dynamic memory allocation.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
class Array final {
public:
    // -----------------------------------------------------------------------------------
    // Type Aliases
    // -----------------------------------------------------------------------------------
    using reference = T&;                                    /*!< [SWS_CORE_01210] Definition of API type ara::core::Array::reference */
    using const_reference = const T&;                        /*!< [SWS_CORE_01211] Definition of API type ara::core::Array::const_reference */
    using iterator = T*;                                     /*!< [SWS_CORE_01212] Definition of API type ara::core::Array::iterator */
    using const_iterator = const T*;                         /*!< [SWS_CORE_01213] Definition of API type ara::core::Array::const_iterator */
    using size_type = std::size_t;                           /*!< [SWS_CORE_01214] Definition of API type ara::core::Array::size_type */
    using difference_type = std::ptrdiff_t;                   /*!< [SWS_CORE_01215] Definition of API type ara::core::Array::difference_type */
    using value_type = T;                                    /*!< [SWS_CORE_01216] Definition of API type ara::core::Array::value_type */
    using pointer = T*;                                      /*!< [SWS_CORE_01217] Definition of API type ara::core::Array::pointer */
    using const_pointer = const T*;                          /*!< [SWS_CORE_01218] Definition of API type ara::core::Array::const_pointer */
    using reverse_iterator = std::reverse_iterator<iterator>; /*!< [SWS_CORE_01219] Definition of API type ara::core::Array::reverse_iterator */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>; /*!< [SWS_CORE_01220] Definition of API type ara::core::Array::const_reverse_iterator */

    // -----------------------------------------------------------------------------------
    // Constructors 
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Default constructor.
     *
     * \details
     * Initializes the array with default-constructed elements.
     *
     * \pre    None.
     */
    Array() = default;

    /*!
     * \brief  Initializer list constructor.
     * \param  init_values  An initializer list to initialize the array elements.
     *
     * \details
     * Initializes the array with the provided values. If the number of initializer elements is less than N,
     * the remaining elements are default-initialized.
     *
     * \pre    init_values.size() <= N
     */
    Array(std::initializer_list<T> init_values) {
        static_assert(N > 0, "Array size must be greater than 0.");
        std::size_t i = 0;
        for(auto it = init_values.begin(); it != init_values.end() && i < N; ++it, ++i) {
            data_[i] = *it;
        }
        // Remaining elements are default-initialized (already handled by default initialization of data_)
    }

    // -----------------------------------------------------------------------------------
    // Element Access
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Access element without bounds checking.
     * \param  index  The index of the element to access.
     * \return Reference to the element at the specified index.
     *
     * \details
     * Provides unchecked access to array elements. Use with caution to avoid undefined behavior.
     *
     * \pre    None.
     */
    auto operator[](size_type index) noexcept -> reference {    // [SWS_CORE_01266] & [SWS_CORE_01265]
        return data_[index];
    }

    /*!
     * \brief  Access element without bounds checking (const version).
     * \param  index  The index of the element to access.
     * \return Const reference to the element at the specified index.
     *
     * \details
     * Provides unchecked access to array elements. Use with caution to avoid undefined behavior.
     *
     * \pre    None.
     */
    auto operator[](size_type index) const noexcept -> const_reference { // [SWS_CORE_01266] & [SWS_CORE_01265]
        return data_[index];
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01273] & [SWS_CORE_01274] Definition of API function ara::core::Array::at
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Access element with bounds checking.
     * \param  index  The index of the element to access.
     * \return Reference to the element at the specified index.
     *
     * \details
     * Provides safe access to array elements. If the index is out of bounds, a Violation is triggered
     * with a standardized log message: "Violation detected in {processIdentifier} at {location}: Array access out of
     * range: Tried to access {indexValue} in array of size {arraySize}".
     *
     * \pre    index < N
     */
    auto at(size_type index) noexcept -> reference {         // [SWS_CORE_01273]
        if(index >= N) {
            // Replace "ProcessID" with the actual process identifier in your application context
            ARA_INTERNAL_TRIGGER_VIOLATION("ProcessID", index, N);
            // Process is aborted, no return needed
        }
        return data_[index];
    }

    /*!
     * \brief  Access element with bounds checking (const version).
     * \param  index  The index of the element to access.
     * \return Const reference to the element at the specified index.
     *
     * \details
     * Provides safe access to array elements. If the index is out of bounds, a Violation is triggered
     * with a standardized log message: "Violation detected in {processIdentifier} at {location}: Array access out of
     * range: Tried to access {indexValue} in array of size {arraySize}".
     *
     * \pre    index < N
     */
    auto at(size_type index) const noexcept -> const_reference { // [SWS_CORE_01274]
        if(index >= N) {
            ARA_INTERNAL_TRIGGER_VIOLATION("ProcessID", index, N);
            // Process is aborted, no return needed
        }
        return data_[index];
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01267] & [SWS_CORE_01268] Definition of API function ara::core::Array::front
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a reference to the first element of this Array.
     * \return Reference to the first element.
     *
     * \details
     * The behavior is undefined if the Array is empty. This function triggers a compile-time error
     * if N == 0.
     *
     * \pre    The Array must not be empty.
     */
    auto front() noexcept -> reference {                     // [SWS_CORE_01267]
        static_assert(N > 0, "Calling front() on an empty ara::core::Array is a compile-time error.");
        return data_[0];
    }

    /*!
     * \brief  Returns a const reference to the first element of this Array.
     * \return Const reference to the first element.
     *
     * \details
     * The behavior is undefined if the Array is empty. This function triggers a compile-time error
     * if N == 0.
     *
     * \pre    The Array must not be empty.
     */
    auto front() const noexcept -> const_reference {         // [SWS_CORE_01268]
        static_assert(N > 0, "Calling front() on an empty ara::core::Array is a compile-time error.");
        return data_[0];
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01269] & [SWS_CORE_01270] Definition of API function ara::core::Array::back
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a reference to the last element of this Array.
     * \return Reference to the last element.
     *
     * \details
     * The behavior is undefined if the Array is empty. This function triggers a compile-time error
     * if N == 0.
     *
     * \pre    The Array must not be empty.
     */
    auto back() noexcept -> reference {                      // [SWS_CORE_01269]
        static_assert(N > 0, "Calling back() on an empty ara::core::Array is a compile-time error.");
        return data_[N - 1];
    }

    /*!
     * \brief  Returns a const reference to the last element of this Array.
     * \return Const reference to the last element.
     *
     * \details
     * The behavior is undefined if the Array is empty. This function triggers a compile-time error
     * if N == 0.
     *
     * \pre    The Array must not be empty.
     */
    auto back() const noexcept -> const_reference {          // [SWS_CORE_01270]
        static_assert(N > 0, "Calling back() on an empty ara::core::Array is a compile-time error.");
        return data_[N - 1];
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01271] & [SWS_CORE_01272] Definition of API function ara::core::Array::data
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a pointer to the first element of this Array.
     * \return Pointer to the first element.
     *
     * \pre    None.
     */
    auto data() noexcept -> pointer {                        // [SWS_CORE_01271]
        if constexpr (N == 0) {
            return nullptr;
        } else {
            return data_;
        }
    }

    /*!
     * \brief  Returns a const pointer to the first element of this Array.
     * \return Const pointer to the first element.
     *
     * \pre    None.
     */
    auto data() const noexcept -> const_pointer {            // [SWS_CORE_01272]
        if constexpr (N == 0) {
            return nullptr;
        } else {
            return data_;
        }
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01262] Definition of API function ara::core::Array::size
    // [SWS_CORE_01263] Definition of API function ara::core::Array::max_size
    // [SWS_CORE_01264] Definition of API function ara::core::Array::empty
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns the number of elements in the array.
     * \return The size of the array.
     *
     * \pre    None.
     */
    auto size() const noexcept -> size_type {                // [SWS_CORE_01262]
        return N;
    }

    /*!
     * \brief  Returns the maximum number of elements supported by this Array.
     * \return The maximum size of the array.
     *
     * \pre    None.
     */
    auto max_size() const noexcept -> size_type {            // [SWS_CORE_01263]
        return N;
    }

    /*!
     * \brief  Returns whether the Array is empty.
     * \return True if the Array contains 0 elements, false otherwise.
     *
     * \pre    None.
     */
    auto empty() const noexcept -> bool {                    // [SWS_CORE_01264]
        return N == 0;
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01250] - [SWS_CORE_01261] Definition of API functions ara::core::Array::begin/end/rbegin/rend/cbegin/cend/crbegin/crend
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns an iterator to the first element of this Array.
     * \return Iterator pointing to the first element.
     *
     * \pre    None.
     */
    auto begin() noexcept -> iterator {                      // [SWS_CORE_01250]
        return data_;
    }

    /*!
     * \brief  Returns a const_iterator to the first element of this Array.
     * \return Const_iterator pointing to the first element.
     *
     * \pre    None.
     */
    auto begin() const noexcept -> const_iterator {          // [SWS_CORE_01251]
        return data_;
    }

    /*!
     * \brief  Returns an iterator pointing past the last element of this Array.
     * \return Iterator pointing past the last element.
     *
     * \pre    None.
     */
    auto end() noexcept -> iterator {                        // [SWS_CORE_01252]
        return data_ + N;
    }

    /*!
     * \brief  Returns a const_iterator pointing past the last element of this Array.
     * \return Const_iterator pointing past the last element.
     *
     * \pre    None.
     */
    auto end() const noexcept -> const_iterator {            // [SWS_CORE_01253]
        return data_ + N;
    }

    /*!
     * \brief  Returns a reverse_iterator pointing to the last element of this Array.
     * \return Reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    auto rbegin() noexcept -> reverse_iterator {             // [SWS_CORE_01254]
        return reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    auto rbegin() const noexcept -> const_reverse_iterator { // [SWS_CORE_01255]
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    auto crbegin() const noexcept -> const_reverse_iterator { // [SWS_CORE_01260]
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a reverse_iterator pointing past the first element of this Array.
     * \return Reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    auto rend() noexcept -> reverse_iterator {               // [SWS_CORE_01256]
        return reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    auto rend() const noexcept -> const_reverse_iterator {   // [SWS_CORE_01257]
        return const_reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    auto crend() const noexcept -> const_reverse_iterator {   // [SWS_CORE_01261]
        return const_reverse_iterator(begin());
    }

    // -----------------------------------------------------------------------------------
    // Modifiers
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Assigns the given value to all elements of this Array.
     * \param  u  The value to assign.
     *
     * \details
     * This function assigns the value u to all elements of the Array.
     * It is conditionally exception safe based on the copy assignability of T.
     *
     * \pre    None.
     */
    auto fill(const T& u) noexcept(std::is_nothrow_copy_assignable<T>::value) -> void { // [SWS_CORE_01241]
        for(std::size_t i = 0; i < N; ++i) {
            data_[i] = u;
        }
    }

    /*!
     * \brief  Exchanges the contents with another Array.
     * \param  other  The other Array to swap with.
     *
     * \details
     * Exchanges the contents of this Array with another Array. This operation is conditionally noexcept
     * based on the noexcept status of the swap operation for the contained type.
     *
     * \pre    None.
     */
    auto swap(Array& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))) -> void { // [SWS_CORE_01242]
        for(std::size_t i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

private:
    // -----------------------------------------------------------------------------------
    // Data Members
    // -----------------------------------------------------------------------------------
    T data_[N]{};
};

/**********************************************************************************************************************
 *  Non-Member Functions: get
 *********************************************************************************************************************/

// -----------------------------------------------------------------------------------
// [SWS_CORE_01282] - [SWS_CORE_01284] Definition of API function ara::core::get
// -----------------------------------------------------------------------------------
/*!
 * \brief  Overload of std::get for an lvalue mutable ara::core::Array.
 * \tparam I  The index into the Array whose element is desired.
 * \tparam T  The type of element stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  a  The Array.
 * \return Reference to the Ith element of the Array.
 *
 * \details
 * Overload of std::get for an lvalue mutable ara::core::Array.
 * The implementation flags the condition I >= N as a compile-time error using static_assert.
 *
 * \pre    I < N
 *
 * \ingroup Containers
 */
template <std::size_t I, typename T, std::size_t N>
auto get(Array<T, N>& a) noexcept -> T& { // [SWS_CORE_01282]
    static_assert(I < N, "Index out of range in ara::core::get");
    return a[I];
}

/*!
 * \brief  Overload of std::get for an rvalue ara::core::Array.
 * \tparam I  The index into the Array whose element is desired.
 * \tparam T  The type of element stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  a  The Array.
 * \return Rvalue reference to the Ith element of the Array.
 *
 * \details
 * Overload of std::get for an rvalue ara::core::Array.
 * The implementation flags the condition I >= N as a compile-time error using static_assert.
 *
 * \pre    I < N
 *
 * \ingroup Containers
 */
template <std::size_t I, typename T, std::size_t N>
auto get(Array<T, N>&& a) noexcept -> T&& { // [SWS_CORE_01283]
    static_assert(I < N, "Index out of range in ara::core::get");
    return std::move(a[I]);
}

/*!
 * \brief  Overload of std::get for an lvalue const ara::core::Array.
 * \tparam I  The index into the Array whose element is desired.
 * \tparam T  The type of element stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  a  The Array.
 * \return Const reference to the Ith element of the Array.
 *
 * \details
 * Overload of std::get for an lvalue const ara::core::Array.
 * The implementation flags the condition I >= N as a compile-time error using static_assert.
 *
 * \pre    I < N
 *
 * \ingroup Containers
 */
template <std::size_t I, typename T, std::size_t N>
auto get(const Array<T, N>& a) noexcept -> const T& { // [SWS_CORE_01284]
    static_assert(I < N, "Index out of range in ara::core::get");
    return a[I];
}

/**********************************************************************************************************************
 *  Non-Member Functions: Comparison Operators
 *********************************************************************************************************************/

/*!
 * \brief  Returns true if the two Arrays have equal content.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if Arrays are equal, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() == std::declval<T&>())) -> bool { // [SWS_CORE_01290]
    for(std::size_t i = 0; i < N; ++i) {
        if(!(lhs[i] == rhs[i])) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief  Returns true if the two Arrays have non-equal content.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if Arrays are non-equal, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() != std::declval<T&>())) -> bool { // [SWS_CORE_01291]
    return !(lhs == rhs);
}

/*!
 * \brief  Returns true if the contents of lhs are lexicographically less than the contents of rhs.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if lhs < rhs, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() < std::declval<T&>())) -> bool { // [SWS_CORE_01292]
    return std::lexicographical_compare(lhs.begin(), lhs.end(), rhs.begin(), rhs.end());
}

/*!
 * \brief  Returns true if the contents of lhs are lexicographically less than or equal to the contents of rhs.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if lhs <= rhs, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() <= std::declval<T&>())) -> bool { // [SWS_CORE_01294]
    return !(rhs < lhs);
}

/*!
 * \brief  Returns true if the contents of lhs are lexicographically greater than the contents of rhs.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if lhs > rhs, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() > std::declval<T&>())) -> bool { // [SWS_CORE_01293]
    return rhs < lhs;
}

/*!
 * \brief  Returns true if the contents of lhs are lexicographically greater than or equal to the contents of rhs.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The left-hand side of the comparison.
 * \param  rhs  The right-hand side of the comparison.
 * \return True if lhs >= rhs, false otherwise.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() >= std::declval<T&>())) -> bool { // [SWS_CORE_01295]
    return !(lhs < rhs);
}

/**********************************************************************************************************************
 *  Non-Member Functions: swap
 *********************************************************************************************************************/

/*!
 * \brief  Overload of std::swap for ara::core::Array.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 * \param  lhs  The first Array to swap.
 * \param  rhs  The second Array to swap.
 *
 * \details
 * Exchanges the contents of two ara::core::Array instances efficiently.
 *
 * \pre    None.
 *
 * \ingroup Containers
 */
template <typename T, std::size_t N>
auto swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) -> void { // [SWS_CORE_01296]
    lhs.swap(rhs);
}

} // namespace core
} // namespace ara

/**********************************************************************************************************************
 *  END OF FILE: array.h
 *********************************************************************************************************************/
#endif // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_
