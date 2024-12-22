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
 *              to meet Adaptive AUTOSAR requirements.
 *********************************************************************************************************************/

#ifndef OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_
#define OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <array>
#include <utility>
#include <cassert>
#include <cstddef>
#include <algorithm> // For std::copy

namespace ara {
namespace core {

/*!
 * \brief  Fixed-size array template for ara::core.
 * \tparam T  The type of elements stored in the array.
 * \tparam N  The number of elements in the array.
 *
 * \details
 * The ara::core::Array class template provides a fixed-size array container that wraps around std::array,
 * adding additional safety and functionality tailored for the OpenAA project. It ensures deterministic behavior
 * suitable for automotive applications by enforcing bounds checking and providing optimized swap operations.
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
        std::copy(init_values.begin(), init_values.end(), data_.begin());
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01265] & [SWS_CORE_01266] Definition of API function ara::core::Array::operator[]
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
     * with a standardized log message: "Array access out of range: Tried to access >n< in array of size >N<"
     *
     * \pre    index < N
     */
    reference at(size_type index) {
        if (index >= N) {
            // [SWS_CORE_00040] Handling violations instead of throwing exceptions
            // Replace the following line with the actual Violation handling mechanism as per AUTOSAR.
            // For demonstration, we'll use assert with a descriptive message.
            assert(false && "Array access out of range: Tried to access >n< in array of size >N<");
            // In a real AUTOSAR implementation, this should trigger a Violation as specified.
        }
        return data_.at(index);
    }

