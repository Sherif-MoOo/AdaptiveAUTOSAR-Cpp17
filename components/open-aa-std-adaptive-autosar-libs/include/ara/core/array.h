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
 *              designed for the OpenAA project. It provides functionalities similar to std::array with additional
 *              customizations to meet Adaptive AUTOSAR requirements (e.g., [SWS_CORE_00040], [SWS_CORE_13017],
 *              [SWS_CORE_11200], etc.), including violation handling and optimized memory allocation.
 *
 *  \note       Based on the Adaptive AUTOSAR SWS (e.g., R24-11) requirements for the "Array" type, especially:
 *              - [SWS_CORE_01201] (Definition of ara::core::Array)
 *              - [SWS_CORE_01265], [SWS_CORE_01266] (operator[]),
 *              - [SWS_CORE_01273], [SWS_CORE_01274] (at()),
 *              - [SWS_CORE_01241] (fill()),
 *              - [SWS_CORE_00040] (No exceptions used – custom violation handling),
 *              - [SWS_CORE_13017] (Out-of-range message format),
 *              - [SWS_CORE_01290..01295] (comparison operators), etc.
 *********************************************************************************************************************/

#ifndef OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_
#define OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*!
 * \brief  Includes necessary standard headers for array operations, plus any additional
 *         headers for violation handling or logging (per AUTOSAR guidelines).
 *
 * [SWS_CORE_01214], [SWS_CORE_01215]: cstddef for std::size_t, std::ptrdiff_t
 * [SWS_CORE_01212], [SWS_CORE_01213]: iterator for std::reverse_iterator
 * [SWS_CORE_01241]: fill uses type traits for noexcept checks
 * [SWS_CORE_00040]: we do not throw exceptions – we do custom violation handling
 */
#include <cstddef>       // For std::size_t, std::ptrdiff_t
#include <iterator>      // For std::reverse_iterator
#include <algorithm>     // For std::lexicographical_compare, std::swap
#include <type_traits>   // For std::is_nothrow_copy_assignable, std::enable_if_t, std::conjunction, std::is_convertible, etc.
#include <utility>       // For std::declval, std::move, std::forward
#include <iostream>      // For demonstration logging (std::cerr)
#include <cstdlib>       // For std::terminate (to handle violations/fatal errors)

// Pull in location utilities for capturing file/line details:
#include "ara/core/internal/location_utils.h"

/**********************************************************************************************************************
 *  NAMESPACE: ara::core
 *********************************************************************************************************************/
/*!
 * \brief  The ara::core namespace, within which our AUTOSAR Adaptive Platform
 *         data types and utilities reside.
 */
namespace ara {
namespace core {

/**********************************************************************************************************************
 *  SECTION: Forward Declaration
 *********************************************************************************************************************/
/*!
 * \brief  Forward declaration of the Array class template.
 */
template <typename T, std::size_t N>
class Array;

/**********************************************************************************************************************
 *  SECTION: Violation Handling
 *********************************************************************************************************************/
/*!
 * \brief  Contains internal details for handling Violations according to AUTOSAR requirements.
 *
 * [SWS_CORE_00090], [SWS_CORE_00091] – Standardized Violations result in abnormal termination,
 * with a diagnostic message describing the process and location.
 *
 * [SWS_CORE_13017] – For Array, the out-of-range message format should mention:
 *   "Violation detected in {processIdentifier} at {location}: Array access out of range..."
 */
namespace detail {

/*!
 * \brief Aborts the process execution (non-recoverable violation).
 *
 * \details
 * Logs an error message to std::cerr for demonstration purposes, then calls std::terminate().
 * In a real implementation, this might integrate with the DLT error log (via \a ara::log or similar).
 */
inline auto Abort() noexcept -> void
{
    // Demonstration log:
    std::cerr << "\nFATAL: [Abort] Process aborted due to a critical violation in ara::core::Array.\n";
    std::terminate();
}

/*!
 * \brief Logs + terminates upon array-access-out-of-range for ara::core::Array.
 * \param processIdentifier A textual name for logging context, e.g., "Array".
 * \param location          The stripped file/line location (e.g., "file.cpp:123").
 * \param invalidIndex      The invalid index that was requested.
 * \param arraySize         The total number of elements in the array.
 *
 * \details
 * - In real Adaptive AUTOSAR usage, this might incorporate more advanced DLT formatting.
 * - This message format is guided by [SWS_CORE_13017].
 */
inline auto TriggerOutOfRangeViolation(const char* processIdentifier,
                                       const char* location,
                                       std::size_t invalidIndex,
                                       std::size_t arraySize) noexcept -> void
{
    // Minimal demonstration for out-of-range logging with location info:
    std::cerr << "\n[FATAL] Violation detected in " << processIdentifier
              << " at " << location
              << ": ara::core::Array access out of range.\n"
              << "      Tried to access index " << invalidIndex
              << " in array of size " << arraySize
              << ".\n";

    // Per [SWS_CORE_00090], forcibly terminate:
    Abort();
}

/*!
 * \brief Helper trait to determine if T[N] can be brace-initialized with Args... without narrowing conversions.
 *
 * \tparam T     The type of elements in the array.
 * \tparam N     The size of the array.
 * \tparam Args  The types of arguments provided for initialization.
 *
 * \details
 * - Utilizes SFINAE to check if brace-initialization of T[N] with Args... is possible.
 * - If brace-initialization would cause narrowing conversions, the trait evaluates to false.
 */
template <typename T, std::size_t N, typename... Args>
struct is_brace_initializable_array {
private:
    template <typename U, typename... A>
    static auto test(int) -> decltype(U{std::declval<A>()...}, std::true_type{});

