// ===========================================================================
/// @file
/// @brief		Utilities.
/// @author		Mura.
/// @copyright	(c) 2023-, Mura.
// ===========================================================================

#ifndef xxx_UTILITY_HPP_
#define xxx_UTILITY_HPP_

// ===========================================================================

///	@brief	The root namespace of the xxx series.
namespace xxx {
}

///	@brief	Utilities to log messages.
namespace xxx::log {
}

///	@brief	Utilities of UNICODE.
namespace xxx::uc {
}

///	@brief	General utilities.
namespace xxx::util {
}

// ===========================================================================

#include <source_location>
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <algorithm>
#include <cstddef>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <numeric>
#include <ranges>
#include <regex>
#include <sstream>
#include <stdexcept>
#include <string>
#include <tuple>
#include <vector>

#if ! __has_include(<source_location>) && __has_include(<experimental/source_location>)
#include <experimental/source_location>
namespace std {
using std::experimental::source_location;
}
#elif ! defined(__cpp_lib_source_location)
namespace std {
struct source_location {
	static consteval source_location current() noexcept;
	constexpr source_location() noexcept {}
	constexpr uint_least32_t line() const noexcept { return 0; }
	constexpr uint_least32_t column() const noexcept { return 0; }
	constexpr const char*	 file_name() const noexcept { return ""; }
	constexpr const char*	 function_name() const noexcept { return ""; }
};
inline consteval source_location
source_location::current() noexcept { return source_location{}; }
} // namespace std
#endif

namespace xxx {

// ===========================================================================
namespace log {

/// @brief Logger.
///		This class logs message.
/// @todo	It is just minimal implementation.
class logger_t {
public:
	/// @brief Logs message.
	/// @param[in]	level    Logging level.
	/// @param[in]	msg      Message to log.
	/// @param[in]	loc      Source location.
	void operator()(std::size_t level, std::string_view const& msg, std::source_location const& loc = std::source_location::current()) const {
		if (level <= level_) return;
		os_ << "[" << file_name(loc) << ":" << std::setfill('_') << std::setw(5) << loc.line() << "] " << msg << std::endl;
	}

	/// @brief Constructor.
	/// @param[in]	level    Logging level.
	/// @param[in]	os       Output stream.
	explicit logger_t(std::size_t level = 0u, std::ostream& os = std::clog) noexcept : os_(os), level_{level} {}

private:
	logger_t(logger_t const&) = delete;

	/// @brief	Strips file path name from source location.
	/// @param[in]	loc		Source location.
	/// @return		Simple file path.
	static std::string file_name(std::source_location const& loc) {
		std::filesystem::path const path{loc.file_name()};
		return path.filename().string();
	}

private:
	std::ostream& os_;	  ///< @brief  Output stream.
	std::size_t	  level_; ///< @brief  Logging level.
};

/// @brief Trace logger.
///		This class logs at entering and leaving scope of source block.
class tracer_t {
public:
	// -----------------------------------

