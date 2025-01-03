/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/linux/process/process.cpp
 *  \brief      Linux-specific implementation of the ara::os::interface::process::ProcessInteraction interface.
 *
 *  \details    Implements the GetProcessName method using /proc/<pid>/comm to retrieve the short process name.
 *              On Linux, /proc/<pid>/comm typically returns the "comm name," which is often truncated (15 or 16 bytes).
 *
 *  \note       This implementation is thread-safe, uses safe string operations, and handles potential errors gracefully.
 *              It does NOT retrieve the full executable path. For that, you'd typically use /proc/<pid>/exe (via readlink).
 ***********************************************************************************************************************/

#include "ara/os/linux/process/process.h"
#include "ara/core/array.h" // Include the ara::core::Array template

#include <unistd.h>     // For getpid()
#include <sys/types.h>  // For pid_t
#include <fstream>      // For std::ifstream
#include <cstring>      // For std::strncpy
#include <cstdio>       // For std::snprintf

namespace ara {
namespace os {
namespace linux {
namespace process {

/**********************************************************************************************************************
 *  FUNCTION: ProcessInteractionImpl::GetProcessName
 *********************************************************************************************************************/
/*!
 * \brief  Retrieves the short name of the current process in Linux (as shown in /proc/<pid>/comm).
 *
 * \param[out] buffer      Pointer to the buffer where the process name will be stored.
 * \param[in]  bufferSize  Size of the provided buffer in bytes.
 *
 * \return An ara::os::interface::process::ErrorCode indicating the result of the operation:
 *         - Success:        The process name was retrieved successfully.
 *         - NullBuffer:     The output buffer pointer was null.
 *         - BufferTooSmall: The provided buffer is too small to hold the process name.
 *         - RetrievalFailed:An unexpected error occurred (e.g., file I/O failure).
 *
 * \warning /proc/<pid>/comm can be truncated at 15 or 16 characters by the kernel. If the real process name is longer,
 *          only the first 15 or 16 characters may be retrieved.
 *
 * \note   - This method avoids throwing exceptions.
 *         - Ensures no buffer overflow by using safe string copy operations.
 *         - The returned name is typically the short command name (no path, no args).
 */
auto ProcessInteractionImpl::GetProcessName(char* buffer, std::size_t bufferSize) const noexcept
    -> ara::os::interface::process::ErrorCode
{
    using ErrorCode = ara::os::interface::process::ErrorCode;

    /* 1. Validate input parameters */
    if (buffer == nullptr) {
        return ErrorCode::NullBuffer;
    }
    if (bufferSize == 0) {
        return ErrorCode::BufferTooSmall; // No space to store anything.
    }

    /* 2. Construct the path to /proc/<pid>/comm */
    pid_t pid = getpid();
    constexpr std::size_t maxPathLength = 256;
    ara::core::Array<char, maxPathLength> procPath{};
    int snprintfResult = std::snprintf(procPath.data(), maxPathLength, "/proc/%d/comm", pid);
    if (snprintfResult < 0 || static_cast<std::size_t>(snprintfResult) >= maxPathLength) {
        // snprintf failed or truncated
        return ErrorCode::RetrievalFailed;
    }

    /* 3. Open the /proc/<pid>/comm file to read the process name */
    std::ifstream commFile(procPath.data(), std::ios::in);
    if (!commFile.is_open()) {
        // Could not open /proc/<pid>/comm
        return ErrorCode::RetrievalFailed;
    }

    /* 4. Read the process name (short name) from the comm file */
    std::string processName;
    std::getline(commFile, processName);
    commFile.close();

    /* 5. Validate the retrieved process name */
    if (processName.empty()) {
        // Possibly an unexpected read error or empty name
        return ErrorCode::RetrievalFailed;
    }

    /*
     * 6. Ensure the caller's buffer is large enough.
     *    We must account for the null terminator, so if processName.length() == 5,
     *    we need at least 6 bytes in bufferSize.
     */
    if (processName.length() + 1 > bufferSize) {
        return ErrorCode::BufferTooSmall;
    }

    /* 7. Safely copy the short process name into the output buffer */
    std::strncpy(buffer, processName.c_str(), bufferSize - 1);
    buffer[bufferSize - 1] = '\0'; // Ensure null-termination

    return ErrorCode::Success;
}

/**********************************************************************************************************************
 *  FUNCTION: CreateProcessInteractionInstance
 *********************************************************************************************************************/
/*!
 * \brief  Factory function to create a Linux-specific ProcessInteraction instance.
 *
 * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
 */
auto CreateProcessInteractionInstance() -> std::unique_ptr<ara::os::interface::process::ProcessInteraction>
{
    return std::make_unique<ProcessInteractionImpl>();
}

} // namespace process
} // namespace linux
} // namespace os
} // namespace ara
