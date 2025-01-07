/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project (CXX_STANDARD 17)
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       demo/manager/demo_manager.h
 *  \brief      Definition of the demo::manager::DemoManager class.
 *
 *  \details    This file declares the demo::manager::DemoManager class, which provides controlled access to a unique
 *              instance without any heap overhead. It ensures that only one instance exists and is accessible only
 *              through the StartManager method by the one who created it.
 *
 *  \note       ara::core::Result to wrap the instance would be a better approach.
 *********************************************************************************************************************/

#ifndef DEMO_MANAGER_DEMOMANAGER_H_
#define DEMO_MANAGER_DEMOMANAGER_H_

/**********************************************************************************************************************
 *  INCLUDES
 *********************************************************************************************************************/
#include <atomic>                           // For the std::atomic_bool
#include <condition_variable>               // For the std::conditional_variable
#include <thread>                           // For the std::thread
#include <mutex>                            // For std::mutex
#include <optional>                         // For std::optional
#include <functional>                       // For std::reference_wrapper

namespace demo {
namespace manager {

/** -------------------------------------------------------------------------------------------------------------------
 *  CLASS DemoManager
 *  -------------------------------------------------------------------------------------------------------------------
 *  \brief  Singleton-like class managing the demo operations with controlled access.
 *
 *  The DemoManager class provides controlled access to a unique instance without any heap overhead. It ensures that
 *  only one instance exists and is accessible only through the StartManager method by the creator.
 */
class DemoManager final {
public:
    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Retrieves the unique instance of DemoManager.
     *
     *  This static method initializes the instance upon the first call in a thread-safe manner and returns an optional
     *  reference to it. Only the first caller can successfully obtain the instance. Subsequent calls will return
     *  an empty optional.
     *
     *  @return     std::optional<std::reference_wrapper<DemoManager>> Optional containing the instance reference or
     *              an empty optional if the instance has already been created.
     */
    static auto StartManager() noexcept -> std::optional<std::reference_wrapper<DemoManager>>;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Runs the manager and returns an exit code.
     *
     *  Executes the primary functionality of the manager and returns a success exit code.
     *
     *  @return     std::uint8_t Exit code indicating success.
     */
    auto RunManager() noexcept -> std::uint8_t;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Deleted copy constructor.
     *
     *  Prevents copying of the DemoManager instance.
     */
    constexpr DemoManager(const DemoManager&) noexcept = delete;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Deleted move constructor.
     *
     *  Prevents moving of the DemoManager instance.
     */
    constexpr DemoManager(DemoManager&&) noexcept = delete;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Deleted copy assignment operator.
     *
     *  Prevents assignment from another DemoManager instance.
     */
    constexpr auto  operator=(const DemoManager&) noexcept ->  DemoManager& = delete;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Deleted move assignment operator.
     *
     *  Prevents move assignment from another DemoManager instance.
     */
    constexpr auto operator=(DemoManager&&) noexcept -> DemoManager& = delete;

private:
    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Private constructor.
     *
     *  Ensures that instances can only be created within the class itself, specifically through the StartManager method.
     */
    DemoManager() noexcept;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Private Destructor.
     *
     *  marked noexcept to ensure no exceptions are thrown during destruction.
     */
    ~DemoManager() noexcept;


    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Private InitializeDemoManager.
     *
     *  Intialization logic of the manager.
     */
    auto InitializeDemoManager() noexcept -> void;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Private TerminateDemoManager.
     *
     *  Terminatation logic of the manager.
     */
    auto TerminateDemoManager() noexcept -> void;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Private GracefulShutdownHandler.
     *
     *  To handle the shutdown request and wait for the specific signals for it.
     */
    auto GracefulShutdownHandler() noexcept -> void;
    
    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Flag indicating whether an instance has been created.
     *
     *  Static flag to ensure that only one instance is created. Protected by a mutex for thread safety.
     */
    static bool instanceCreated_;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      Mutex for thread-safe access to instance creation.
     */
    static std::mutex mutex_;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      thread to wait for either SIGTERM, OR SIGINT.
     */
    std::thread graceful_shutdown_handler_thread_;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      condition variable to notify wating threads.
     */
    std::condition_variable shutdown_notifier_;

    /** -------------------------------------------------------------------------------------------------------------------
     *  @brief      flag of Application turn off request.
     */
    std::atomic_bool turn_off_requested_;
};

} // namespace manager
} // namespace demo

#endif // DEMO_MANAGER_DEMOMANAGER_H_
