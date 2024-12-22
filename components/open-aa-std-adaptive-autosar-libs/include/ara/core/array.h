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
#include <cassert>        // For assert
#include <type_traits>    // For std::is_nothrow_copy_assignable
#include <utility>        // For std::declval

// Placeholder for AUTOSAR's Diagnostic Log and Trace (DLT) system
// Replace with actual DLT integration as per AUTOSAR specifications
#include <iostream>       // For demonstration purposes (std::cerr)

namespace ara {
namespace core {
namespace detail {

/*!
 * \brief  Triggers a violation with a specified message.
 * \param  processIdentifier  Identifier of the process causing the violation.
 * \param  location           Location where the violation was detected (e.g., filename:line).
 * \param  indexValue         The index value that was accessed.
 * \param  arraySize          The size of the array.
 *
 * \details
 * This function serves as a placeholder for AUTOSAR's Diagnostic Log and Trace (DLT) system.
 * Replace the implementation with the actual DLT logging mechanism as per AUTOSAR specifications.
 *
 * \pre    None.
 */
inline void TriggerViolation(const char* processIdentifier, const char* location, std::size_t indexValue, std::size_t arraySize) {
    // Construct the violation message as per [SWS_CORE_13017]
    std::cerr << "Violation detected in " << processIdentifier << " at " << location
              << ": Array access out of range: Tried to access " << indexValue
              << " in array of size " << arraySize << "." << std::endl;
    // In a real AUTOSAR environment, integrate with DLT and handle accordingly (e.g., log and halt)
}

} // namespace detail

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
    // [SWS_CORE_01210] Definition of API type ara::core::Array::reference
    // -----------------------------------------------------------------------------------
    using reference = T&;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01211] Definition of API type ara::core::Array::const_reference
    // -----------------------------------------------------------------------------------
    using const_reference = const T&;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01212] Definition of API type ara::core::Array::iterator
    // -----------------------------------------------------------------------------------
    using iterator = T*;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01213] Definition of API type ara::core::Array::const_iterator
    // -----------------------------------------------------------------------------------
    using const_iterator = const T*;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01214] Definition of API type ara::core::Array::size_type
    // -----------------------------------------------------------------------------------
    using size_type = std::size_t;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01215] Definition of API type ara::core::Array::difference_type
    // -----------------------------------------------------------------------------------
    using difference_type = std::ptrdiff_t;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01216] Definition of API type ara::core::Array::value_type
    // -----------------------------------------------------------------------------------
    using value_type = T;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01217] Definition of API type ara::core::Array::pointer
    // -----------------------------------------------------------------------------------
    using pointer = T*;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01218] Definition of API type ara::core::Array::const_pointer
    // -----------------------------------------------------------------------------------
    using const_pointer = const T*;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01219] Definition of API type ara::core::Array::reverse_iterator
    // -----------------------------------------------------------------------------------
    using reverse_iterator = std::reverse_iterator<iterator>;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01220] Definition of API type ara::core::Array::const_reverse_iterator
    // -----------------------------------------------------------------------------------
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01201] Definition of API class ara::core::Array
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
        assert(init_values.size() <= N && "Initializer list size exceeds Array capacity.");
        std::size_t i = 0;
        for(auto it = init_values.begin(); it != init_values.end(); ++it, ++i) {
            data_[i] = *it;
        }
        // Remaining elements are default-initialized (already handled by default initialization of data_)
    }

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
    reference operator[](size_type index) noexcept {
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
    constexpr const_reference operator[](size_type index) const noexcept {
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
    reference at(size_type index) noexcept {
        if(index >= N) {
            // Retrieve location information (e.g., __FILE__, __LINE__)
            // Since 'at' is a member function, we need to pass these from the call site.
            // For demonstration, using __FILE__ and __LINE__ directly.
            detail::TriggerViolation(__FILE__, std::to_string(__LINE__).c_str(), index, N);
            // Returning a reference to the first element to satisfy return type.
            // In practice, this should handle the violation as per AUTOSAR specifications.
            return data_[0];
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
    constexpr const_reference at(size_type index) const noexcept {
        if(index >= N) {
            // Retrieve location information (e.g., __FILE__, __LINE__)
            // Since 'at' is a member function, we need to pass these from the call site.
            // For demonstration, using __FILE__ and __LINE__ directly.
            detail::TriggerViolation(__FILE__, std::to_string(__LINE__).c_str(), index, N);
            // Returning a reference to the first element to satisfy return type.
            // In practice, this should handle the violation as per AUTOSAR specifications.
            return data_[0];
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
    reference front() noexcept {
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
    constexpr const_reference front() const noexcept {
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
    reference back() noexcept {
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
    constexpr const_reference back() const noexcept {
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
    pointer data() noexcept {
        return data_;
    }

    /*!
     * \brief  Returns a const pointer to the first element of this Array.
     * \return Const pointer to the first element.
     *
     * \pre    None.
     */
    const_pointer data() const noexcept {
        return data_;
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
    constexpr size_type size() const noexcept {
        return N;
    }

    /*!
     * \brief  Returns the maximum number of elements supported by this Array.
     * \return The maximum size of the array.
     *
     * \pre    None.
     */
    constexpr size_type max_size() const noexcept {
        return N;
    }

    /*!
     * \brief  Returns whether the Array is empty.
     * \return True if the Array contains 0 elements, false otherwise.
     *
     * \pre    None.
     */
    constexpr bool empty() const noexcept {
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
    iterator begin() noexcept {
        return data_;
    }

    /*!
     * \brief  Returns a const_iterator to the first element of this Array.
     * \return Const_iterator pointing to the first element.
     *
     * \pre    None.
     */
    const_iterator begin() const noexcept {
        return data_;
    }

    /*!
     * \brief  Returns an iterator pointing past the last element of this Array.
     * \return Iterator pointing past the last element.
     *
     * \pre    None.
     */
    iterator end() noexcept {
        return data_ + N;
    }

    /*!
     * \brief  Returns a const_iterator pointing past the last element of this Array.
     * \return Const_iterator pointing past the last element.
     *
     * \pre    None.
     */
    const_iterator end() const noexcept {
        return data_ + N;
    }

    /*!
     * \brief  Returns a reverse_iterator pointing to the last element of this Array.
     * \return Reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    reverse_iterator rbegin() noexcept {
        return reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    const_reverse_iterator rbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    const_reverse_iterator crbegin() const noexcept {
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a reverse_iterator pointing past the first element of this Array.
     * \return Reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    reverse_iterator rend() noexcept {
        return reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    const_reverse_iterator rend() const noexcept {
        return const_reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    const_reverse_iterator crend() const noexcept {
        return const_reverse_iterator(begin());
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01241] Definition of API function ara::core::Array::fill
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Assigns the given value to all elements of this Array.
     * \param  u  The value to assign.
     *
     * \details
     * This function assigns the value `u` to all elements of the Array.
     * It is conditionally exception safe based on the copy assignability of T.
     *
     * \pre    None.
     */
    void fill(const T& u) noexcept(std::is_nothrow_copy_assignable<T>::value) {
        for(std::size_t i = 0; i < N; ++i) {
            data_[i] = u;
        }
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01242] Definition of API function ara::core::Array::swap
    // -----------------------------------------------------------------------------------
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
    void swap(Array& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))) {
        for(std::size_t i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

private:
    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01201] Definition of API class ara::core::Array
    // -----------------------------------------------------------------------------------
    T data_[N]{};
};

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
constexpr T& get(Array<T, N>& a) noexcept {
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
constexpr T&& get(Array<T, N>&& a) noexcept {
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
constexpr const T& get(const Array<T, N>& a) noexcept {
    static_assert(I < N, "Index out of range in ara::core::get");
    return a[I];
}

// -----------------------------------------------------------------------------------
// [SWS_CORE_01290] - [SWS_CORE_01295] Definition of API comparison operators
// -----------------------------------------------------------------------------------
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
bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() == std::declval<T&>())) {
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
bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() != std::declval<T&>())) {
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
bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() < std::declval<T&>())) {
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
bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() <= std::declval<T&>())) {
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
bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() > std::declval<T&>())) {
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
bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept(noexcept(std::declval<T&>() >= std::declval<T&>())) {
    return !(lhs < rhs);
}

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
void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
    lhs.swap(rhs);
}

} // namespace core
} // namespace ara

#endif // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_