	/// @brief Constructor.
	///	@param[in]	logger		Logger.
	///	@param[in]	level		Logging level.
	///	@param[in]	loc			Source location.
	tracer_t(logger_t& logger, std::size_t level, std::source_location const& loc = std::source_location::current()) noexcept : logger_{logger}, loc_{loc}, result_{}, level_{level} {
		std::ostringstream oss;
		oss << ">>> " << function_name(loc_) << "()";
		logger_(level_, oss.str(), loc_);
	}
	/// @overload
	///	@param[in]	logger		Logger.
	///	@param[in]	argument	Argument to log.
	///	@param[in]	level		Logging level.
	///	@param[in]	loc			Source location.
	tracer_t(logger_t& logger, std::size_t level, std::string_view const& argument, std::source_location const& loc = std::source_location::current()) noexcept : logger_{logger}, loc_{loc}, result_{}, level_{level} {
		std::ostringstream oss;
		oss << ">>> " << function_name(loc_) << "(" << argument << ")";
		logger_(level_, oss.str(), loc_);
	}
	/// @overload
	///	@param[in]	logger		Logger.
	///	@param[in]	arguments	Arguments to log.
	///	@param[in]	level		Logging level.
	///	@param[in]	loc			Source location.
	tracer_t(logger_t& logger, std::size_t level, std::vector<std::string> const& arguments, std::source_location const& loc = std::source_location::current()) noexcept : logger_{logger}, loc_{loc}, result_{}, level_{level} {
		std::ostringstream oss;
		oss << ">>> " << function_name(loc_) << "(";
		if (! arguments.empty()) {
			oss << arguments.at(0);
			std::ranges::for_each(arguments | std::views::drop(1), [&oss](auto const& a) { oss << ", " << a; });
		}
		oss << ")";
		logger_(level_, oss.str(), loc_);
	}
	/// @overload
	tracer_t(logger_t& logger, std::size_t level, std::vector<std::string_view> const& arguments, std::source_location const& loc = std::source_location::current()) noexcept : logger_{logger}, loc_{loc}, result_{}, level_{level} {
		std::ostringstream oss;
		oss << ">>> " << function_name(loc_) << "(";
		if (! arguments.empty()) {
			oss << arguments.at(0);
			std::ranges::for_each(arguments | std::views::drop(1), [&oss](auto const& a) { oss << ", " << a; });
		}
		oss << ")";
		logger_(level_, oss.str(), loc_);
	}

	/// @overload
	explicit tracer_t(logger_t& logger, std::source_location const& loc = std::source_location::current()) noexcept : tracer_t(logger, 10, loc) {}
	/// @overload
	tracer_t(logger_t& logger, std::string_view const& argument, std::source_location const& loc = std::source_location::current()) noexcept : tracer_t(logger, 10, argument, loc) {}
	/// @overload
	tracer_t(logger_t& logger, std::vector<std::string> const& arguments, std::source_location const& loc = std::source_location::current()) noexcept : tracer_t(logger, 10, arguments, loc) {}
	/// @overload
	tracer_t(logger_t& logger, std::vector<std::string_view> const& arguments, std::source_location const& loc = std::source_location::current()) noexcept : tracer_t(logger, 10, arguments, loc) {}

	/// @brief	Destructor.
	~tracer_t() {
		std::ostringstream oss;
		oss << "<<< " << function_name(loc_) << "(" << result_ << ")";
		logger_(level_, oss.str(), loc_);
	}

	// -----------------------------------

	/// @brief	Sets result.
	/// @param[in]	result 	Result to log.
	void set_result(std::string_view const& result) { result_ = result; }
	/// @overload
	void set_result(std::string const& result) { result_ = result; }
	/// @overload
	void set_result(bool result) { result_ = result ? "true" : "false"; }
	/// @overload
	///	@tparam	T	Type of @p result.
	template<typename T>
	void set_result(T const& result) {
		std::ostringstream oss;
		oss << result;
		result_ = oss.str();
	}

	// -----------------------------------

