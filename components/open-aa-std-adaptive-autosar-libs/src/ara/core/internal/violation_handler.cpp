/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/core/internal/ViolationHandler.cpp
 *  \brief      Implementation of the ara::core::internal::ViolationHandler class.
 *
 *  \details    This file provides the implementation of the ara::core::internal::ViolationHandler class, which
 *              handles violations by logging diagnostic messages and terminating the process as required
 *              by AUTOSAR specifications.
 *
 *              The class ensures that violation messages are formatted according to [SWS_CORE_13017] and that
 *              the process is terminated in a controlled and standardized manner.
 *
 *  \note       Based on the Adaptive AUTOSAR SWS (e.g., R24-11) requirements, especially:
 *              - [SWS_CORE_00040] (Errors originating from C++ standard classes)
 *              - [SWS_CORE_13017] (ViolationMessage ArrayAccessOutOfRangeViolation)
 *              - [SWS_CORE_00090] (Handling of Standardized Violations)
 *********************************************************************************************************************/
/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <cstddef>       // For std::size_t
#include <string>        // For std::string
#include <iostream>      // For std::cerr
#include <cstdlib>       // For std::terminate
#include <cstring>       // For std::strncpy

// Include OS Abstraction Layer headers for ProcessInteraction
#include "ara/os/interface/process/process_factory.h"       // For ProcessFactory

#include <string_view>                                      // Required for std::string_view
#include "ara/core/internal/violation_handler.h"

namespace ara {
namespace core {
namespace internal {

/**********************************************************************************************************************
 *  FUNCTION: ViolationHandler::Instance
 *********************************************************************************************************************/
/*!
 * \brief  Retrieves the singleton instance of ViolationHandler.
 *
 * \return Reference to the ViolationHandler instance.
 *
 * \details
 * Utilizes a local static variable to ensure thread-safe initialization as per C++11 and later standards.
 *
 * \note   [SWS_CORE_00090]
 */
auto ViolationHandler::Instance() noexcept -> ViolationHandler&
{
    static ViolationHandler instance;
    return instance;
}

/**********************************************************************************************************************
 *  FUNCTION: ViolationHandler::TriggerArrayAccessOutOfRangeViolation
 *********************************************************************************************************************/
/*!
 * \brief  Triggers an ArrayAccessOutOfRangeViolation.
 *
 * \param  location     An implementation-defined identifier of the location where the violation was detected
 *                      (e.g., "file.cpp:123").
 * \param  indexValue   The index that was out of range.
 * \param  arraySize    The size of the array.
 *
 * \details
 * Constructs the violation message as per [SWS_CORE_13017], logs it, and terminates the process.
 *
 * \note   [SWS_CORE_13017], [SWS_CORE_00090]
 */
[[noreturn]] auto ViolationHandler::TriggerArrayAccessOutOfRangeViolation(std::string_view location,
                                                              std::size_t indexValue,
                                                              std::size_t arraySize) noexcept -> void
{
    std::cerr << "[App vlt][FATAL]: Violation detected in " << GetProcessIdentifier()
              << " at " << location
              << ": Array access out of range: Tried to access "
              << indexValue << " in array of size " << arraySize << ".\n";

    // Terminate the process as per AUTOSAR requirements
    Abort();
}


/**********************************************************************************************************************
 *  FUNCTION: ViolationHandler::Abort
 *********************************************************************************************************************/
/*!
 * \brief  Handles the termination of the process upon violation detection.
 *
 * \details
 * Logs a fatal error message to std::cerr and calls std::terminate() to abort the process.
 *
 * \note   [SWS_CORE_00090]
 */
[[noreturn]] auto ViolationHandler::Abort() noexcept -> void
{
    std::cerr << "FATAL: Process aborted due to a critical violation in ara::core::Array.\n";
    std::terminate();
}

/**********************************************************************************************************************
 *  FUNCTION: ViolationHandler::GetProcessIdentifier
 *********************************************************************************************************************/
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
auto ViolationHandler::GetProcessIdentifier() noexcept -> std::string
{
    constexpr std::size_t process_name_buffer_size = 256;
    char buf[process_name_buffer_size] = {0};

    auto processInteraction = ara::os::interface::process::ProcessFactory::CreateInstance();
    if (processInteraction) {
        auto error = processInteraction->GetProcessName(buf, process_name_buffer_size);
        if (error != ara::os::interface::process::ErrorCode::Success) {
            std::strncpy(buf, "UnknownProcess", process_name_buffer_size - 1);
        }
    } else {
        std::strncpy(buf, "UnsupportedPlatform", process_name_buffer_size - 1);
    }

    return std::string(buf);
}



} // namespace internal
} // namespace core
} // namespace ara
