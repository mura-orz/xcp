// ===========================================================================
/// @file
/// @brief		Preprocessor of C++.
/// @author		Mura.
/// @copyright	(c) 2023-, Mura.
// ===========================================================================

#ifndef xxx_PREPROCESSOR_HPP_
#define xxx_PREPROCESSOR_HPP_

// ================================================================================

#include "lexer.hpp"

#include <string_view>
#include <unordered_map>
#include <algorithm>
#include <ranges>
#include <regex>
#include <sstream>
#include <stack>
#include <string>
#include <tuple>
#include <variant>
#include <vector>

// ================================================================================
namespace xxx::opt {

using options_t = std::unordered_map<std::string, std::vector<std::string>>;

inline options_t
parse_options(std::vector<std::string_view> const& options) {
	auto o = options | std::views::filter([](auto const& option) { return option != "-" && option.starts_with("-"); }) | std::views::common;
	return std::accumulate(o.begin(), o.end(), options_t{}, [](auto&& options, auto const& option) {
		std::regex const option_re{R"(--?([A-Za-z]+)(?:=(.*))?)"};
		if (svmatch result; std::regex_match(option.cbegin(), option.cend(), result, option_re)) {
			options[result.str(1)].push_back((1u < result.size()) ? result.str(2) : std::string{});
		} else {
			throw std::invalid_argument(option.data());
		}
		return std::move(options);
	});
}

} // namespace xxx::opt

// ================================================================================
namespace xxx::xcp::cpp {

class context_t {
public:
	using macros_t = std::unordered_map<std::string, std::pair<lex::preprocessing_token_t, lex::preprocessing_token_t>>;
	using file_t   = std::pair<std::filesystem::path, lex::tokens_t>;

	explicit context_t(opt::options_t const& options) : options_{options}, sources_{}, conditions_{}, includes_{}, macros_{} {
		macros_["__cplusplus"] = std::make_pair(lex::preprocessing_token_t{}, lex::preprocessing_token_t{"201103L"});
		macros_["_XCP_VER"]	   = std::make_pair(lex::preprocessing_token_t{}, lex::preprocessing_token_t{"0x00020000"});
	}
	context_t() : context_t(opt::options_t{}) {}
	auto&		conditions() noexcept { return conditions_; }
	auto const& conditions() const noexcept { return conditions_; }
	auto&		macros() noexcept { return macros_; }
	auto const& macros() const noexcept { return macros_; }

private:
	// TODO: tokens, current, consumed
	opt::options_t					   options_;
	std::stack<file_t>				   sources_;
	std::stack<bool>				   conditions_;
	std::vector<std::filesystem::path> includes_;
	macros_t						   macros_;
}; // namespace xcp::cpp

inline auto
next_preprocessing_token(lex::tokens_t const& tokens, lex::tokens_t::const_iterator const& current) {
	if (current == tokens.cend()) return tokens.cend();
	auto itr = current + 1;
	for (auto const end = tokens.cend(); itr != end; ++itr) {
		if (std::holds_alternative<lex::preprocessing_token_t>(*itr)) break;
	}
	return itr;
}
inline auto
next_newline(lex::tokens_t const& tokens, lex::tokens_t::const_iterator const& current) {
	if (current == tokens.cend()) return tokens.cend();
	auto itr = current + 1;
	for (auto const end = tokens.cend(); itr != end; ++itr) {
		if (auto const ws = std::get_if<lex::whitespace_t>(&*itr); ws && 0u < ws->newlines()) {
			break;
		}
	}
	return itr;
}

inline std::optional<bool>
evaluate(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};
	return true; // TODO: not implemented yet.
}