	/// @brief	Trace the message.
	/// @param[in]	level 		Logging level.
	/// @param[in]	message		Message to log.
	void trace(std::size_t level, std::string_view const& message) { logger_(level, "--- " + std::string{message}, loc_); }
	/// @overload
	void trace(std::size_t level, std::string const& message) { logger_(level, "--- " + message, loc_); }
	/// @overload
	///	@tparam	T	Type of @p message.
	template<typename T>
	void trace(std::size_t level, T const& message) {
		std::ostringstream oss;
		oss << "--- " << message;
		logger_(level, oss.str(), loc_);
	}
	/// @overload
	void trace(std::string_view const& message) { trace(level_, message); }
	/// @overload
	void trace(std::string const& message) { trace(level_, message); }
	/// @overload
	template<typename T>
	void trace(T const& message) { trace(level_, message); }

private:
	/// @brief	Strips function name from source location.
	/// @param[in]	loc		Source location.
	/// @return		Simple function name.
	static std::string function_name(std::source_location const& loc) {
		std::regex const re{R"(^.* ((?:`?[A-Za-z_][-A-Za-z_0-9<>']*::)*~?[A-Za-z_][A-Za-z_0-9<>]*) ?\(.*$)"};

		std::cmatch match;
		return std::regex_match(loc.function_name(), match, re) ? match.str(1) : loc.function_name();
	}

private:
	logger_t&			 logger_; ///< @brief  Logger.
	std::source_location loc_;	  ///< @brief  Source location.
	std::string			 result_; ///< @brief  Result.
	std::size_t			 level_;  ///< @brief  Logging level.
};

} // namespace log

// ===========================================================================
namespace util {

/// @brief	Escapes string.
/// @param[in]	str		String to escape with white-space characters.
/// @param[in]	limit	Length to trancate.
/// @return	Escaped string.
inline std::string
escape(std::string_view const& str, std::size_t limit = 16u) {
	std::unordered_map<char, std::string_view> const ws_{{'\n', "$n"}, {'\r', "$r"}, {'\f', "$f"}, {'\v', "$v"}, {'\t', "$t"}};

	auto const s = str.substr(0, limit);
	return std::accumulate(s.begin(), s.end(), std::ostringstream{}, [&ws_](auto&& oss, auto const ch) { if (ws_.contains(ch)) oss << ws_.at(ch); else oss << ch; return std::move(oss); }).str();
}

} // namespace util

// ===========================================================================
namespace uc {

/// @brief	Checks if character starts with specified bits.
/// @param[in]	ch		A character.
/// @param[in]	bits	Bits to check.
/// @param[in]	mask	Bit mask to check.
/// @return		Whether the character starts with the specified bits.
constexpr inline bool
starts_bits_with(char ch, unsigned char bits, unsigned char mask) noexcept {
	return (ch & mask) == (bits & mask);
}

/// @brief	Type of UTF-8 character.
enum class type_t {
	H1, ///< @brief	Head byte of a single-bytes character.
	H2, ///< @brief	Head byte of tho-bytes character.
	H3, ///< @brief	Head byte of three-bytes character.
	H4, ///< @brief	Head byte of four-bytes character.
	T,	///< @brief	Trail byte of multiple character.
	E,	///< @brief	Invalid byte as UTF-8 character.
};

/// @brief	Checks if byte is head of UTF-8 character.
/// @param[in]	ch 	Byte to check.
/// @return 	Whether the byte is head of a single character.
constexpr inline bool
is_head1(char ch) noexcept { return starts_bits_with(ch, 0b0000'0000, 0b1000'0000); }
/// @brief	Checks if byte is head of UTF-8 character.
/// @param[in]	ch 	Byte to check.
/// @return 	Whether the byte is trail of character.
constexpr inline bool
is_trail(char ch) noexcept { return starts_bits_with(ch, 0b1000'0000, 0b1100'0000); }
/// @brief	Checks if byte is head of UTF-8 character.
/// @param[in]	ch 	Byte to check.
/// @return 	Whether the byte is head of two-bytes character.
constexpr inline bool
is_head2(char ch) noexcept { return starts_bits_with(ch, 0b1100'0000, 0b1110'0000); }
/// @brief	Checks if byte is head of UTF-8 character.
/// @param[in]	ch 	Byte to check.
/// @return 	Whether the byte is head of three-bytes character.
constexpr inline bool
is_head3(char ch) noexcept { return starts_bits_with(ch, 0b1110'0000, 0b1111'0000); }
/// @brief	Checks if byte is head of UTF-8 character.
/// @param[in]	ch 	Byte to check.
/// @return 	Whether the byte is head of four-bytes character.
constexpr inline bool
is_head4(char ch) noexcept { return starts_bits_with(ch, 0b1111'0000, 0b1111'1000); }

/// @brief	Gets type of the @p ch byte.
/// @param[in]	ch 	Byte to check.
/// @return 	Type of the @p ch byte.
constexpr inline type_t
get_type(char ch) noexcept {
	using enum type_t;
	return is_head1(ch) ? H1
		 : is_head2(ch) ? H2
		 : is_head3(ch) ? H3
		 : is_head4(ch) ? H4
		 : is_trail(ch) ? T
						: E;
}

/// @brief	Gets character as head of a single character.
/// @param[in]	ch 	Character.
/// @return 	Head of the single character, or invalid character.
constexpr inline char32_t
get_head1(char ch) noexcept { return is_head1(ch) ? ch : static_cast<char32_t>(~0uL); }
/// @brief	Gets character as trail of a character.
/// @param[in]	ch 	Character.
/// @return 	Trail of the character, or invalid character.
constexpr inline char32_t
get_trail(char ch) noexcept { return is_head1(ch) ? (ch & ~0b1100'0000) : static_cast<char32_t>(~0uL); }
/// @brief	Gets character as head of two-bytes character.
/// @param[in]	ch 	Character.
/// @return 	Head of the two-bytes character, or invalid character.
constexpr inline char32_t
get_head2(char ch) noexcept { return is_head2(ch) ? (ch & ~0b1110'0000) : static_cast<char32_t>(~0uL); }
/// @brief	Gets character as head of three-bytes character.
/// @param[in]	ch 	Character.
/// @return 	Head of the three-bytes character, or invalid character.
constexpr inline char32_t
get_head3(char ch) noexcept { return is_head3(ch) ? (ch & ~0b1111'0000) : static_cast<char32_t>(~0uL); }
/// @brief	Gets character as head of four-bytes character.
/// @param[in]	ch 	Character.
/// @return 	Head of the four-bytes character, or invalid character.
constexpr inline char32_t
get_head4(char ch) noexcept { return is_head4(ch) ? (ch & ~0b1111'1000) : static_cast<char32_t>(~0uL); }
/// @brief	Gets whether the @p ch is valid byte of UTF-8 character or not.
/// @param[in]	ch 	Character.
/// @return 	Whether the @p ch is valid byte of UTF-8 character or not.
constexpr inline bool
is_valid(char32_t ch) noexcept { return ch <= 0x0010'FFFF; }

/// @brief	Gets a head character from the @p sv string.
/// @param[in]	sv 	String view.
/// @return	Parsed character.
///		The first of tuple is remaining string dropped the head character.
///		The second of tuple is the head character.
inline std::tuple<std::string_view, char32_t>
get_head_ucs32(std::string_view const& sv) {
	using enum type_t;
	if (sv.empty()) return {};
	auto const ch = sv.at(0u);
	switch (get_type(ch)) {
	// -----------------------------------
	case H1: return {sv.substr(1u), get_head1(sv.at(0u))};
	// -----------------------------------
	case H2: {
		if (! is_trail(sv.at(1u))) throw std::invalid_argument(std::string{sv.substr(1u, 1u)});
		auto const v = get_head2(ch) << 6 | get_trail(sv.at(1u));
		if (! is_valid(v)) throw std::invalid_argument(util::escape(sv, 2u));
		return {sv.substr(2u), v};
	}
	// -----------------------------------
	case H3: {
		if (! is_trail(sv.at(1u))) throw std::invalid_argument(std::string{sv.substr(1u, 1u)});
		if (! is_trail(sv.at(2u))) throw std::invalid_argument(std::string{sv.substr(2u, 1u)});
		auto const v = get_head3(ch) << 12 | get_trail(sv.at(1u)) << 6 | get_trail(sv.at(2u));
		if (! is_valid(v)) throw std::invalid_argument(util::escape(sv, 3u));
		return {sv.substr(3u), v};
	}
	// -----------------------------------
	case H4: {
		if (! is_trail(sv.at(1))) throw std::invalid_argument(std::string{sv.substr(1u, 1u)});
		if (! is_trail(sv.at(2))) throw std::invalid_argument(std::string{sv.substr(2u, 1u)});
		if (! is_trail(sv.at(3))) throw std::invalid_argument(std::string{sv.substr(3u, 1u)});
		auto const v = get_head4(ch) << 18 | get_trail(sv.at(1u)) << 12 | get_trail(sv.at(2u)) << 6 | get_trail(sv.at(3u));
		if (! is_valid(v)) throw std::invalid_argument(util::escape(sv, 4u));
		return {sv.substr(4u), v};
	}
	// -----------------------------------
	case E:
	default:
		throw std::invalid_argument(std::string{sv.substr(0u, 1u)});
	}
}

/// @brief	Converts UTF-8 string to UTF-32 string.
///	@param[in]	str		UTF-8 string.
///	@return		UTF-32 string.
inline std::u32string
to_32string(std::string_view const& str) {
	std::basic_ostringstream<char32_t> oss;
	for (auto s = str; ! s.empty();) {
		auto const [rest, ch] = get_head_ucs32(s);
		oss << ch;
		s = rest;
	}
	return oss.str();
}

} // namespace uc

// ===========================================================================
namespace util {

/// @brief	Appends value to container.
/// @tparam	T	Type of @p container.
/// @tparam	Y	Type of @p value.
/// @param[in]	container	Original container.
/// @param[in]	value		Value to append.
/// @return		Appended container.
template<typename T, typename Y>
inline std::vector<T>
push_back(std::vector<T> const& container, Y const& value) {
	std::vector<T> c;
	c.reserve(container.size() + 1u);
	c.insert(c.end(), container.begin(), container.end());
	c.push_back(value);
	return c;
}

/// @brief	Loads while contexts of the file onto memory as string.
/// @param[in]	path	Path of the file to read.
/// @return		Contents of the file.
inline std::string
load_file(std::filesystem::path const& path) {
	log::logger_t logger;
	log::tracer_t tracer{logger, path.string()};

	// -----------------------------------
	//	Checks the path is valid.
	if (path.empty() || ! std::filesystem::exists(path)) throw std::invalid_argument(path.string());
	//	Opens the file.
	std::ifstream ifs;
	ifs.exceptions(std::ios::badbit | std::ios::failbit);
	ifs.open(path, std::ios::in | std::ios::binary);
	ifs.exceptions(std::ios::badbit);

	// -----------------------------------
	//	Gets size of the file.
	auto const size = std::filesystem::file_size(path);
	if (size == 0u) return {}; // Nothing to read because the file is empty.

	// -----------------------------------
	std::ostringstream oss;
	std::copy(std::istreambuf_iterator<char>(ifs), std::istreambuf_iterator<char>(), std::ostreambuf_iterator<char>(oss));
	auto const s = oss.str();
	// Removes the BOM if exists.
	return s.starts_with("\xEF\xBB\xBF") ? s.substr(3u) : s;
}

/// @brief	Converts a view to container.
///	@tparam	Y	Type of outputt container.
///	@tparam	T	Type of input view.
///	@param[in]	a	Container.
/// @return		It returns true if the container is empty.
/// @note   TODO: This is workaround for C++20 or older. C++23 or newer has std::ranges::to.
template<typename Y, typename T>
constexpr inline Y
to(T const& a) {
	using ct = typename std::remove_reference<decltype(a)>::type;
	using t	 = typename std::remove_const<ct>::type;
	auto& aa = const_cast<t&>(a);
	return Y(aa.begin(), aa.end());
}

/// @brief	Gets whether container orr view is empty or not.
///	@tparam	T	Type of the @p a container.
///	@param[in]	a	Container or view.
/// @return		It returns true if it is empty.
template<typename T>
constexpr inline bool
empty(T const& a) {
	using ct = typename std::remove_reference<decltype(a)>::type;
	using t	 = typename std::remove_const<ct>::type;
#if defined(__GNUC__) // TODO: This is workaround for gcc.
	return const_cast<t&>(a).empty();
#else
	return std::ranges::empty(const_cast<t&>(a));
#endif
}

} // namespace util

/// @brief	Regex result for std::string_view.
using svmatch = std::match_results<std::string_view::const_iterator>;

#ifndef const_
///	@brief	immutable marcker for non-const types.
#define const_ /*const*/
#endif

} // namespace xxx

#endif // xxx_UTILITY_HPP_