    template <typename, typename...>
    static auto test(...) -> std::false_type;

public:
    static constexpr bool value = decltype(test<T[N], Args...>(0))::value;
};

/*!
 * \brief Helper trait to detect if Args... is exactly one ara::core::Array<T, N>
 *
 * \tparam Args  The types of arguments
 */
template<typename... Args>
struct is_single_same_array : std::false_type {};

// Partial specialization for a single ara::core::Array<T, N> argument
template<typename T, std::size_t N>
struct is_single_same_array<Array<T, N>> : std::true_type {};

// Variable template for convenience
template<typename... Args>
constexpr bool is_single_same_array_v = is_single_same_array<std::decay_t<Args>...>::value;

}  // namespace detail

/**********************************************************************************************************************
 *  CLASS: ara::core::Array
 *********************************************************************************************************************/
/*!
 * \brief  A fixed-size array template for the Adaptive AUTOSAR platform.
 *
 * \tparam T  Type of elements stored in the array.
 * \tparam N  Number of elements in the array.
 *
 * \details
 * - [SWS_CORE_11200]: Should behave like std::array, except at() uses Violations instead of exceptions.
 * - Logging out-of-range (fulfills [SWS_CORE_13017]).
 * - No exceptions for out-of-range, consistent with [SWS_CORE_00040].
 * - Complies with [SWS_CORE_01201], which defines the API class \a ara::core::Array.
 * - Provides fill(), swap(), and comparison operators as required by [SWS_CORE_01241], [SWS_CORE_01242], etc.
 *
 * \pre  Typically, we expect N > 0, though zero-sized arrays can be permitted if required by the specification.
 */
template <typename T, std::size_t N>
class Array final
{
public:
    // -----------------------------------------------------------------------------------
    // TYPE ALIASES (public) [SWS_CORE_01210..01220]
    // -----------------------------------------------------------------------------------
    using value_type             = T;                                                   /*!< [SWS_CORE_01216]: Type of the elements  */
    using size_type              = std::size_t;                                         /*!< [SWS_CORE_01214]: Used for indexing      */
    using difference_type        = std::ptrdiff_t;                                      /*!< [SWS_CORE_01215]: Used for pointer diffs */
    using reference              = T&;                                                  /*!< [SWS_CORE_01210]: Type of a reference    */
    using const_reference        = const T&;                                            /*!< [SWS_CORE_01211]: Type of a const-ref    */
    using pointer                = T*;                                                  /*!< [SWS_CORE_01217]: Pointer to element     */
    using const_pointer          = const T*;                                            /*!< [SWS_CORE_01218]: Const pointer          */
    using iterator               = T*;                                                  /*!< [SWS_CORE_01212]: Iterator type          */
    using const_iterator         = const T*;                                            /*!< [SWS_CORE_01213]: Const iterator type    */
    using reverse_iterator       = std::reverse_iterator<iterator>;                     /*!< [SWS_CORE_01219] */
    using const_reverse_iterator = std::reverse_iterator<const_iterator>;               /*!< [SWS_CORE_01220] */