inline std::tuple<bool, lex::tokens_t>
parse_define_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "define") return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::tuple<bool, lex::tokens_t>
parse_undef_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "undef") return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::tuple<bool, lex::tokens_t>
parse_error_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "error") return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::tuple<bool, lex::tokens_t>
parse_line_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "line") return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::tuple<bool, lex::tokens_t>
parse_include_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "include") return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::tuple<bool, lex::tokens_t>
parse_pragma_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return {false, tokens};

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || (std::get<lex::preprocessing_token_t>(*directive).str() != "pragma" && std::get<lex::preprocessing_token_t>(*directive).str() != "_Pragma")) return {false, tokens};

	// TODO:

	auto const newline = next_newline(tokens, directive);
	if (newline == tokens.cend() || std::get<lex::whitespace_t>(*newline).newlines() == 0u) return {false, tokens};

	tracer.set_result("true");
	return {true, lex::tokens_t(newline + 1, tokens.cend())};
}
inline std::optional<std::tuple<bool, lex::whitespace_t>>
parse_ifdef_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return std::nullopt;

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || (std::get<lex::preprocessing_token_t>(*directive).str() != "ifdef" && std::get<lex::preprocessing_token_t>(*directive).str() != "ifndef")) return std::nullopt;
	auto const affamative = std::get<lex::preprocessing_token_t>(*directive).str() != "ifdef";

	auto const macro_itr = next_preprocessing_token(tokens, directive);
	if (macro_itr == tokens.cend()) return std::nullopt;

	auto const macro   = std::get<lex::preprocessing_token_t>(*macro_itr);
	auto const defined = context.macros().contains(macro.str());

	auto const newline_itr = next_newline(tokens, macro_itr);
	if (newline_itr == tokens.cend()) return std::nullopt;

	auto const newline = std::get<lex::whitespace_t>(*newline_itr);

	auto const result = affamative == defined;

	tracer.set_result(result);
	return std::make_tuple(result, newline);
}
inline std::optional<std::tuple<bool, lex::whitespace_t>>
parse_if_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return std::nullopt;

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "if") return std::nullopt;

	auto const begin	= next_preprocessing_token(tokens, directive);
	auto const end_ritr = tokens.crbegin() + 1; // It is always valid because at least above tokens eixst.
	auto const end		= end_ritr.base();
	if (begin == tokens.cend()) return std::nullopt;

	auto const newline = std::get<lex::whitespace_t>(*end);

	// TODO: apply macros between begin and end.
	auto const result = evaluate(lex::tokens_t(begin, end), context);
	if (! result) return std::nullopt;
	tracer.set_result(*result);
	return std::make_tuple(*result, newline);
}
inline std::optional<std::tuple<bool, lex::whitespace_t>>
parse_elif_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return std::nullopt;

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "elif") return std::nullopt;
	if (context.conditions().empty()) return std::nullopt;

	auto const begin	= next_preprocessing_token(tokens, directive);
	auto const end_ritr = tokens.crbegin() + 1; // It is always valid because at least above tokens eixst.
	auto const end		= end_ritr.base();
	if (begin == tokens.cend()) return std::nullopt;

	auto const newline = std::get<lex::whitespace_t>(*end);

	// TODO: apply macros between begin and end.
	auto const result = evaluate(lex::tokens_t(begin, end), context);
	if (! result) return std::nullopt;
	tracer.set_result(*result);
	return std::make_tuple(*result, newline);
}
inline std::optional<lex::whitespace_t>
parse_else_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return std::nullopt;

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "else") return std::nullopt;

	auto const newline_itr = next_newline(tokens, directive);
	if (newline_itr == tokens.cend()) return std::nullopt;
	auto const newline = std::get<lex::whitespace_t>(*newline_itr);

	tracer.set_result(newline.newlines());
	return newline;
}
inline std::optional<lex::whitespace_t>
parse_endif_directive(lex::tokens_t const& tokens, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const pound = next_preprocessing_token(tokens, tokens.cbegin());
	if (pound == tokens.cend() || std::get<lex::preprocessing_token_t>(*pound).str() != "#") return std::nullopt;

	auto const directive = next_preprocessing_token(tokens, pound);
	if (directive == tokens.cend() || std::get<lex::preprocessing_token_t>(*directive).str() != "endif") return std::nullopt;

	auto const newline_itr = next_newline(tokens, directive);
	if (newline_itr == tokens.cend()) return std::nullopt;
	auto const newline = std::get<lex::whitespace_t>(*newline_itr);

	tracer.set_result(newline.newlines());
	return newline;
}

