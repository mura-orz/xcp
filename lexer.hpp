// ===========================================================================
/// @file
/// @brief		Lexical parser of C++.
/// @author		Mura.
/// @copyright	(c) 2023-, Mura.
// ===========================================================================

#ifndef xxx_LEXER_HPP_
#define xxx_LEXER_HPP_

// ===========================================================================

#include "utility.hpp"

#include <string_view>
#include <unordered_set>
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <optional>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

// ===========================================================================

///	@brief	The root namespace of the xcp.
namespace xxx::xcp {
}

///	@brief	Lexiccal parser of the xcp.
namespace xxx::xcp::lex {
}

///	@brief	Definitions for lexiccal parser of the xcp.
namespace xxx::xcp::lex::def {
}

namespace xxx {

// ===========================================================================
namespace xcp::lex {
namespace def {

// literal list

///	@brief	Alternative tokens.
std::unordered_set<std::string_view> const alternative_expressions{"and", "and_eq", "bitand", "bitor", "compl", "not", "not_eq", "or", "or_eq", "xor", "xor_eq"};
///	@brief	Reserved keywords.
std::unordered_set<std::string_view> const keywords{
	// A
	"alignas",
	"alignof",
	"asm",
	"auto",
	// B
	"bool",
	"break",
	// C
	"case",
	"catch",
	"char",
	"char16_t",
	"char32_t",
	"class",
	"const",
	"constexpr",
	"consteval",
	"const_cast",
	"continue",
	// D
	"decltype",
	"default",
	"delete",
	"do",
	"double",
	"dynamic_cast",
	// E
	"else",
	"enum",
	"explicit",
	"export",
	"extern",
	// F
	"false",
	"float",
	"for",
	"friend",
	// G
	"goto",
	// H
	// I
	"if",
	"inline",
	"int",
	// J,K
	// L
	"long",
	// M
	"mutable",
	// N
	"namespace",
	"new",
	"noexcept",
	"nullptr",
	// O
	"operator",
	// P
	"private",
	"protected",
	"public",
	// Q
	// R
	"return",
	"reinterpret_cast",
	"register",
	// S
	"short",
	"signed",
	"sizeof",
	"static",
	"static_assert",
	"static_cast",
	"struct",
	"switch",
	// T
	"template",
	"this",
	"thread_local",
	"throw",
	"typedef",
	"typeid",
	"typename",
	"try",
	"true",
	// U
	"union",
	"unsigned",
	"using",
	// V
	"virtual",
	"void",
	"volatile",
	// W
	"wchar_t",
	"while",
	// X,Y,Z
};

// -----------------------------------
// regex rules

//	@brief	One or more new lines (LF+).
//	@note	VC++ does not support multiline.
// std::regex const newline_re{R"(^(?:\r?\n)+.*)", std::regex_constants::multiline};
//	@brief	Multiple-lines comment (/*...*/).
//	@note	VC++ does not support multiline.
// std::regex const block_comment_re{R"(^(/[*](?:[^*]|[*][^/]|^|$|[\r\n])*[*]/).*)", std::regex_constants::multiline};
///	@brief	Escaped new line (@\LF).
std::regex const escaped_newline_re{R"(\\\r?\n)"};
///	@brief	Single-line comment (//... LF+).
std::regex const line_comment_re{R"(^(//[^\r\n]*).*)"};
///	@brief	Inline whitespaces (WS+).
std::regex const inline_whitespaces_re{R"(^([ \t\v\f]+).*)"};
///	@brief	Identifier ([A-Za-z_][A-Za-z0-9_]*).
std::regex const identifier_re{R"(^([a-zA-Z_]\w*).*)"};
///	@brief	Preprocessing number (pp-number LF*).
std::regex const pp_number_re{R"(^([.]?\d+(?:'\w*|[eEpP][-+]|[.])?).*)"};
///	@brief	String literal (u8|[UL])?"..."ID*).
std::regex const string_literal_re{R"(^((?:u8?|[UL])?"(?:[^'\r\n]|\\['"?\abfnrtv]|\\u[0-9a-fA-f]{4}|\\U[0-9a-fA-f]{8})*"\w*).*)"};
///	@brief	Character literal (u8|[UL])?'...'ID*).
std::regex const character_literal_re{R"(^((?:u8?|[UL])?'(?:[^'\r\n]|\\['"?\abfnrtv]|\\u[0-9a-fA-f]{4}|\\U[0-9a-fA-f]{8})+'\w*).*)"};
///	@brief	Header name (@<path@>).
std::regex const header_name_re{R"(^(<[^>\r\n]+>).*)"}; // "" is also matched by string_literal_re

///	@brief	Preprocessing operators and punctuators.
std::regex const preprocessing_op_or_punc_re{
	"^("
	R"([.]{3}|(?:%:){1,2})"				   // ... %:%: %:
	R"(|\[|\]|(?:[.]|->)[*]?)"			   // [ ] . .* -> ->*
	R"(|&&?|-[-=]?|\+[+=]?|##?|::?|\|\|?)" // && || -- ++ ## :: & | - + # : += -=
	R"(|<[:%]|[:%]>)"					   // <: <% :> %>
	R"(|>>?=?|<<?=?)"					   // >>= <<= >> << >= <= > <
	R"(|[*/%^&|~!=]=?)"					   // *= /= %= ^= &= |= ~= != == * / % ^ & | ~ ! =
	R"(|[{}();?,])"						   // { } ( ) ; ? ,
	").*"};								   // ( new delete and and_eq bitand bitor compl not not_eq or or_eq xor xor_eq )

///	@brief	Basic source character set.
std::string_view const basic_source_character_set{"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789_{}[]#()<>%:;.?*+-/^&|~!=,\\\"’ \t\v\f\r\n"};

} // namespace def

// ===========================================================================

/// @brief	Gets whether the specified character is a basic source character or not.
/// @param[in] ch	 The character.
/// @return		Whether the @p ch is a basic source character or not.
constexpr inline bool
is_basic_source_character_set(char32_t ch) noexcept {
	return uc::is_head1(ch) && def::basic_source_character_set.find(static_cast<char>(ch)) != std::string_view::npos;
}

///	@brief	Escapes the non basic source character set.
///	@param[in] str	 The string.
///	@return		The escaped string.
inline std::string
escape_non_basic_source_character_set(std::u32string_view const& str) {
	auto escaped = str
				 | std::views::transform([](auto const& ch) {
					   if (is_basic_source_character_set(ch)) {
						   return std::string(1u, ch);
					   } else {
						   std::ostringstream oss;
						   oss << R"(\U)" << std::hex << std::uppercase << std::setw(8) << std::setfill('0') << static_cast<std::uint32_t>(ch);
						   return oss.str();
					   }
				   })
				 | std::views::join
				 | std::views::common;
	return std::accumulate(escaped.begin(), escaped.end(), std::move(std::ostringstream{}), [](auto&& oss, auto const& s) { oss << s; return std::move(oss); }).str();
}

///	@brief	Whitespaces.
class whitespace_t {
public:
	///	@brief	Gets number of newlines.
	///	@return	Number of newlines.
	auto newlines() const noexcept { return newlines_; }