    // -----------------------------------------------------------------------------------
    // 1) VARIADIC CONSTRUCTOR (CONSTRAINED)
    // -----------------------------------------------------------------------------------
    /*!
     * \brief Variadic constructor that can take up to N elements of type \c T.
     *
     * \tparam Args  Parameter pack of arguments to forward to \c T.
     * \pre (sizeof...(Args) <= N) AND each \c Arg is convertible to \c T 
     *      AND brace-initialization with Args... does not cause narrowing conversions.
     * \details
     * - This allows direct initialization like: 
     *       \code
     *       ara::core::Array<int,3> arr(10, 20, 30);
     *       \endcode
     * - If the user passes more than N arguments, it's a compile-time error via SFINAE.
     * - Ensures each argument is convertible to T (preventing accidental passing of
     *   e.g., an entire \c Array<TestClass,2> into the spot of a single \c TestClass).
     * - Brace-initialization is used to trigger compiler's narrowing conversion checks.
     * - Marked \c noexcept if all T constructions from Args are \c noexcept ([SWS_CORE_00040]).
     */
    template <typename... Args,
              typename = std::enable_if_t<
                  // Condition #1: Must not exceed N arguments
                  (sizeof...(Args) <= N) &&
                  // Condition #2: Each argument must be convertible to T
                  (std::conjunction_v<std::is_convertible<Args, T>...>) &&
                  // Condition #3: Brace-initialization does not cause narrowing
                  (detail::is_brace_initializable_array<T, N, Args...>::value) &&
                  // Condition #4: Prevent constructor from being selected when Args... is exactly Array<T, N>
                  (!detail::is_single_same_array_v<Args...>)
              >>
    constexpr Array(Args&&... args) 
        noexcept(std::conjunction_v<std::is_nothrow_constructible<T, Args&&>...>)
        : data_{std::forward<Args>(args)...} // Initialize data_ directly with args to trigger narrowing checks
    {
        // No further initialization required; compiler handles list-initialization of data_
    }

    // -----------------------------------------------------------------------------------
    // 2) REJECTING CONSTRUCTOR (TOO MANY OR WRONG TYPES)
    // -----------------------------------------------------------------------------------
    /*!
     * \brief Overload constructor that catches calls which violate the above conditions.
     *
     * \tparam Args  Parameter pack that either exceeds N or has arguments not convertible to T.
     * \note  This never actually constructs anything; it only fires `static_assert` errors.
     *
     * \details 
     * - If the user supplies (#args > N) or supplies arguments that are not convertible to T,
     *   then SFINAE excludes the first (normal) constructor and selects this constructor instead.
     * - We produce user-friendly error messages via \c static_assert.
     */
    template <
        typename... Args,
        // Condition: either too many arguments OR not all convertible OR narrowing conversion
        typename = std::enable_if_t<
            (sizeof...(Args) > N) ||
            (!detail::is_single_same_array_v<Args...>) && 
            ((!std::conjunction_v<std::is_convertible<Args, T>...>) ||
            (!detail::is_brace_initializable_array<T, N, Args...>::value))
        >,
        // Dummy int parameter to differentiate overload signatures (prevents overload conflict)
        int = 0
    >
    constexpr Array(Args&&...)
    {
        static_assert(sizeof...(Args) <= N,
            "\n[ERROR] Too many arguments passed to Array<T,N> constructor!\n"
            "        You can only pass up to N elements in ara::core::Array.\n");

        static_assert(std::conjunction_v<std::is_convertible<Args, T>...>,
            "\n[ERROR] One or more constructor arguments cannot be converted to type T.\n"
            "        Please ensure all arguments match T (or are convertible) in ara::core::Array.\n");

        static_assert(detail::is_brace_initializable_array<T, N, Args...>::value,
            "\n[ERROR] Brace-initialization would cause narrowing conversions in ara::core::Array.\n"
            "        Please ensure all arguments can initialize the array without narrowing.\n");
    }

