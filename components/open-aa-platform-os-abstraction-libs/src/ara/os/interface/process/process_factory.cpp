/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/interface/process/process_factory.cpp
 *  \brief      Implementation of the ara::os::interface::process::ProcessFactory.
 *
 *  \details    Provides a method to create platform-specific instances of the ProcessInteraction interface.
 *
 *  \note       This facilitates the OS abstraction by hiding platform-specific details from the client.
 ***********************************************************************************************************************/

#include "ara/os/interface/process/process_factory.h"

// Include platform-specific headers for ProcessInteraction implementations
#if defined(__linux__)
    #include "ara/os/linux/process/process.h" // Linux-specific ProcessInteraction implementation
#elif defined(__QNXNTO__)
    #include "ara/os/qnx/process/process.h"   // QNX-specific ProcessInteraction implementation
#else
    /* Unsupported platform: Generate a compile-time error */
    #error "Unsupported platform. The ProcessFactory cannot create a ProcessInteraction instance."
#endif

namespace ara {
namespace os {
namespace interface {
namespace process {

/**********************************************************************************************************************
 *  FUNCTION: ProcessFactory::CreateInstance
 *********************************************************************************************************************/
/*!
 * \brief  Factory method to create a platform-specific ProcessInteraction instance.
 *
 * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
 *         Compilation fails if the platform is unsupported.
 */
auto ProcessFactory::CreateInstance() noexcept -> std::unique_ptr<ProcessInteraction> {
#if defined(__linux__)
    /* Create and return a Linux-specific ProcessInteraction instance */
    return linux::process::CreateProcessInteractionInstance();
#elif defined(__QNXNTO__)
    /* Create and return a QNX-specific ProcessInteraction instance */
    return qnx::process::CreateProcessInteractionInstance();
#else
    /* Unsupported platform: This should never be reached due to the #error directive above */
    return nullptr;
#endif
}

} // namespace process
} // namespace interface
} // namespace os
} // namespace ara