	///	@brief	Gets a new whitespacs with modified number of newlines.
	///	@param[in]	newlines	Delta of newlines number.
	///	@return		New whitespaces.
	[[nodiscard]] auto appent(std::size_t newlines) const noexcept { return whitespace_t{newlines_ + newlines}; }

	///	@brief	Constructor.
	whitespace_t() noexcept : newlines_{} {}
	///	@overload
	///	@param[in] newlines	Number of newlines.
	explicit whitespace_t(std::size_t newlines) noexcept : newlines_{newlines} {}

private:
	std::size_t newlines_; ///< @brief	Number of newlines.
};

///	@brief	Preprocessing token type.
enum class preprocessing_token_type_t {
	unknown,				  ///< @brief	Unknown as default.
	string_literal,			  ///< @brief	String literal.
	character_literal,		  ///< @brief	Character literal.
	header_name,			  ///< @brief	Header name.
	pp_number,				  ///< @brief	Preprocessing number.
	identifier,				  ///< @brief	Identifier.
	preprocessing_op_or_punc, ///< @brief	Preprocessing operator or punctuator.
};

///	@brief	Preprocessing token.
class preprocessing_token_t {
public:
	///	@brief	Gets token string.
	///	@return	Token string.
	auto const& str() const noexcept { return str_; }
	///	@brief	Gets token type.
	///	@return	Token type.
	auto type() const noexcept { return type_; }

