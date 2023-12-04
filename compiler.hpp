// ===========================================================================
/// @file
/// @brief      Parser of C++.
/// @author     Mura.
/// @copyright  (c) 2023-, Mura.
// ===========================================================================

#ifndef xxx_COMPILER_HPP_
#define xxx_COMPILER_HPP_

// ================================================================================

#include "preprocessor.hpp"

#include <filesystem>

// ================================================================================
namespace xxx::xcp::tu {

/// @brief	Translation unit.
class translation_unit_t {
public:
	/// @brief	Get the path.
	/// @return	The path.
	auto const& path() const noexcept { return path_; }
	/// @brief	Get the tokens.
	/// @return	The tokens.
	auto const& tokens() const noexcept { return tokens_; }
	/// @brief	Get the context.
	/// @return	The context.
	auto const& context() const noexcept { return context_; }

	/// @brief	Constructor.
	/// @param[in]	path	Path of the file.
	/// @param[in]	tokens	Tokens of the file.
	/// @param[in]	context	Context of the file.
	translation_unit_t(std::filesystem::path const& path, lex::tokens_t const& tokens, cpp::context_t const& context) : path_{path}, tokens_{tokens}, context_{context} {}

private:
	std::filesystem::path path_;	///< Path of the file.
	lex::tokens_t		  tokens_;	///< Tokens of the file.
	cpp::context_t		  context_; ///< Context of the file.
};

} // namespace xxx::xcp::tu

#endif // xxx_COMPILER_HPP_