    /*!
     * \brief  Access element with bounds checking (const version).
     * \param  index  The index of the element to access.
     * \return Const reference to the element at the specified index.
     *
     * \details
     * Provides safe access to array elements. If the index is out of bounds, a Violation is triggered
     * with a standardized log message: "Array access out of range: Tried to access >n< in array of size >N<"
     *
     * \pre    index < N
     */
    constexpr const_reference at(size_type index) const {
        if (index >= N) {
            // [SWS_CORE_00040] Handling violations instead of throwing exceptions
            // Replace the following line with the actual Violation handling mechanism as per AUTOSAR.
            // For demonstration, we'll use assert with a descriptive message.
            assert(false && "Array access out of range: Tried to access >n< in array of size >N<");
            // In a real AUTOSAR implementation, this should trigger a Violation as specified.
        }
        return data_.at(index);
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
    // [SWS_CORE_01267] & [SWS_CORE_01268] Definition of API function ara::core::Array::front
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a reference to the first element of this Array.
     * \return Reference to the first element.
     *
     * \details
     * The behavior is undefined if the Array is empty.
     *
     * \pre    The Array must not be empty.
     */
    reference front() {
        assert(N > 0 && "Calling front() on an empty Array is undefined.");
        return data_.front();
    }

    /*!
     * \brief  Returns a const reference to the first element of this Array.
     * \return Const reference to the first element.
     *
     * \details
     * The behavior is undefined if the Array is empty.
     *
     * \pre    The Array must not be empty.
     */
    constexpr const_reference front() const {
        assert(N > 0 && "Calling front() on an empty Array is undefined.");
        return data_.front();
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01269] & [SWS_CORE_01270] Definition of API function ara::core::Array::back
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a reference to the last element of this Array.
     * \return Reference to the last element.
     *
     * \details
     * The behavior is undefined if the Array is empty.
     *
     * \pre    The Array must not be empty.
     */
    reference back() {
        assert(N > 0 && "Calling back() on an empty Array is undefined.");
        return data_.back();
    }

    /*!
     * \brief  Returns a const reference to the last element of this Array.
     * \return Const reference to the last element.
     *
     * \details
     * The behavior is undefined if the Array is empty.
     *
     * \pre    The Array must not be empty.
     */
    constexpr const_reference back() const {
        assert(N > 0 && "Calling back() on an empty Array is undefined.");
        return data_.back();
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
        return data_.data();
    }

    /*!
     * \brief  Returns a const pointer to the first element of this Array.
     * \return Const pointer to the first element.
     *
     * \pre    None.
     */
    const_pointer data() const noexcept {
        return data_.data();
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
     * It does not throw exceptions and is not exception safe.
     *
     * \pre    None.
     */
    void fill(const T& u) {
        data_.fill(u);
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
    void swap(Array& other) noexcept(noexcept(std::swap(data_, other.data_))) {
        data_.swap(other.data_);
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
        return data_.begin();
    }

    /*!
     * \brief  Returns a const_iterator to the first element of this Array.
     * \return Const_iterator pointing to the first element.
     *
     * \pre    None.
     */
    const_iterator begin() const noexcept {
        return data_.begin();
    }

    /*!
     * \brief  Returns an iterator pointing past the last element of this Array.
     * \return Iterator pointing past the last element.
     *
     * \pre    None.
     */
    iterator end() noexcept {
        return data_.end();
    }

    /*!
     * \brief  Returns a const_iterator pointing past the last element of this Array.
     * \return Const_iterator pointing past the last element.
     *
     * \pre    None.
     */
    const_iterator end() const noexcept {
        return data_.end();
    }

    /*!
     * \brief  Returns a reverse_iterator pointing to the last element of this Array.
     * \return Reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    reverse_iterator rbegin() noexcept {
        return data_.rbegin();
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    const_reverse_iterator rbegin() const noexcept {
        return data_.rbegin();
    }

    /*!
     * \brief  Returns a reverse_iterator pointing past the first element of this Array.
     * \return Reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    reverse_iterator rend() noexcept {
        return data_.rend();
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    const_reverse_iterator rend() const noexcept {
        return data_.rend();
    }

    /*!
     * \brief  Returns a const_iterator to the first element of this Array.
     * \return Const_iterator pointing to the first element.
     *
     * \pre    None.
     */
    const_iterator cbegin() const noexcept {
        return data_.cbegin();
    }

    /*!
     * \brief  Returns a const_iterator pointing past the last element of this Array.
     * \return Const_iterator pointing past the last element.
     *
     * \pre    None.
     */
    const_iterator cend() const noexcept {
        return data_.cend();
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing to the last element of this Array.
     * \return Const_reverse_iterator pointing to the last element.
     *
     * \pre    None.
     */
    const_reverse_iterator crbegin() const noexcept {
        return data_.crbegin();
    }

    /*!
     * \brief  Returns a const_reverse_iterator pointing past the first element of this Array.
     * \return Const_reverse_iterator pointing past the first element.
     *
     * \pre    None.
     */
    const_reverse_iterator crend() const noexcept {
        return data_.crend();
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01290] - [SWS_CORE_01295] Definition of API comparison operators
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns true if the two Arrays have equal content.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if Arrays are equal, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator==(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return lhs.GetStdArray() == rhs.GetStdArray();
    }

    /*!
     * \brief  Returns true if the two Arrays have non-equal content.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if Arrays are non-equal, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return !(lhs == rhs);
    }

    /*!
     * \brief  Returns true if the contents of lhs are lexicographically less than the contents of rhs.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if lhs < rhs, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator<(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return lhs.GetStdArray() < rhs.GetStdArray();
    }

    /*!
     * \brief  Returns true if the contents of rhs are lexicographically less than the contents of lhs.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if lhs > rhs, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator>(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return rhs < lhs;
    }

    /*!
     * \brief  Returns true if the contents of lhs are lexicographically less than or equal to the contents of rhs.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if lhs <= rhs, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return !(rhs < lhs);
    }

    /*!
     * \brief  Returns true if the contents of rhs are lexicographically less than or equal to the contents of lhs.
     * \param  lhs  The left-hand side of the comparison.
     * \param  rhs  The right-hand side of the comparison.
     * \return True if lhs >= rhs, false otherwise.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    bool operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs) noexcept {
        return !(lhs < rhs);
    }

    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01296] Definition of API function ara::core::swap
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Overload of std::swap for ara::core::Array.
     * \param  lhs  The first Array to swap.
     * \param  rhs  The second Array to swap.
     *
     * \details
     * Exchanges the contents of two ara::core::Array instances efficiently.
     *
     * \pre    None.
     */
    template <typename T, std::size_t N>
    void swap(Array<T, N>& lhs, Array<T, N>& rhs) noexcept(noexcept(lhs.swap(rhs))) {
        lhs.swap(rhs);
    }

private:
    // -----------------------------------------------------------------------------------
    // [SWS_CORE_01201] Definition of API class ara::core::Array
    // -----------------------------------------------------------------------------------
    std::array<T, N> data_;  /*!< Underlying fixed-size array storage */
};

}  // namespace core
}  // namespace ara

#endif  // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_