	/// @brief	Constructor.
	preprocessing_token_t() noexcept : type_{}, str_{} {}
	///	@overload
	/// @param[in]	str		Token string.
	/// @param[in]	type	Token type.
	explicit preprocessing_token_t(std::string_view const& str, preprocessing_token_type_t type = preprocessing_token_type_t{}) noexcept : type_{type}, str_{str} {}

private:
	preprocessing_token_type_t type_; ///< @brief	Token type.
	std::string				   str_;  ///< @brief	Token string.
};

///	@brief	Token, which is either whitespace or preprocessing token.
using token_t = std::variant<whitespace_t, preprocessing_token_t>;
///	@brief	Collection of tokens.
using tokens_t = std::vector<token_t>;

///	@brief	Parses newlines.
///	@param[in]	str		String.
///	@return	Parsed newlines.
///		The first of tuple is remaining string.
///		The second of tuple is number of new lines.
inline std::tuple<std::string_view, std::size_t>
parse_newlines(std::string_view const& str) {
	if (auto const pos = str.find_first_not_of("\r\n"); pos == std::string_view::npos) {
		return {std::string_view{}, 0u};
	} else if (str.find("\r\r") != std::string_view::npos || str.at(pos - 1) == '\r') {
		throw std::invalid_argument("unsupported single carridge return");
	} else {
		return {str.substr(pos), std::ranges::count(str, '\n')};
	}
}

///	@brief	Parses block comments.
///	@param[in]	str		String.
///	@return	Parsed newlines.
///		The first of tuple is remaining string.
///		The second of tuple is number of new lines.
inline std::tuple<std::string_view, std::size_t>
parse_block_comments(std::string_view const& str) {
	if (! str.starts_with("/*")) {
		return {str, 0u};
	} else {
		auto const s   = str.substr(2);
		auto const end = s.find_first_of("*/");
		return {s, end};
	}
}

///	@brief	Parses whitespaces.
///	@param[in]	str		String.
///	@return	Parsed whitespaces.
///		The first of tuple is remaining string.
///		The second of tuple is parsed whitespace if exists.
inline std::tuple<std::string_view, std::optional<whitespace_t>>
parse_whitespaces(std::string_view const& str) {
	log::logger_t logger;
	log::tracer_t tracer{logger, util::escape(str)};

	std::size_t newlines{};
	svmatch		r;
	if (auto const [s1, lf1] = parse_newlines(str); 0u < lf1) {
		tracer.trace(std::to_string(__LINE__) + " {newlines:" + std::to_string(lf1));
		return {s1, whitespace_t{lf1}};
	} else if (auto const [s2, lf2] = parse_block_comments(str); 0u < lf2) {
		tracer.trace(std::to_string(__LINE__) + " {block comments:" + std::to_string(lf2));
		return {s2, whitespace_t{lf2}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::line_comment_re)) {
		newlines = std::ranges::count(r.str(1), '\n');
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1) + " " + std::to_string(newlines));
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::inline_whitespaces_re)) {
		// nothing to do
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1) + " " + std::to_string(newlines));
	} else {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1) + " " + std::to_string(newlines));
		tracer.set_result(0);
		return {str, std::nullopt};
	}
	auto const sss = str.substr(r.str(1).size());

	tracer.set_result(std::to_string(newlines) + ":" + util::escape(sss));
	return {sss, whitespace_t{newlines}};
}

