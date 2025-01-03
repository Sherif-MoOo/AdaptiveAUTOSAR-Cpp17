/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       ara/os/qnx/process/process.h
 *  \brief      QNX-specific implementation of the ara::os::interface::process::ProcessInteraction interface.
 *
 *  \details    Declares the QNX-specific ProcessInteraction implementation and the factory function.
 *
 *  \note       This class ensures that process-related functionalities are implemented using QNX system calls.
 ***********************************************************************************************************************/

#ifndef ARA_OS_QNX_PROCESS_PROCESS_H
#define ARA_OS_QNX_PROCESS_PROCESS_H

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
namespace qnx {
namespace process {

/**********************************************************************************************************************
 *  CLASS: ProcessInteractionImpl
 *********************************************************************************************************************/
/*!
 * \brief  QNX-specific implementation of the ara::os::interface::process::ProcessInteraction interface.
 *
 * \details
 * - Utilizes QNX-specific system calls to retrieve the process name.
 * - Ensures that operations are secure and handle errors appropriately.
 */
class ProcessInteractionImpl final : public ara::os::interface::process::ProcessInteraction {
public:
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
    auto GetProcessName(char* buffer, std::size_t bufferSize) const noexcept -> ara::os::interface::process::ErrorCode override;

    /*!
     * \brief  Destructor for ProcessInteractionImpl.
     */
    ~ProcessInteractionImpl() override = default;
};

/**********************************************************************************************************************
 *  FUNCTION: CreateProcessInteractionInstance
 *********************************************************************************************************************/
/*!
 * \brief  Factory function to create a QNX-specific ProcessInteraction instance.
 *
 * \return A std::unique_ptr to an ara::os::interface::process::ProcessInteraction object.
 */
std::unique_ptr<ara::os::interface::process::ProcessInteraction> CreateProcessInteractionInstance();

} // namespace process
} // namespace qnx
} // namespace os
} // namespace ara

#endif // ARA_OS_QNX_PROCESS_PROCESS_H