    // -----------------------------------------------------------------------------------
    // 3) TEMPLATE ASSIGNMENT OPERATOR FOR CROSS-TYPE/SIZE ASSIGNMENT
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Template assignment operator to handle assignment from Array<U, M>.
     *
     * \tparam U The type of elements in the source array.
     * \tparam M The size of the source array.
     *
     * \details
     * - This operator is enabled only if U is not the same as T or M is not equal to N.
     * - Inside, a static_assert checks that U is the same as T and M equals N.
     * - If the conditions are met, it performs an element-wise assignment.
     * - Otherwise, it triggers a compile-time error with a descriptive message.
     */
    template <typename U, std::size_t M>
    constexpr Array& operator=(const Array<U, M>& other)
        noexcept(std::is_nothrow_assignable_v<T&, U>)
    {
        static_assert(std::is_same_v<U, T>,
            "\n[ERROR] Cannot assign ara::core::Array<U,M> to ara::core::Array<T,N>.\n"
            "        Ensure that U is the same type as T and that M equals N in ara::core::Array.\n");

        static_assert((M == N),
            "\n[ERROR] Cannot assign ara::core::Array<U,M> to ara::core::Array<T,N>.\n"
            "        Ensure that both arrays have the same size in ara::core::Array.\n");

        for (std::size_t i = 0; i < N; ++i) {
            data_[i] = other[i];
        }

        return *this;
    }

    // -----------------------------------------------------------------------------------
    // 4) EXPLICIT COPY/MOVE OPERATIONS any issue will be detected by default
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Explicitly declare copy/move constructors and operators.
     *
     * \details
     * - This ensures that when the user writes \c Array<T,N> arrB = arrA, the compiler
     *   uses the normal copy constructor rather than the \e variadic constructor. 
     * - If these are not declared, the compiler might try to match the single-argument
     *   constructor that expects \c T but is inadvertently passed \c Array<T,N>.
     */
    constexpr Array(const Array&) = default;                /*!< Copy constructor */
    constexpr Array(Array&&) noexcept = default;            /*!< Move constructor */
    constexpr Array& operator=(const Array&) = default;     /*!< Copy assignment  */
    constexpr Array& operator=(Array&&) noexcept = default; /*!< Move assignment  */

    /*!
     * \brief Default constructor: all elements default-constructed.
     * \note Satisfies [SWS_CORE_01201].
     */
    constexpr Array() noexcept = default;

    // -----------------------------------------------------------------------------------
    // OPERATOR[]
    // [SWS_CORE_01265], [SWS_CORE_01266]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Unchecked array subscript (mutable).
     * \param  idx Index in range [0..N-1].
     * \return Reference to the element at that index.
     * \note   Out-of-bounds => undefined behavior, so \e no violation triggered here.
     *         Use \c at() for a checked access.
     */
    constexpr auto operator[](size_type idx) noexcept -> reference {
        return at(idx);
    }

    /*!
     * \brief  Unchecked array subscript (const).
     * \param  idx Index in range [0..N-1].
     * \return \c const_reference to the element at that index.
     */
    constexpr auto operator[](size_type idx) const noexcept -> const_reference {
        return at(idx);
    }

    // -----------------------------------------------------------------------------------
    // at()
    // [SWS_CORE_01273], [SWS_CORE_01274]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  \e Checked element access (mutable).
     * \param  idx The index to access.
     * \return Reference to the element.
     * \note   If idx >= N, triggers \c detail::TriggerOutOfRangeViolation => terminates.
     */
    constexpr auto at(size_type idx) noexcept -> reference {
        if (idx >= N) {
            detail::TriggerOutOfRangeViolation(
                "Array",
                ARA_CORE_INTERNAL_FILELINE,  // capture file:line from location_utils
                idx,
                N
            );
        }
        return data_[idx];
    }

    /*!
     * \brief  \e Checked element access (const).
     * \param  idx The index to access.
     * \return Const reference to the element.
     * \note   If idx >= N, triggers \c detail::TriggerOutOfRangeViolation => terminates.
     */
    constexpr auto at(size_type idx) const noexcept -> const_reference {
        if (idx >= N) {
            detail::TriggerOutOfRangeViolation(
                "Array",
                ARA_CORE_INTERNAL_FILELINE,
                idx,
                N
            );
        }
        return data_[idx];
    }

