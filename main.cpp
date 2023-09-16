// ===========================================================================
/// @file
/// @brief  Main entry of xcp.
/// @author Mura.
/// @copyright (c) 2023-, Mura.
// ===========================================================================

// Includes standard headers.
#include <string_view>
#include <type_traits>
#include <unordered_map>
#include <unordered_set>
#include <algorithm>
#include <chrono>
#include <clocale>
#include <exception>
#include <filesystem>
#include <iostream>
#include <ranges>
#include <regex>
#include <string>
#include <tuple>
#include <vector>

// ===========================================================================

#ifndef const_
///	@brief	const macro.
#define const_ /*const*/
#endif

// ===========================================================================
/// @brief The root namespace of xxx.
namespace xxx {

// -----------------------------------
/// @brief Messages.
namespace msg {
using namespace std::string_view_literals;

/// @brief Title of this program.
static constexpr auto const Title =
	"=============================\n"
	" xcp - xxx c++ compiler      \n"
	" --------------------------- \n"
	" v0.2.0.0   - (c)2023-, Mura \n"
	"=============================\n"sv;

/// @brief Usage of this program.
static constexpr auto const Usage =
	"[Usage] $ xcp.exe  (options)  {source_files}\n"
	"[Options]\n"
	" --help/-h :                Show this usage only\n"
	" --version/-v :             Show program version only\n"
	" -D{macro_name} :           Define the macro.\n"
	" -D{macro_name}={value} :   Define the macro with the value.\n"
	" -l{library_name} :         Set the name of library.\n"
	" -I{header_path} :          Set search path of includes.\n"
	" -L{linker_path} :          Set search path of libraries.\n"sv;

/// @brief This program completes successfully.
static constexpr auto const Completed = "Done."sv;

// -----------------------------------
/// @brief Error messages.
namespace err {
using namespace std::string_literals;

static auto const Unsupported_input_pipe = "Standard input pipe is unsupported."s;
static auto const Invalid_arguments		 = "Invalid argument(s) appeared."s;
static auto const Invalid_option		 = "Invalid option:"s;
static auto const No_input_file			 = "No input file exists."s;
static auto const No_such_input_file	 = "No such input file(s) exists."s;
static auto const Unexpected			 = "Unexpected exception occurred."s;

} // namespace err
} // namespace msg

// -----------------------------------
namespace impl {

/// @brief Converts a view to container.
///	@tparam Y	Type of outputt container.
///	@tparam T	Type of input view.
///	@param[in]	a	Container.
/// @return It returns true if the container is empty.
/// @note   TODO: This is workaround for C++20 or older. C++23 or newer has std::ranges::to.
template<typename Y, typename T>
constexpr inline Y
to(T const& a) {
	using ct = typename std::remove_reference<decltype(a)>::type;
	using t	 = typename std::remove_const<ct>::type;
	auto& aa = const_cast<t&>(a);
	return Y(aa.begin(), aa.end());
}

/// @brief Gets whether container orr view is empty or not.
///	@tparam T	Type of the @p a container.
///	@param[in]	a	Container or view.
/// @return It returns true if it is empty.
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

} // namespace impl

// -----------------------------------

using arguments_t = std::vector<std::string_view>;					   ///< @brief Type of program arguments.
using paths_t	  = std::unordered_set<std::filesystem::path>;		   ///< @brief Type of source file paths.
using options_t	  = std::unordered_multimap<std::string, std::string>; ///< @brief Type of options.
using messages_t  = std::unordered_set<std::string>;				   ///< @brief Type of error messages.

/// @brief Checks program arguments as options and source paths.
/// @param[in] args     Program arguments.
/// @return Tuple of program arguments as options, source paths, and error messages.
std::tuple<int, options_t, paths_t, messages_t>
check_arguments(arguments_t const& args) {
	messages_t const						   success;
	messages_t const						   unsupported_input_pipe_message = {msg::err::Unsupported_input_pipe};
	messages_t const						   no_input_file_message		  = {msg::err::No_input_file};
	messages_t const						   no_such_input_file_message	  = {msg::err::No_such_input_file};
	std::unordered_set<std::string_view> const usage_options				  = {"-h", "--help", "-v", "--version"};

	// -----------------------------------
	// This program does not support standard input pipe.
	auto const pipe		   = args | std::views::all | std::views::filter([](auto const& a) { return a == "-"; }) | std::views::common;
	auto const pipe_errors = impl::empty(pipe) ? success : unsupported_input_pipe_message;

	// -----------------------------------
	// Collects options.
	auto const parse_option = [](std::string_view const& a) -> options_t::value_type {
		// Format: --opt, -opt, --opt=value, -opt=value, --opt:value, -opt:value
		std::regex const re{R"(^(--?[A-Za-z0-9_-]+)(?:[=:](.+))?$)"};
		if (std::match_results<std::string_view::const_iterator> match; std::regex_match(std::ranges::cbegin(a), std::ranges::cend(a), match, re)) {
			return (2 < match.size()) //
					 ? std::make_pair(std::string{match.str(1)}, std::string{match.str(2)})
					 : std::make_pair(std::string{match.str(1)}, std::string{});
		} else {
			return {"", std::string{a}}; // Invalid option is mapped to empty key.
		};
	};

	auto const_ options			= args | std::views::all | std::views::filter([](auto const& a) { return a.starts_with("-") && ! std::filesystem::exists(a); }) | std::views::transform(parse_option) | std::views::common;
	auto const_ invalid_options = options | std::views::filter([](auto const& a) { return a.first.empty(); }) | std::views::common;
	auto const	option_errors	= impl::empty(invalid_options) ? success : impl::to<messages_t>(invalid_options | std::views::transform([](auto const& a) { return msg::err::Invalid_option + a.second; }) | std::views::common);
	auto const	show_only		= ! impl::empty(options | std::views::filter([&usage_options](auto const& a) { return usage_options.contains(a.first); }) | std::views::common);

	// TODO: option_errors

	// -----------------------------------
	// Collects source file paths.
	auto const_ sources		   = args | std::views::all | std::views::filter([](auto const& a) { return ! a.starts_with("-") || std::filesystem::exists(a); }) | std::views::transform([](auto const& a) { return std::filesystem::path{a}; }) | std::views::common;
	auto const	source_missing = sources | std::views::filter([](auto const& a) { return ! std::filesystem::exists(a); }) | std::views::common;
	auto const	source_errors  = (impl::empty(sources) && ! show_only) //
								   ? no_input_file_message
								   : impl::empty(source_missing) //
										 ? success
										 : no_such_input_file_message;

	// -----------------------------------
	// Collects errors.
	auto const errors = std::vector{pipe_errors, option_errors, source_errors} | std::views::join;
	auto const result = show_only //
						  ? 1
						  : impl::empty(errors) //
								? 0
								: -1;

	return std::make_tuple(result, impl::to<options_t>(options), impl::to<paths_t>(sources), impl::to<messages_t>(errors));
}

} // namespace xxx

