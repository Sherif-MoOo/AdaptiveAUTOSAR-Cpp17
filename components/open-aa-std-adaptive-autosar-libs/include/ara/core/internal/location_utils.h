/**********************************************************************************************************************
 *  PROJECT
 *  -------------------------------------------------------------------------------------------------------------------
 *  \verbatim
 *  OpenAA: Open Source Adaptive AUTOSAR Project
 *  \endverbatim
 *  -------------------------------------------------------------------------------------------------------------------
 *  FILE DESCRIPTION
 *  -------------------------------------------------------------------------------------------------------------------
 *  \file       location_utils.h
 *  \brief      Internal utilities for compile-time extraction of filename/line information.
 *
 *  \details    This file defines helper functions and macros for path-stripping and location capturing. They are used
 *              internally within the Adaptive AUTOSAR environment to support consistent violation logging, etc.
 *
 *********************************************************************************************************************/

#ifndef ARA_CORE_INTERNAL_LOCATION_UTILS_H_
#define ARA_CORE_INTERNAL_LOCATION_UTILS_H_

#include <string_view>  // Required for std::string_view

namespace ara {
namespace core {
namespace internal {

/*!
 * \brief   A constexpr function to strip directory paths from a string (like __FILE__).
 * \param   fullPath  A std::string_view representing the full path, typically __FILE__.
 * \return  A std::string_view pointing to the filename part after the last slash or backslash.
 *
 * \details 
 *  - This handles both forward slash '/' and backslash '\\' to cover various platforms.
 *  - Example: 
 *       std::string_view result = CutLeadingPath("C:\\MyProject\\source\\file.cpp");
 *       // result now points to "file.cpp"
 */
constexpr auto CutLeadingPath(std::string_view fullPath) noexcept -> std::string_view
{
    std::size_t pos = fullPath.find_last_of("/\\");
    if (pos == std::string_view::npos) {
        return fullPath;
    } else {
        return fullPath.substr(pos + 1);
    }
}

}  // namespace internal
}  // namespace core
}  // namespace ara

/*!
 * \brief   Macro to stringify tokens.
 */
#define ARA_CORE_LOC_STRINGIFY_(x) #x
#define ARA_CORE_LOC_STRINGIFY(x)  ARA_CORE_LOC_STRINGIFY_(x)

/*!
 * \brief   Provides a path-stripped version of __FILE__ at compile time using lambda capture.
 *
 * \return  A `std::string_view` pointing to the stripped filename.
 */
#define ARA_CORE_INTERNAL_FILE \
    []() -> std::string_view { \
        constexpr std::string_view stripped = ::ara::core::internal::CutLeadingPath(__FILE__); \
        return stripped; \
    }()

/*!
 * \brief   Provides a path-stripped version of __FILE__ plus line number at compile time.
 *
 * \details 
 *  - Combines __FILE__ ":" __LINE__ and then strips the path.
 *  - The final string looks like "file.cpp:123" if the path was "C:\xyz\file.cpp".
 *
 * \return  A `std::string_view` containing the stripped filename and line number.
 */
#define ARA_CORE_INTERNAL_FILELINE \
    []() -> std::string_view { \
        static constexpr char path_and_line[] = __FILE__ ":" ARA_CORE_LOC_STRINGIFY(__LINE__); \
        static constexpr std::string_view sv = ::ara::core::internal::CutLeadingPath(path_and_line); \
        return sv; \
    }()

#endif // ARA_CORE_INTERNAL_LOCATION_UTILS_H_