    // -----------------------------------------------------------------------------------
    // front(), back() 
    // [SWS_CORE_01267], [SWS_CORE_01268], [SWS_CORE_01269], [SWS_CORE_01270]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a \e mutable reference to the first element (like std::array::front).
     * \return Reference to the first element.
     * \note   static_assert(N > 0) so this is a compile-time error if N=0.
     */
    constexpr auto front() noexcept -> reference {
        static_assert(N > 0,
            "\n[ERROR] front() called on zero-sized Array.\n"
            "        front() shall be called upon a non-zero sized Array in ara::core::Array.\n");
        return data_[0];
    }

    /*!
     * \brief  Returns a \e const reference to the first element.
     * \return Const reference to the first element.
     * \note   static_assert(N > 0) so this is a compile-time error if N=0.
     */
    constexpr auto front() const noexcept -> const_reference {
        static_assert(N > 0,
            "\n[ERROR] front() called on zero-sized Array.\n"
            "        front() shall be called upon a non-zero sized Array in ara::core::Array.\n");
        return data_[0];
    }

    /*!
     * \brief  Returns a \e mutable reference to the last element.
     * \return Reference to the last element.
     * \note   static_assert(N > 0) so this is a compile-time error if N=0.
     */
    constexpr auto back() noexcept -> reference {
        static_assert(N > 0,
            "\n[ERROR] back() called on zero-sized Array.\n"
            "        back() shall be called upon a non-zero sized Array in ara::core::Array.\n");
        return data_[N - 1];
    }

    /*!
     * \brief  Returns a \e const reference to the last element.
     * \return Const reference to the last element.
     * \note   static_assert(N > 0) so this is a compile-time error if N=0.
     */
    constexpr auto back() const noexcept -> const_reference {
        static_assert(N > 0,
            "\n[ERROR] back() called on zero-sized Array.\n"
            "        back() shall be called upon a non-zero sized Array in ara::core::Array.\n");
        return data_[N - 1];
    }

    // -----------------------------------------------------------------------------------
    // data()
    // [SWS_CORE_01271], [SWS_CORE_01272]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns a pointer to the first element (mutable).
     * \return If N==0, returns nullptr; else &data_[0].
     */
    constexpr auto data() noexcept -> pointer {
        if constexpr (N == 0) {
            return nullptr;
        }
        return data_;
    }

    /*!
     * \brief  Returns a pointer to the first element (const).
     * \return If N==0, returns nullptr; else &data_[0].
     */
    constexpr auto data() const noexcept -> const_pointer {
        if constexpr (N == 0) {
            return nullptr;
        }
        return data_;
    }

    // -----------------------------------------------------------------------------------
    // size(), max_size(), empty()
    // [SWS_CORE_01262], [SWS_CORE_01263], [SWS_CORE_01264]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns the number of elements (N).
     * \return The number of elements in the array.
     */
    constexpr auto size() const noexcept -> size_type {
        return N;
    }

    /*!
     * \brief  Returns maximum number of elements (same as size for fixed array).
     * \return The maximum number of elements in the array.
     */
    constexpr auto max_size() const noexcept -> size_type {
        return N;
    }

    /*!
     * \brief  Returns true if the array is empty (N==0).
     * \return \c true if the array is empty; \c false otherwise.
     */
    constexpr auto empty() const noexcept -> bool {
        return (N == 0);
    }

    // -----------------------------------------------------------------------------------
    // ITERATORS
    // [SWS_CORE_01250..01261]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Returns an iterator to the first element (mutable).
     * \return Iterator to the first element.
     */
    constexpr auto begin() noexcept -> iterator {
        return data();
    }

    /*!
     * \brief  Returns a const_iterator to the first element.
     * \return Const iterator to the first element.
     */
    constexpr auto begin() const noexcept -> const_iterator {
        return data();
    }

    /*!
     * \brief  Returns an iterator to one-past-the-last element (mutable).
     * \return Iterator to one-past-the-last element.
     */
    constexpr auto end() noexcept -> iterator {
        return data() + N;
    }

    /*!
     * \brief  Returns a const_iterator to one-past-the-last element.
     * \return Const iterator to one-past-the-last element.
     */
    constexpr auto end() const noexcept -> const_iterator {
        return data() + N;
    }

    /*!
     * \brief  Returns a reverse_iterator to the last element (mutable).
     * \return Reverse iterator to the last element.
     */
    constexpr auto rbegin() noexcept -> reverse_iterator {
        return reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator to the last element.
     * \return Const reverse iterator to the last element.
     */
    constexpr auto rbegin() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a const_reverse_iterator to the last element (const).
     * \return Const reverse iterator to the last element.
     */
    constexpr auto crbegin() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator(end());
    }

