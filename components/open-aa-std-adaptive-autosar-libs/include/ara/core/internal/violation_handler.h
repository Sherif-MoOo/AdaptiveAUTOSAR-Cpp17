/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/core/internal/ViolationHandler.h
 *  \brief      Definition of the ara::core::internal::ViolationHandler class.
 *
 *  \details    This file declares the ara::core::internal::ViolationHandler class, which is responsible for handling
 *              violations as per AUTOSAR Adaptive Platform requirements. It ensures that violations are logged
 *              appropriately and that the process is terminated in a controlled manner.
 *
 *              The ViolationHandler follows the Singleton design pattern to provide a single point of violation
 *              management across the system. It is designed to be thread-safe and noexcept where applicable,
 *              aligning with the safety and reliability standards of automotive systems.
 *
 *  \note       Based on the Adaptive AUTOSAR SWS (e.g., R24-11) requirements, especially:
 *              - [SWS_CORE_00040] (Errors originating from C++ standard classes)
 *              - [SWS_CORE_13017] (ViolationMessage ArrayAccessOutOfRangeViolation)
 *              - [SWS_CORE_00090] (Handling of Standardized Violations)
 *********************************************************************************************************************/
#ifndef ARA_CORE_INTERNAL_VIOLATION_HANDLER_H_
#define ARA_CORE_INTERNAL_VIOLATION_HANDLER_H_

/**********************************************************************************************************************
 *  NAMESPACE: ara::core::internal
 *********************************************************************************************************************/
namespace ara {
namespace core {
/*!
 * \brief  Forward declaration of the Array class template.
 */
template <typename T, std::size_t N>
class Array;

namespace internal {

/**********************************************************************************************************************
 *  CLASS: ViolationHandler
 *********************************************************************************************************************/
/*!
 * \brief  Singleton class responsible for handling violations within the ara::core::Array class.
 *
 * \details
 * The ViolationHandler class manages the logging and termination processes when violations occur.
 * It adheres to AUTOSAR requirements by ensuring that violations are logged using the Diagnostic Log and
 * Trace (DLT) mechanism and that the process is terminated in a controlled manner upon detecting a violation.
 *
 * This class cannot be instantiated directly. Instead, the singleton instance is accessed via the
 * Instance() method. The class is designed to be thread-safe and noexcept where applicable to meet
 * the stringent safety and reliability standards of automotive systems.
 *
 * \note   [SWS_CORE_00090], [SWS_CORE_13017]
 */
class ViolationHandler final {
public:
    /*!
     * \brief  Retrieves the singleton instance of ViolationHandler.
     *
     * \return Reference to the ViolationHandler instance.
     *
     * \note   Ensures that only one instance exists throughout the application lifecycle.
     */
    static auto Instance() noexcept -> ViolationHandler&;


private:
    /*!
     * \brief  Private constructor to enforce the Singleton pattern.
     *
     * \details
     * Prevents external instantiation of the ViolationHandler class. Only the Instance() method
     * can access this constructor to create the singleton instance.
     */
    ViolationHandler() noexcept = default;

    /*!
     * \brief  Deletes the copy constructor.
     *
     * \details
     * Prevents copying of the singleton instance.
     */
    ViolationHandler(const ViolationHandler&) = delete;

    /*!
     * \brief  Deletes the move constructor.
     *
     * \details
     * Prevents moving of the singleton instance.
     */
    ViolationHandler(ViolationHandler&&) = delete;

    /*!
     * \brief  Deletes the copy assignment operator.
     *
     * \details
     * Prevents copy assignment of the singleton instance.
     */
    auto operator=(const ViolationHandler&) noexcept -> ViolationHandler& = delete;

    /*!
     * \brief  Deletes the move assignment operator.
     *
     * \details
     * Prevents move assignment of the singleton instance.
     */
    auto operator=(ViolationHandler&&) noexcept -> ViolationHandler& = delete;

    /*!
     * \brief  Triggers an ArrayAccessOutOfRangeViolation.
     *
     * \param  location     An implementation-defined identifier of the location where the violation was detected
     *                      (e.g., "file.cpp:123").
     * \param  indexValue   The index that was out of range.
     * \param  arraySize    The size of the array.
     *
     * \details
     * Logs a violation message following the format specified by [SWS_CORE_13017] and terminates the process
     * abnormally as per [SWS_CORE_00090]. This method is noexcept and does not throw exceptions.
     *
     * \note   [SWS_CORE_13017], [SWS_CORE_00090]
     */
    [[noreturn]] auto TriggerArrayAccessOutOfRangeViolation(std::string_view location,
                                               std::size_t indexValue,
                                               std::size_t arraySize) noexcept -> void;
                                               
    /*!
     * \brief  Handles the termination of the process upon violation detection.
     *
     * \details
     * Logs a fatal error message to std::cerr and calls std::terminate() to abort the process.
     * This method is noexcept and marked as [[noreturn]] to indicate that it does not return.
     *
     * \note   [SWS_CORE_00090]
     */
    [[noreturn]] auto Abort() noexcept -> void;

    /*!
     * \brief  Retrieves the identifier of the current process.
     *
     * \return The process identifier as a std::string.
     *
     * \details
     * Interacts with the OS Abstraction Layer to obtain the name of the current process. If the process name
     * cannot be retrieved, defaults to "UnknownProcess" or "UnsupportedPlatform" based on the context.
     *
     * \note   [SWS_CORE_00090]
     */
    auto GetProcessIdentifier() noexcept -> std::string;
    
    /*!
     * \brief  Grants friendship to the ara::core::Array class to allow exclusive access.
     *
     * \tparam T  The type of elements in the Array.
     * \tparam N  The number of elements in the Array.
     *
     * \note   Ensures that ara::core::Array allowed trigger violations.
     */
    template <typename T, std::size_t N>
    friend class ara::core::Array;
};

} // namespace internal
} // namespace core
} // namespace ara

#endif // ARA_CORE_INTERNAL_VIOLATION_HANDLER_H_
