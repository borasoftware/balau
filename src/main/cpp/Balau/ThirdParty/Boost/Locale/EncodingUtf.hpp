//
// Boost locale functions converted to string types with a specified allocator.
//

//
//  Copyright (c) 2009-2011 Artyom Beilis (Tonkikh)
//
//  Distributed under the Boost Software License, Version 1.0. (See
//  accompanying file LICENSE_1_0.txt or copy at
//  http://www.boost.org/LICENSE_1_0.txt)
//

#ifndef BALAU_BOOST_LOCALE_ENCODING_UTF_HPP
#define BALAU_BOOST_LOCALE_ENCODING_UTF_HPP

#include <boost/locale/utf.hpp>
#include <boost/locale/encoding_errors.hpp>
#include <iterator>

#ifdef BOOST_MSVC
#  pragma warning(push)
#  pragma warning(disable : 4275 4251 4231 4660)
#endif

namespace Balau::Boost::Locale::Conv {

///
/// Convert a Unicode text in range [begin,end) to other Unicode encoding
///
template <typename CharOut, typename CharIn, typename AllocatorT>
std::basic_string<CharOut, std::char_traits<CharOut>, AllocatorT>
utf_to_utf(CharIn const * begin,
           CharIn const * end,
           boost::locale::conv::method_type how = boost::locale::conv::default_method) {
	using StringOutT = std::basic_string<CharOut, std::char_traits<CharOut>, AllocatorT>;

	StringOutT result;
	result.reserve(end - begin);
	using inserter_type = std::back_insert_iterator<StringOutT>;
	inserter_type inserter(result);
	boost::locale::utf::code_point c;

	while (begin != end) {
		c = boost::locale::utf::utf_traits<CharIn>::template decode<CharIn const *>(begin, end);
		if (c == boost::locale::utf::illegal || c == boost::locale::utf::incomplete) {
			if (how == boost::locale::conv::stop) {
				throw boost::locale::conv::conversion_error();
			}
		} else {
			boost::locale::utf::utf_traits<CharOut>::template encode<inserter_type>(c, inserter);
		}
	}

	return result;
}

///
/// Convert a Unicode NUL terminated string \a str other Unicode encoding
///
template <typename CharOut, typename CharIn, typename AllocatorT>
std::basic_string<CharOut, std::char_traits<CharOut>, AllocatorT>
utf_to_utf(CharIn const * str, boost::locale::conv::method_type how = boost::locale::conv::default_method) {
	CharIn const * end = str;

	while (*end) {
		end++;
	}

	return utf_to_utf<CharOut, CharIn, AllocatorT>(str, end, how);
}


///
/// Convert a Unicode string \a str other Unicode encoding
///
template <typename CharOut, typename CharIn, typename AllocatorT>
std::basic_string<CharOut, std::char_traits<CharOut>, AllocatorT>
utf_to_utf(std::basic_string<CharIn, std::char_traits<CharIn>, AllocatorT> const & str,
           boost::locale::conv::method_type how = boost::locale::conv::default_method) {
	return utf_to_utf<CharOut, CharIn, AllocatorT>(str.c_str(), str.c_str() + str.size(), how);
}

} // Balau::Boost::Locale::Conv

#ifdef BOOST_MSVC
#pragma warning(pop)
#endif

#endif // BALAU_BOOST_LOCALE_ENCODING_UTF_HPP