    /*!
     * \brief  Returns a reverse_iterator to one-before-the-first element.
     * \return Reverse iterator to one-before-the-first element.
     */
    constexpr auto rend() noexcept -> reverse_iterator {
        return reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator to one-before-the-first element.
     * \return Const reverse iterator to one-before-the-first element.
     */
    constexpr auto rend() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator(begin());
    }

    /*!
     * \brief  Returns a const_reverse_iterator to one-before-the-first element (const).
     * \return Const reverse iterator to one-before-the-first element.
     */
    constexpr auto crend() const noexcept -> const_reverse_iterator {
        return const_reverse_iterator(begin());
    }

    // -----------------------------------------------------------------------------------
    // fill()
    // [SWS_CORE_01241]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Assign the given value to all elements of this Array.
     * \param  val The value to copy into each element.
     * \note   The operation is conditionally \c noexcept if T's copy assignment is \c noexcept.
     */
    constexpr auto fill(const T& val) noexcept(std::is_nothrow_copy_assignable<T>::value) -> void {
        for (size_type i = 0; i < N; ++i) {
            data_[i] = val;
        }
    }

    // -----------------------------------------------------------------------------------
    // swap()
    // [SWS_CORE_01242]
    // -----------------------------------------------------------------------------------
    /*!
     * \brief  Exchanges contents with another Array of the same size \c N.
     * \param  other The other Array to swap with.
     * \note   Uses std::swap for each element; conditionally \c noexcept if T is \c nothrow swappable.
     */
    constexpr auto swap(Array& other) noexcept(noexcept(std::swap(std::declval<T&>(), std::declval<T&>()))) -> void {
        for (size_type i = 0; i < N; ++i) {
            std::swap(data_[i], other.data_[i]);
        }
    }

private:
    // -----------------------------------------------------------------------------------
    // STORAGE
    // [SWS_CORE_01201]
    // -----------------------------------------------------------------------------------
    T data_[N]{}; /*!< Underlying storage for the array elements, default-initialized. */
};

/**********************************************************************************************************************
 *  NON-MEMBER FUNCTIONS
 *********************************************************************************************************************/

// -----------------------------------------------------------------------------------
// get<I> (lvalue, rvalue, const) [SWS_CORE_01282], [SWS_CORE_01283], [SWS_CORE_01284]
// -----------------------------------------------------------------------------------
/*!
 * \brief   Retrieves the I-th element (mutable reference) from \c arr.
 * \tparam  I   The compile-time index.
 * \tparam  T   The element type.
 * \tparam  N   The array size.
 *
 * \details
 * - If \c I >= N, compile-time static_assert fails ("out of range").
 */
template <std::size_t I, typename T, std::size_t N>
constexpr auto get(Array<T, N>& arr) noexcept -> T& {
    static_assert(I < N,
        "\n[ERROR] get<I>() out of range!\n"
        "        get shall be in range of Array in ara::core::Array.\n");
    return arr[I];
}

/*!
 * \brief   Retrieves the I-th element (rvalue reference) from an rvalue \c arr.
 * \tparam  I   The compile-time index.
 * \tparam  T   The element type.
 * \tparam  N   The array size.
 *
 * \details
 * - If \c I >= N, compile-time static_assert fails ("out of range").
 */
template <std::size_t I, typename T, std::size_t N>
constexpr auto get(Array<T, N>&& arr) noexcept -> T&& {
    static_assert(I < N,
        "\n[ERROR] get<I>() out of range!\n"
        "        get shall be in range of Array in ara::core::Array.\n");
    return std::move(arr[I]);
}

/*!
 * \brief   Retrieves the I-th element (const reference) from a \c const \c arr.
 * \tparam  I   The compile-time index.
 * \tparam  T   The element type.
 * \tparam  N   The array size.
 *
 * \details
 * - If \c I >= N, compile-time static_assert fails ("out of range").
 */
template <std::size_t I, typename T, std::size_t N>
constexpr auto get(const Array<T, N>& arr) noexcept -> const T& {
    static_assert(I < N,
        "\n[ERROR] get<I>() out of range!\n"
        "        get shall be in range of Array in ara::core::Array.\n");
    return arr[I];
}

// -----------------------------------------------------------------------------------
// Comparison operators [SWS_CORE_01290..01295]
// -----------------------------------------------------------------------------------
/*!
 * \brief  Checks if two Arrays have \e equal content (elementwise).
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if all elements are equal; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator==(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() == std::declval<T&>()))
    -> bool
{
    for (std::size_t i = 0; i < N; ++i) {
        if (!(lhs[i] == rhs[i])) {
            return false;
        }
    }
    return true;
}

/*!
 * \brief  Checks if two Arrays differ in content.
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if any element differs; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator!=(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() != std::declval<T&>()))
    -> bool
{
    return !(lhs == rhs);
}

/*!
 * \brief  Lexicographical compare: returns true if \c lhs < \c rhs.
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if \c lhs is lexicographically less than \c rhs; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator<(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() < std::declval<T&>()))
    -> bool
{
    return std::lexicographical_compare(lhs.begin(), lhs.end(),
                                        rhs.begin(), rhs.end());
}

/*!
 * \brief  Lexicographical compare: returns true if \c lhs <= \c rhs.
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if \c lhs is lexicographically less than or equal to \c rhs; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator<=(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() <= std::declval<T&>()))
    -> bool
{
    return !(rhs < lhs);
}

/*!
 * \brief  Lexicographical compare: returns true if \c lhs > \c rhs.
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if \c lhs is lexicographically greater than \c rhs; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator>(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() > std::declval<T&>()))
    -> bool
{
    return rhs < lhs;
}

/*!
 * \brief  Lexicographical compare: returns true if \c lhs >= \c rhs.
 *
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to compare.
 * \param  rhs The second array to compare.
 * \return \c true if \c lhs is lexicographically greater than or equal to \c rhs; \c false otherwise.
 */
template <typename T, std::size_t N>
auto operator>=(const Array<T, N>& lhs, const Array<T, N>& rhs)
    noexcept(noexcept(std::declval<T&>() >= std::declval<T&>()))
    -> bool
{
    return !(lhs < rhs);
}

/*!
 * \brief  Overload of std::swap for ara::core::Array, calls Array::swap internally.
 * \tparam T  The type of elements stored in the arrays.
 * \tparam N  The number of elements in the arrays.
 * \param  lhs The first array to swap.
 * \param  rhs The second array to swap.
 *
 * \details
 * - This function enables the use of \c std::swap with ara::core::Array.
 * - It delegates the swapping to the member \c swap() function of Array.
 */
template <typename T, std::size_t N>
auto swap(Array<T, N>& lhs, Array<T, N>& rhs)
    noexcept(noexcept(lhs.swap(rhs)))
    -> void
{
    lhs.swap(rhs);
}

/*!
 * \brief  Attempting to swap arrays of different types or sizes => fail with a user-friendly message.
 *
 * \tparam T  The type of elements in the first array.
 * \tparam N  The number of elements in the first array.
 * \tparam U  The type of elements in the second array.
 * \tparam M  The number of elements in the second array.
 * \param  lhs The first array to swap.
 * \param  rhs The second array to swap.
 *
 * \details
 * - This is purely optional. Normally, the overload for \c Array<T,N> won't match
 *   \c Array<U,M> if (T != U) or (N != M). But if you want a more explicit message:
 *   - The template is enabled only if \c T and \c U are different or \c N and \c M are different.
 *   - Inside, a \c static_assert enforces that \c T and \c U are the same and \c N and \c M are equal.
 *   - If the conditions are not met, it triggers a compile-time error with a descriptive message.
 */
template <typename T, std::size_t N, typename U, std::size_t M>
constexpr auto swap(Array<T, N>& /*lhs*/, Array<U, M>& /*rhs*/)
    -> std::enable_if_t<!(std::is_same_v<T, U> && (N == M)), void>
{
    static_assert(std::is_same_v<T, U>,
        "\n[ERROR] Cannot swap arrays of different types!\n"
        "        (swap(Array<T,N>&, Array<U,M>&)) in ara::core::Array.\n");
    static_assert(N == M,
        "\n[ERROR] Cannot swap arrays of different sizes!\n"
        "        (swap(Array<T,N>&, Array<U,M>&)) in ara::core::Array.\n");
}

} // namespace core
} // namespace ara

#endif // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_CORE_ARRAY_H_
