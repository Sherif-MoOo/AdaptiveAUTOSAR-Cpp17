/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/qnx/process/process.cpp
 *  \brief      QNX-specific implementation of the ara::os::interface::process::ProcessInteraction interface.
 *
 *  \details    Implements the GetProcessName method using QNX system calls and standard libraries.
 *
 *  \note       This implementation ensures thread safety and handles potential errors gracefully.
 ***********************************************************************************************************************/

#include "ara/os/qnx/process/process.h"
#include "ara/core/array.h" // Include the ara::core::Array template

#include <sys/proc_info.h> // For proc_name
#include <unistd.h>        // For getpid
#include <cstring>         // For std::strncpy

namespace ara {
namespace os {
namespace qnx {
namespace process {

/**********************************************************************************************************************
 *  FUNCTION: ProcessInteractionImpl::GetProcessName
 *********************************************************************************************************************/
/*!
 * \brief  Retrieves the name of the current process.
 *
 * \param[out] buffer      Pointer to the buffer where the process name will be stored.
 * \param[in]  bufferSize  Size of the provided buffer in bytes.
 *
 * \return An ara::os::interface::process::ErrorCode indicating the result of the operation.
 *
 * \note   This method avoids exceptions and uses safe string operations to prevent buffer overflows.
 */
auto ProcessInteractionImpl::GetProcessName(char* buffer, std::size_t bufferSize) const noexcept -> ara::os::interface::process::ErrorCode {
    /* Validate input parameters */
    if (buffer == nullptr) {
        return ara::os::interface::process::ErrorCode::NullBuffer;
    }

    /* Define the maximum process name length */
    constexpr std::size_t max_name_length = PROC_NAME_MAX;
    ara::core::Array<char, max_name_length> pname{};
    pid_t pid = getpid();

    /* Retrieve the process name using QNX-specific system call */
    int proc_result = proc_name(pid, pname.data(), pname.size());
    if (proc_result == -1) {
        /* proc_name failed */
        return ara::os::interface::process::ErrorCode::RetrievalFailed;
    }

    /* Validate the retrieved process name */
    if (std::strlen(pname.data()) == 0) {
        /* Empty process name retrieved */
        return ara::os::interface::process::ErrorCode::RetrievalFailed;
    }

    /* Calculate the length of the process name */
    std::size_t name_length = std::strlen(pname.data());

    /* Ensure the buffer is large enough (including null terminator) */
    if (name_length + 1 > bufferSize) {
        return ara::os::interface::process::ErrorCode::BufferTooSmall;
    }

    /* Securely copy the process name into the buffer */
    ara::core::Array<char, max_name_length> temp_buffer{};
    std::strncpy(temp_buffer.data(), pname.data(), bufferSize - 1);
    temp_buffer[bufferSize - 1] = '\0'; // Ensure null-termination

    std::strncpy(buffer, temp_buffer.data(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0'; // Ensure null-termination

    return ara::os::interface::process::ErrorCode::Success;
}

/**********************************************************************************************************************
 *  FUNCTION: CreateProcessInteractionInstance
 *********************************************************************************************************************/
/*!
 * \brief  Factory function to create a QNX-specific ProcessInteraction instance.
 *
 * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
 */
auto CreateProcessInteractionInstance() -> std::unique_ptr<ara::os::interface::process::ProcessInteraction> {
    return std::make_unique<ProcessInteractionImpl>();
}

} // namespace process
} // namespace qnx
} // namespace os
} // namespace ara