///	@brief	Parses preprocessing token.
///	@param[in]	str		String.
///	@return	Parsed preprocessing token.
///		The first of tuple is remaining string.
///		The second of tuple is parsed tokens if exists.
inline std::tuple<std::string_view, token_t>
parse_preprocessing_token(std::string_view const& str) {
	log::logger_t logger;
	log::tracer_t tracer{logger, util::escape(str)};

	if (str.empty()) throw std::invalid_argument("empty string");

	// parses preprocessing tokens
	if (svmatch r; std::regex_match(str.cbegin(), str.cend(), r, def::string_literal_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::string_literal}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::character_literal_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::character_literal}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::header_name_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::header_name}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::pp_number_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::pp_number}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::identifier_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::identifier}};
	} else if (std::regex_match(str.cbegin(), str.cend(), r, def::preprocessing_op_or_punc_re)) {
		tracer.trace(std::to_string(__LINE__) + " " + r.str(1));
		return {str.substr(r.str(1).size()), preprocessing_token_t{r.str(1), preprocessing_token_type_t::preprocessing_op_or_punc}};
	} else {
		throw std::invalid_argument("invalid preprocessing token:" + util::escape(str));
	}
}

///	@brief	Scans file.
///		- phase 1: replaces all the non basic source character set to universal character set.
/// 	- phase 2: converts physical source to logical source.
/// 	- phase 3: parses preprocessing tokens and replaces comments exespting new line.
/// 	- phase 4: preprocessing directies are executed.
///	@param[in]	path	Path.
///	@return		Parsed tokens.
inline tokens_t
scan(std::filesystem::path const& path) {
	log::logger_t logger;
	log::tracer_t tracer{logger, path.string()};

	// -----------------------------------
	// Loads file and strip BOM if exists.
	auto const physical_source = util::load_file(path);
	if (physical_source.empty()) {
		tracer.set_result(0);
		return {};
	}
	// -----------------------------------
	// All the charcaters are converted to 32bit string once.
	auto const chars = uc::to_32string(physical_source);
	if (chars.empty()) {
		auto const msg = __func__ + std::to_string(__LINE__);
		tracer.set_result(msg);
		throw std::invalid_argument(msg);
	}
	// replaces all the non basic source character set to universal character set.
	auto const phase1 = escape_non_basic_source_character_set(chars);
	if (phase1.empty()) {
		auto const msg = __func__ + std::to_string(__LINE__);
		tracer.set_result(msg);
		throw std::invalid_argument(msg);
	}

	// -----------------------------------
	// converts physical source to logical source.
	auto const phase2 = std::regex_replace(phase1, def::escaped_newline_re, "");
	if (phase2.empty()) {
		auto const msg = __func__ + std::to_string(__LINE__);
		tracer.set_result(msg);
		throw std::invalid_argument(msg);
	}
	// Any source file shall be terminated by a newline character.
	auto const terminated = phase2.back() == '\n' ? phase2 : phase2 + '\n'; // Extra line would be added if the last LF is escaped.

	// -----------------------------------
	// parses preprocessing tokens
	tokens_t phase3;
	for (auto s = terminated; ! s.empty();) {
		// -----------------------------------
		// replaces whilespaces, which just keep count of new lines.
		if (auto const [rest, ws] = parse_whitespaces(s); ws) {
			if (0u < ws->newlines()) {
				if (phase3.empty() || ! std::holds_alternative<whitespace_t>(phase3.back())) {
					phase3.emplace_back(*ws);
					tracer.trace(std::to_string(__LINE__) + " " + std::to_string(ws->newlines()));
				} else {
					auto const sum = std::get<whitespace_t>(phase3.back()).appent(ws->newlines());
					phase3.back()  = sum;
					tracer.trace(std::to_string(__LINE__) + " " + std::to_string(sum.newlines()));
				}
			}
			s = rest;
			continue;
		}

		// -----------------------------------
		// parses preprocessing tokens.
		auto const [rest, token] = parse_preprocessing_token(s);
		phase3.emplace_back(token);
		s = rest;
	}
	if (phase3.empty()) {
		auto const msg = __func__ + std::to_string(__LINE__);
		tracer.set_result(msg);
		throw std::invalid_argument(msg);
	}

	tracer.set_result(phase3.size());
	return phase3;
}

} // namespace xcp::lex
} // namespace xxx

#endif // xxx_LEXER_HPP_
