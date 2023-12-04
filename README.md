# xcp - xxx c++ compiler

 (c) 2023-, Mura. All rights reserved.

## Introduction

 This is a simple C++ compiler for hobby.

 - It is written in the C++20.
 - It conforms to the following standard: ISO/IEC 14882:2020

## Environment

 It supports the following compilers:
 - Visual C++
 - GNU g++
 - LLVM clang++

## Status

 It has not implemented yet.

## License

 It is not licensed now.

## Implementation defined

 Implementation-defined behaviors in C++ standard is the following:

 - #pragma: Ignores any #pragma directive currently.
 - additional execution policies supported by parallel algorithms: Nothing
 - additional file_type enumerators for file systems supporting additional types of file: Defined by library.
 - additional formats for time_get::do_get_date: Defined by library.
 - additional supported forms of preprocessing directive: Nothing
 - algorithms for producing the standard random number distributions: Defined by library.
 - alignment: 8
 - alignment additional values: TBD
 - alignment of bit-fields within a class object: TBD
 - allocation of bit-fields within a class object: TBD
 - any use of an invalid pointer other than to perform indirection or deallocate: TBD
 - argument values to construct basic_ios::failure: Defined by library.
 - assignability of placeholder objects: TBD
 - behavior of iostream classes when traits::pos_type is not streampos or when traits::off_type is not streamoff: Defined by library.
 - behavior of non-standard attributes: Ignores them.
 - bits in a byte: 8
 - choice of larger or smaller value of floating literal: TBD
 - concatenation of some types of string literals: TBD
 - conversions between pointers and integers: TBD
 - converting characters from source character set to execution character set: Assumes both the character sets are the same, so conversion is unnecessasry. [!]
 - converting function pointer to object pointer and vice versa:  TBD
 - default configuration of a pool: TBD
 - default next_buffer_size for a monotonic_buffer_resource: TBD
 - default number of buckets in unordered_map: Defined by library.
 - default number of buckets in unordered_multimap: Defined by library.
 - default number of buckets in unordered_multiset: Defined by library.
 - default number of buckets in unordered_set: Defined by library.
 - defining main in freestanding environment: Yes, it is in freestanding environmnet currently.
 - definition and meaning of __STDC__: The macro is not defined.
 - definition and meaning of __STDC_VERSION__: The macro is not defined.
 - definition of NULL: The NULL is defined in the <cstddef> header [!]
 - derived type for typeid: TBD
 - diagnostic message: It is minimal currently. [!]
 - dynamic initialization of static inline variables before main: TBD
 - dynamic initialization of static variables before main: TBD
 - dynamic initialization of thread-local variables before entry: TBD
 - effect of calling associated Laguerre polynomials with n >= 128 or m >= 128: TBD
 - effect of calling associated Legendre polynomials with l >= 128: TBD
 - effect of calling basic_filebuf::setbuf with nonzero arguments: Defined by library
 - effect of calling basic_filebuf::sync when a get area exists: Defined by library
 - effect of calling basic_streambuf::setbuf with nonzero arguments: Defined by library
 - effect of calling cylindrical Bessel functions of the first kind with nu >= 128: Defined by library
 - effect of calling cylindrical Neumann functions with nu >= 128: Defined by library
 - effect of calling Hermite polynomials with n >= 128: Defined by library
 - effect of calling ios_base::sync_with_stdio after any input or output operation on standard streams: Defined by library
 - effect of calling irregular modified cylindrical Bessel functions with nu >= 128: Defined by library
 - effect of calling Laguerre polynomials with n >= 128: Defined by library
 - effect of calling Legendre polynomials with l >= 128: Defined by library
 - effect of calling regular modified cylindrical Bessel functions with nu >= 128: Defined by library
 - effect of calling spherical associated Legendre functions with l >= 128: Defined by library
 - effect of calling spherical Bessel functions with n >= 128: Defined by library
 - effect of calling spherical Neumann functions with n >= 128: Defined by library
 - effect of filesystem::copy: Defined by library
 - effect on C locale of calling locale::global: Defined by library
 - encoding of universal character name not in execution character set: Compile failed [!]
 - error_category for errors originating outside the operating system: Defined by library
 - exception type when random_device constructor fails: Defined by library
 - exception type when random_device::operator() fails: Defined by library
 - exception type when shared_ptr constructor fails: Defined by library
 - exceptions thrown by standard library functions that have a potentially-throwing
 - exception specification: Ignored
 - execution character set and execution wide-character set: The first character set is UTF-8, wide-character set is UTF-16(BE).
 - exit status: EXIT_SUCCESS is zero, EXIT_FAILURE is -1. [!]
 - extended signed integer types: TBD
 - file type of the file argument of filesystem::status: Defined by library
 - formatted character sequence generated by time_put::do_put in C locale: Defined by library
 - forward progress guarantees for implicit threads of parallel algorithms (if not defined for thread): Defined by library
 - growth factor for monotonic_buffer_resource: Defined by library
 - headers for freestanding implementation: Provides the following:
	- atomic
	- cfloat
	- climits
	- cstdarg
	- cstddef
	- cstdint
	- cstdlib(minimal)
	- exception
	- initializer_list
	- limits
	- new
	- typeinfo
	- typet_traits
 - how random_device::operator() generates values: Defined by library
 - interactive device: TBD
 - interpretation of the path character sequence with format path::auto_format: Defined by library
 - largest supported value to configure the largest allocation satisfied directly by a pool: TBD
 - largest supported value to configure the maximum number of blocks to replenish a pool: TBD
 - linkage of main: Default "C++" linlkage [!]
 - linkage of names from C standard library: Default "C++" linkage [!]
 - linkage of objects between C++ and other languages: The "C" linkage [!]
 - locale names: Defined locals are the following: [!]
	- "": system native
	- "C": The C language comatible
	- "en_US": English in USA.
	- "ja_JP": Japanese in Japan.
 - lvalue-to-rvalue conversion of an invalid pointer value: TBD
 - manner of search for included source file: In related path from including source file.
 - mapping from physical source file characters to basic source character set: Non basic source character is dealt as universal character. The CR folloed by LF is dealt as single LF as new-line.
 - mapping header name to header or external source file: TBD
 - mapping of pointer to integer: TBD
 - mapping physical source file characters to basic source character set:
	- assumes source file characters are in UTF-8.
	- deals CR-LF pair as the same as the one new-line character, single LF only.
 - mapping to message when calling messages::do_get: Defined by library
 - maximum depth of recursive template instantiations: TBD
 - maximum size of an allocated object: TBD
 - meaning of ’, \, /*, or // in a q-char-sequence or an h-char-sequence: TBD
 - meaning of asm declaration: Inoine assembly in Intel-style format.
 - meaning of attribute declaration: TBD
 - meaning of dot-dot in root-directory: Defined by library
 - negative value of character literal in preprocessor: TBD
 - nesting limit for #include directives: TBD
 - NTCTS in basic_ostream<charT, traits>& operator<<(nullptr_t): Defined by library
 - number of placeholders for bind expressions: TBD
 - number of threads in a program under a freestanding implementation: TBD
 - numeric values of character literals in #if directives: UTF-8 (ASCII)
 - operating system on which implementation depends: POSIX or Win32 [!]
 - parameters to main: The first is always program name. Following parameters are arguments of the program if exists.
 - passing argument of class type through ellipsis: TBD 
 - physical source file characters: UTF-8
 - presence and meaning of native_handle_type and native_handle: Defined by library
 - range defined for character literals: 0-127
 - rank of extended signed integer type: TBD
 - representation of char: TBD
 - required libraries for freestanding implementation: TBD
 - resource limits on a message catalog: TBD
 - result of filesystem::file_size: Definedd by library
 - result of inexact floating-point conversion: TBD
 - result of right shift of negative value: TBD
 - return value of bad_alloc::what: Definedd by library
 - return value of bad_any_access::what: Definedd by library
 - return value of bad_array_new_length::what: Definedd by library
 - return value of bad_cast::what: Definedd by library
 - return value of bad_exception::what: Definedd by library
 - return value of bad_function_call::what: Definedd by library
 - return value of bad_optional_access::what: Definedd by library
 - return value of bad_typeid::what: Definedd by library
 - return value of bad_variant_access::what: Definedd by library
 - return value of bad_weak_ptr::what: Definedd by library
 - return value of char_traits<char16_t>::eof: Definedd by library
 - return value of char_traits<char32_t>::eof: Definedd by library
 - return value of exception::what: Defined by library
 - return value of type_info::name(): Full-qualified type name.
 - search locations for "" header: The priorities are as follows: descending order of lines, and descending order of items within the same line.
	- directory where compiling source file exists in.
	- set using -I argument if exists.
	- set as the INCLUDES environment variable if defined.
	- priset of this compiler: /usr/local/include, /usr/include
 - search locations for <> header: The priorities are as follows: descending order of lines, and descending order of items within the same line.
	- set using -I argument if exists.
	- set as the INCLUDES environment variable if defined.
	- priset of this compiler: /usr/local/include, /usr/include
 - semantics and default value of token parameter to random_device constructor: Defined by library
 - semantics of an access through a volatile glvalue: TBD
 - semantics of linkage specification on templates: TBD
 - semantics of linkage specifiers: TBD
 - semantics of non-standard escape sequences: Invalid character 
 - semantics of parallel algorithms invoked with implementation-defined execution policies: TBD
 - sequence of places searched for a header: TBD
 - set of character types that iostreams templates can be instantiated for: Defined by library
 - signedness of char: signed
 - sizeof applied to fundamental types other than char, signed char, and unsigned char: They are the followings:
	- short: 2
	- int: 4
	- long: 8
	- long long: 8
	- float: 2
	- double: 4
	- long double: 8
	- wchar_t: 2
	- char8_t: 1
	- char16_t: 2
	- char32_t: 4
 - stack unwinding before call to std::terminate(): TBD
 - start-up and termination in freestanding environment: TBD
 - string resulting from __func__: [!]
 - support for extended alignment: Unsupported [!]
 - support for extended alignments: Unsupported [!]
 - support for over-aligned types: Unsupported [!]
 - supported multibyte character encoding rules: UTF-8
 - supported root-names in addition to any operating system dependent root-names: The '/' always maens root-name.
 - text of __DATE__ when date of translation is not available: "Non 99 9999"
 - text of __TIME__ when time of translation is not available: 99:99:99"
 - threads and program points at which deferred dynamic initialization is performed: TBD 
 - type of a directory-like file:  type of array::const_iterator: Defined by library
 - type of array::iterator: Defined by library
 - type of basic_string::const_iterator: Defined by library
 - type of basic_string::iterator: Defined by library
 - type of basic_string_view::const_iterator: Defined by library
 - type of default_random_engine: Defined by library
 - type of deque::const_iterator: Defined by library
 - type of deque::iterator: Defined by library
 - type of filesystem trivial clock: Defined by library
 - type of forward_list::const_iterator: Defined by library
 - type of forward_list::iterator: Defined by library
 - type of list::const_iterator: Defined by library
 - type of list::iterator: Defined by library
 - type of map::const_iterator: Defined by library
 - type of map::iterator: Defined by library
 - type of multimap::const_iterator: Defined by library
 - type of multimap::iterator: Defined by library
 - type of multiset::const_iterator: Defined by library
 - type of multiset::iterator: Defined by library
 - type of ptrdiff_t: This compiler assumes "long long" although defined by library
 - type of regex_constants::error_type: Defined by library
 - type of regex_constants::match_flag_type: Defined by library
 - type of set::const_iterator: Defined by library
 - type of set::iterator: Defined by library
 - type of size_t: This compiler assumes "unsigned long long" although defined by library.
 - type of streamoff: Defined by library
 - type of streampos: Defined by library
 - type of syntax_option_type: Defined by library
 - type of u16streampos: Defined by library
 - type of u32streampos: Defined by library
 - type of unordered_map::const_iterator: Defined by library
 - type of unordered_map::const_local_iterator: Defined by library
 - type of unordered_map::iterator: Defined by library
 - type of unordered_map::local_iterator: Defined by library
 - type of unordered_multimap::const_iterator: Defined by library
 - type of unordered_multimap::const_local_iterator: Defined by library
 - type of unordered_multimap::iterator: Defined by library
 - type of unordered_multimap::local_iterator: Defined by library
 - type of unordered_multiset::const_iterator: Defined by library
 - type of unordered_multiset::const_local_iterator: Defined by library
 - type of unordered_multiset::iterator: Defined by library
 - type of unordered_multiset::local_iterator: Defined by library
 - type of unordered_set::const_iterator: Defined by library
 - type of unordered_set::const_local_iterator: Defined by library
 - type of unordered_set::iterator: Defined by library
 - type of unordered_set::local_iterator: Defined by library
 - type of vector::const_iterator: Defined by library
 - type of vector::iterator: Defined by library
 - type of vector<bool>::const_iterator: Defined by library
 - type of vector<bool>::iterator: Defined by library
 - type of wstreampos: Defined by library
 - underlying type for enumeration: TBD 
 - value of bit-field that cannot represent assigned value: TBD
 - incremented value: TBD
 - initializer: TBD
 - value of character literal outside range of corresponding type: TBD
 - value of ctype<char>::table_size: Defined by library [!]
 - value of multicharacter literal: Unsupported
 - value of pow(0,0): Defined bt library
 - value of result of inexact integer to floating-point conversion: TBD
 - value of result of unsigned to signed conversion: TBD
 - value of wide-character literal containing multiple characters: Invalid character
 - value of wide-character literal with single c-char that is not in execution wide-character set: TBD
 - value representation of floating-point types: 0xXXXXXXXX
 - value representation of pointer types: 0xXXXXXXXX
 - values of a trivially copyable type: TBD
 - values of various ATOMIC_..._LOCK_FREE macros: TBD
 - whether <cfenv> functions can be used to manage floating-point status: TBD
 - whether a given atomic type’s operations are always lock free: Defined by library
 - whether an implementation has relaxed or strict pointer safety: TBD
 - whether functions from Annex K of the C standard library are declared when C++ headers are included: TBD
 - whether get_pointer_safety returns pointer_safety::relaxed or pointer_safety::preferred if the implementation has relaxed pointer safety: TBD
 - whether locale object is global or per-thread: Global
 - whether pragma FENV_ACCESS is supported: Unsupported
 - whether rand may introduce a data race: TBD
 - whether sequence pointers are copied by basic_filebuf move constructor: Defined by library
 - whether sequence pointers are copied by basic_stringbuf move constructor: Defined by library
 - whether source of translation units must be available to locate template definitions: TBD
 - whether stack is unwound before calling std::terminate() when a noexcept specification is violated: TBD
 - whether the lifetime of a parameter ends when the callee returns or at the end of the enclosing full-expression: TBD
 - whether the thread that executes main and the threads created by std::thread provide concurrent forward progress guarantees: TBD
 - whether time_get::do_get_year accepts two-digit year numbers: Defined by library
 - whether values are rounded or truncated to the required precision when converting between time_t values and time_point objects: Defined by library
 - whether variant supports over-aligned types: Defined by library
 - which functions in the C++ standard library may be recursively reentered: Defined by library.
 - which scalar types have unique object representations: TBD


