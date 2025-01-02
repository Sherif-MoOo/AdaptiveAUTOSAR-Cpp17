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

namespace ara {
namespace core {
namespace internal {

/*!
 * \brief   A constexpr function to strip directory paths from a string (like __FILE__).
 * \param   fullPath  A zero-terminated string, typically __FILE__.
 * \return  Pointer to the character immediately after the last slash or backslash.
 *
 * \details 
 *  - This handles both forward slash '/' and backslash '\\' to cover various platforms.
 *  - Example: 
 *       const char* result = CutLeadingPath("C:\\MyProject\\source\\file.cpp");
 *       // result now points to "file.cpp"
 */
constexpr auto CutLeadingPath(const char* fullPath) noexcept -> const char*
{
    const char* filename = fullPath;
    while (*fullPath != '\0') {
        if (*fullPath == '/' || *fullPath == '\\') {
            filename = fullPath + 1; 
        }
        ++fullPath;
    }
    return filename;
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
 */
#define ARA_CORE_INTERNAL_FILE \
    [] { \
        constexpr const char* stripped = ::ara::core::internal::CutLeadingPath(__FILE__); \
        return stripped; \
    }()

/*!
 * \brief   Provides a path-stripped version of __FILE__ plus line number at compile time.
 *
 * \details 
 *  - We combine __FILE__ ":" __LINE__ and then strip the path.
 *  - The final string might look like "file.cpp:123" if the path was "C:\xyz\file.cpp".
 */
#define ARA_CORE_INTERNAL_FILELINE \
    []() -> const char* { \
        static constexpr char path_and_line[] = __FILE__ ":" ARA_CORE_LOC_STRINGIFY(__LINE__); \
        return ::ara::core::internal::CutLeadingPath(path_and_line); \
    }()

#endif // ARA_CORE_INTERNAL_LOCATION_UTILS_H_
