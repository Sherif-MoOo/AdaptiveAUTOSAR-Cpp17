/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       main.cpp
 *  \brief      The entry file for the demo App...
 *
 *  \details    This application just a demo simple application for the adaptive autosar platform.
 *
 *  \note       Most likely your application shall have the same flow.
 ***********************************************************************************************************************/

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/

#include <iostream>                     // For std::cout and std::cerr
#include <csignal>                      // For signal mask
#include <algorithm>                    // For std::all_of
#include <optional>                     // For std::optional
#include <functional>                   // For std::reference_wrapper

#include "ara/core/array.h"             // For platform core Array class
#include "demo/manager/demo_manager.h"  // For manager class

namespace demo {
namespace sighandle {

/*!
 * \brief Block all signals except the SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGSEGV signals.
 */
static auto InitilizeSigHandlerMask() noexcept -> void {

    bool success{false};

    sigset_t signals{};

    /**
     * @brief Configures the signal mask for the process to ensure proper signal handling.
     *
     * This configuration must be applied as early as possible in the program's execution to guarantee that any newly
     * spawned thread inherits this signal mask and blocks signals accordingly. By doing so, we ensure that only the
     * dedicated signal handler thread is responsible for handling specific signals.
     *
     * @details
     * All signals are blocked except for the following critical ones: `SIGABRT`, `SIGBUS`, `SIGFPE`, `SIGILL`, and
     * `SIGSEGV`. Blocking these signals could lead to undefined behavior, as their default handling typically involves
     * terminating the process and generating a core dump. Preserving their default actions ensures that fatal errors are
     * handled appropriately, adhering to the underlying POSIX environment's standards.
     *
     * **Signal Mask Inheritance:**
     * - The configured signal mask will be inherited by all subsequent threads. This inheritance guarantees that the
     *   blocking configuration is consistent across all threads, preventing unintended signal interruptions outside the
     *   dedicated handler thread.
     *
     *
     * **Critical Signals Preserved:**
     * - The following signals are **not** blocked and are exclusively:
     *
     *   - **`SIGABRT` (Abort Signal):**
     *     - **Purpose:** Indicates that the process has aborted, typically invoked by the `abort()` function.
     *     - **Default Action:** Terminates the process and generates a core dump for debugging.
     *
     *   - **`SIGBUS` (Bus Error Signal):**
     *     - **Purpose:** Signifies an invalid memory access, such as misaligned memory access or accessing non-existent memory regions.
     *     - **Default Action:** Terminates the process and produces a core dump.
     *
     *   - **`SIGFPE` (Floating-Point Exception Signal):**
     *     - **Purpose:** Raised due to erroneous arithmetic operations, including division by zero, integer overflow, or invalid floating-point operations.
     *     - **Default Action:** Terminates the process and generates a core dump.
     *
     *   - **`SIGILL` (Illegal Instruction Signal):**
     *     - **Purpose:** Occurs when the process attempts to execute an invalid, undefined, or privileged machine instruction.
     *     - **Default Action:** Terminates the process and produces a core dump.
     *
     *   - **`SIGSEGV` (Segmentation Fault Signal):**
     *     - **Purpose:** Indicates an invalid memory reference, such as dereferencing a null or dangling pointer.
     *     - **Default Action:** Terminates the process and generates a core dump.
     *
     * @note
     * - **POSIX Compliance:** This setup adheres to POSIX standards, ensuring compatibility and predictable behavior across
     *   Unix-like operating systems.
     * - **Core Dumps:** Preserving the default actions for critical signals facilitates the generation of core dumps,
     *   which are essential for post-mortem debugging and diagnosing the root causes of fatal errors.
     */
    /*Fill signals var with all possible signals*/ 
    if(sigfillset(&signals) == 0) {

        constexpr const ara::core::Array<int,5> kCriticalSigs{SIGABRT, SIGBUS, SIGILL, SIGFPE, SIGSEGV};

        auto const dlt_sig_status = [&signals](int const& sig) noexcept -> bool {

            return (sigdelset(&signals, sig) == 0);
        }; 

        /*Check status of each deleted signal*/
        if(std::all_of(kCriticalSigs.cbegin(), kCriticalSigs.cend(), dlt_sig_status)) {

            /*Block all signals except the SIGABRT, SIGBUS, SIGFPE, SIGILL, SIGSEGV signals*/
            if(pthread_sigmask(SIG_SETMASK, &signals, nullptr) == 0) {

                success = true;
            }
        }

    }
    
    if(!success) {

        std::cerr << "[demo main][FATAL] Initialize signal handling failed." << std::endl;
        std::abort();
    }

}

} // namespace sighandle
} // namespace demo


int main() {

    /*Set main thread name for debugging*/
    pthread_setname_np(pthread_self(), "demo_main");

    demo::sighandle::InitilizeSigHandlerMask();

    // ara::core::Initialize

    std::cout << "[demo main][INFO] main thread started." << std::endl;

    std::uint8_t exit_code{EXIT_FAILURE};
    {
        std::optional<std::reference_wrapper<demo::manager::DemoManager>> managerOpt = demo::manager::DemoManager::StartManager();


        if (managerOpt.has_value()) {
            demo::manager::DemoManager& manager = managerOpt.value().get();

            exit_code = manager.RunManager();

            std::cout << "[demo main][INFO] Manager exited with code: " << static_cast<int>(exit_code) << std::endl;
        }
        else {
            std::cerr << "[demo main][ERROR] Failed to start DemoManager: Instance already created and exclusively owned." << std::endl;
        }
    }

    // ara::core::Deinitialize

    std::cout << "[demo main][INFO] main thread finished." << std::endl;

    return exit_code;
}