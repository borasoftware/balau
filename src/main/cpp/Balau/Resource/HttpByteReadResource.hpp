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
/// @file HttpByteReadResource.hpp
///
/// A read-only HTTP resource which is read as bytes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTP_BYTE_READ_RESOURCE

#include <Balau/Resource/ByteReadResource.hpp>
#include <Balau/Resource/Impl/HttpSource.hpp>

#include <boost/iostreams/stream.hpp>

namespace Balau::Resource {

class Http;
class Uri;

///
/// A read-only HTTP resource which is read as bytes.
///
class HttpByteReadResource : public ByteReadResource {
	///
	/// Create a new HTTP byte read resource from the supplied HTTP URI.
	///
	/// @throw InvalidUriException if no host information was provided
	///
	public: explicit HttpByteReadResource(const Http & url_);

	public: ~HttpByteReadResource() {
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

	private: std::unique_ptr<Http> url;
	private: boost::iostreams::stream<Impl::HttpSource> stream;
};

} // namespace Balau::Resource

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE__HTTPS_BYTE_READ_RESOURCE
