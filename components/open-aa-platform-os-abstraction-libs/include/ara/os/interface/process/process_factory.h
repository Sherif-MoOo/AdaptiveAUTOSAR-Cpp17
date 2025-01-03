/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/interface/process/process_factory.h
 *  \brief      Declaration of the ara::os::interface::process::ProcessFactory.
 *
 *  \details    This file declares the ara::os::interface::process::ProcessFactory class responsible for creating
 *              platform-specific instances of the ProcessInteraction interface.
 *
 *  \note       This facilitates the OS abstraction by hiding platform-specific details from the client.
 ***********************************************************************************************************************/

#ifndef OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_FACTORY_H_
#define OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_FACTORY_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
/*!
 * \brief  Includes the ProcessInteraction interface header.
 */
#include "ara/os/interface/process/process_interaction.h"

#include <memory> // For std::unique_ptr

namespace ara {
namespace os {
namespace interface {
namespace process {

/**********************************************************************************************************************
 *  CLASS: ProcessFactory
 *********************************************************************************************************************/
/*!
 * \brief  Factory class for creating ara::os::interface::process::ProcessInteraction instances.
 *
 * \details
 * - Determines the target platform at compile-time and instantiates the corresponding ProcessInteraction implementation.
 * - Ensures scalability by supporting multiple platforms.
 * - Thread-safe and stateless, allowing concurrent access in multi-threaded environments.
 */
class ProcessFactory {
public:
    /*!
     * \brief  Creates a platform-specific ProcessInteraction instance.
     *
     * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
     *         Compilation fails if the platform is unsupported.
     *
     * \note   This method is thread-safe and can be called concurrently from multiple threads.
     */
    static auto CreateInstance() noexcept -> std::unique_ptr<ProcessInteraction>;
};

} // namespace process
} // namespace interface
} // namespace os
} // namespace ara

#endif // OPEN_AA_ADAPTIVE_AUTOSAR_LIBS_INCLUDE_ARA_OS_INTERFACE_PROCESS_PROCESS_FACTORY_H_
