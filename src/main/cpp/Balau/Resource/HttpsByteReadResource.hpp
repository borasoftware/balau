// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file HttpsByteReadResource.hpp
///
/// A read-only HTTPS resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE

#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/Impl/HttpsSource.hpp>

#include <boost/iostreams/stream.hpp>

namespace Balau::Resource {

class Https;
class Uri;

///
/// A read-only HTTP resource which is read as bytes.
///
class HttpsByteReadResource : public ByteReadResource {
	///
	/// Create a new HTTPS byte read resource from the supplied HTTPS URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpsByteReadResource(const Https & url);

	public: ~HttpsByteReadResource() {
		close();
	}

	public: std::istream & readStream() override {
		return stream;
	}

	public: const Uri & uri() const override;

	public: void close() override {
		stream.close();
	}

	////////////////////////// Private implementation /////////////////////////

	private: std::unique_ptr<Https> url;
	private: boost::iostreams::stream<Impl::HttpsSource> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE
