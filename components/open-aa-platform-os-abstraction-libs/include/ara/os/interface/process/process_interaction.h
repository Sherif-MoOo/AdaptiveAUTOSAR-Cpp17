/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/interface/process/process_interaction.h
 *  \brief      Definition of the ara::os::interface::process::ProcessInteraction interface.
 *
 *  \details    This file defines the ara::os::interface::process::ProcessInteraction interface for retrieving
 *              process-related information.
 *
 *  \note       While not specified by AUTOSAR requirements, this interface is essential for creating a generic
 *              platform solution to abstract OS-specific functionalities.
 ***********************************************************************************************************************/

#ifndef OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_INTERACTION_H_
#define OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_INTERACTION_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*!
 * \brief  Includes necessary standard headers for process operations.
 *
 * [SWS_CORE_01214], [SWS_CORE_01215]: cstddef for std::size_t
 * [SWS_CORE_01216]: cstdint for fixed-width integer types
 * [SWS_CORE_01212], [SWS_CORE_01213]: cstring for std::strlen, std::strncpy
 */
#include <cstddef>      // For std::size_t
#include <cstdint>      // For fixed-width integer types
#include <cstring>      // For std::strlen, std::strncpy

namespace ara {
namespace os {
namespace interface {
namespace process {

/**********************************************************************************************************************
 *  ENUM: ErrorCode
 *********************************************************************************************************************/
/*!
 * \brief  Enumeration of possible error codes for ProcessInteraction operations.
 *
 * \details
 * - Defines standard error codes to indicate the result of ProcessInteraction interface operations.
 * - Facilitates precise error handling and debugging.
 */
enum class ErrorCode : uint8_t {
    Success = 0,                   /*!< Operation completed successfully */
    BufferTooSmall,                /*!< Provided buffer is too small */
    RetrievalFailed,               /*!< Failed to retrieve the process name */
    NullBuffer,                    /*!< Provided buffer pointer is null */
    UnknownError                   /*!< An unknown error occurred */
};

/**********************************************************************************************************************
 *  CLASS: ProcessInteraction
 *********************************************************************************************************************/
/*!
 * \brief  Abstract interface for process-related functionalities.
 *
 * \details
 * - Provides methods to retrieve process-specific information in a platform-agnostic manner.
 * - Implementations must ensure thread safety and handle any platform-specific nuances.
 *
 * \note   This interface serves as an abstraction layer to interact with different operating systems seamlessly.
 */
class ProcessInteraction {
public:
    /*!
     * \brief  Virtual destructor for proper cleanup of derived classes.
     */
    virtual ~ProcessInteraction() = default;

    /*!
     * \brief  Retrieves the name of the current process.
     *
     * \param[out] buffer      Pointer to the buffer where the process name will be stored.
     * \param[in]  bufferSize  Size of the provided buffer in bytes.
     *
     * \return An ara::os::interface::process::ErrorCode indicating the result of the operation.
     *
     * \note   Implementations must ensure that this method is thread-safe.
     *         The process name is null-terminated if successfully retrieved.
     */
    virtual auto GetProcessName(char* buffer, std::size_t bufferSize) const noexcept -> ErrorCode = 0;
};

} // namespace process
} // namespace interface
} // namespace os
} // namespace ara

#endif // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_INTERACTION_H_