// ===========================================================================

/// @brief Initializes primitive language features.
static inline void
initialize_primitives() {
	// -----------------------------------
	// Turns off synchronization with the standard I/O streams
	// because this program does not use C-based I/O.
	std::ios::sync_with_stdio(false);

	// -----------------------------------
	// Sets default locale to the standard I/O streams.
	auto const default_locale = std::locale("C"); // for compatibility
	std::locale::global(default_locale);
	std::cin.imbue(default_locale);
	std::cout.imbue(default_locale);
	std::cerr.imbue(default_locale);
	std::clog.imbue(default_locale);
	std::cin.tie(&std::clog);
}

/// @brief  Main entry of this program.
/// @param[in]  ac  Arugument count.
/// @param[in]  av  AArgument values.
/// @return Result code of this program.
///     It returns zero if the program succeeded; otherwise,
///     it returns positive value if the program exits without actual effects
///     such as to show usage or version only; otherwise,
///     it returns nagative value if the program aborted by any failure.
int
main(int ac, char* av[]) {
	try {
		// -----------------------------------
		// Iniitializes this program.
		initialize_primitives();
		std::clog << xxx::msg::Title << std::endl;

		// -----------------------------------
		// Checks arguments as options and source paths.
		xxx::arguments_t const args(av + 1, av + ac);
		auto const [result, options, paths, errors] = xxx::check_arguments(args);
		if (result != 0) {
			if (0 < result) {
				std::clog << xxx::msg::Usage << std::endl;
			} else {
				std::clog << xxx::msg::err::Invalid_arguments << std::endl;
				std::ranges::for_each(errors, [](auto const& a) { std::clog << std::string(4, ' ') << a << std::endl; });
			}
			return result;
		}

		// -----------------------------------
		{
			auto const started = std::chrono::steady_clock::now();

			// TODO:

			auto const ended = std::chrono::steady_clock::now();
			std::clog << xxx::msg::Completed << ":" << std::chrono::duration_cast<std::chrono::milliseconds>(ended - started).count() << "ms." << std::endl;
		}
		return 0;

		// -----------------------------------
	} catch (std::exception const& e) {
		// Handles exceptions.
		std::cerr << xxx::msg::err::Unexpected << ": " << e.what() << std::endl;
	} catch (...) {
		std::cerr << xxx::msg::err::Unexpected << std::endl;
	}
	return -1;
}