using line_t  = lex::tokens_t;
using lines_t = std::vector<line_t>;
inline lines_t
split_lines(lex::tokens_t const& tokens) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	auto const lines = std::accumulate(tokens.cbegin(), tokens.cend(), lines_t{{}}, [](auto&& lines, auto const& a) {
		auto const newline = std::holds_alternative<lex::whitespace_t>(a) ? std::get<lex::whitespace_t>(a).newlines() : 0u;
		lines.back().emplace_back(std::move(a));
		if (0u < newline) {
			lines.emplace_back(std::move(lex::tokens_t{}));
		}
		return std::move(lines);
	});
	tracer.set_result(lines.size());
	return lines;
}

inline std::tuple<lines_t, std::optional<context_t>>
parse_line(lines_t const& lines, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger};

	if (lines.empty()) return {};

	auto const& line			= lines.at(0);
	auto const	ifdef_directive = parse_ifdef_directive(line, context);
	auto const	if_directive	= parse_if_directive(line, context);
	auto const	elif_directive	= parse_elif_directive(line, context);
	auto const	else_directive	= parse_else_directive(line, context);
	auto const	endif_directive = parse_endif_directive(line, context);

	std::optional<context_t> _context;
	lines_t					 unavailables;

	auto const& conditions = context.conditions();

	if (! conditions.empty() && ! conditions.top()) {
		// here is unavailable.
		if (endif_directive) {
			_context = context;
			_context->conditions().pop();
		} else if (else_directive || elif_directive) {
			_context = context;
			_context->conditions().pop();
			_context->conditions().push(elif_directive ? std::get<0>(*elif_directive) : true);
		} else {
			// skips unavailable lines.
			auto _unavailables = lines | std::views::drop(1) | std::views::drop_while([&context](auto const& a) {
									 if (a.empty()) return false;
									 return parse_if_directive(a, context) || parse_else_directive(a, context) || parse_endif_directive(a, context);
								 })
							   | std::views::common;
			unavailables.assign(_unavailables.begin(), _unavailables.end());
		}
	} else {
		if (conditions.empty() && (elif_directive || else_directive || endif_directive)) throw std::invalid_argument(std::to_string(__LINE__));
		if (endif_directive) {
			_context = context;
			_context->conditions().pop();
		} else if (else_directive || elif_directive) {
			_context = context;
			_context->conditions().pop();
			_context->conditions().push(elif_directive ? std::get<0>(*elif_directive) : false);
		} else if (if_directive || ifdef_directive) {
			_context = context;
			_context->conditions().push(if_directive ? std::get<0>(*if_directive) : std::get<0>(*ifdef_directive));
		} else {
			// here is other directives or nondirectives.
		}
	}
	auto const _dropped = lines | std::views::drop(1) | std::views::common;
	auto const trails	= ! unavailables.empty() ? unavailables : lines_t(_dropped.begin(), _dropped.end());
	return parse_line(trails, context);
}

inline std::tuple<lex::tokens_t, std::optional<context_t>>
preprocess(std::filesystem::path const& path, context_t const& context) {
	log::logger_t logger;
	log::tracer_t tracer{logger, path.string()};

	auto const tokens = lex::scan(path);
	auto const lines  = split_lines(tokens);
	if (lines.empty()) return {lex::tokens_t{}, std::nullopt};

	auto const [_lines, _context] = parse_line(lines, context);
	if (_lines.empty()) return {lex::tokens_t{}, std::nullopt};

	auto const joint = _lines | std::views::join | std::views::common;
	return {lex::tokens_t(joint.begin(), joint.end()), std::move(_context)};
}

} // namespace xxx::xcp::cpp

#if 0
	// -----------------------------------
	// splits the tokens to lines.
	auto const lines = std::accumulate(phase3.cbegin(), phase3.cend(), std::move(std::vector<tokens_t>{tokens_t{}}), [](auto&& lines, auto const& token) {
		lines.back().push_back(token);
		if (auto const ws = std::get_if<whitespace_t>(&token); ws && 0 < ws->newlines()) {
			lines.emplace_back(tokens_t{});
		}
		return std::move(lines);
	});
#endif

#endif // xxx_PREPROCESSOR_HPP_