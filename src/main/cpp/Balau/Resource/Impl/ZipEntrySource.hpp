// @formatter:off
//
// Balau core C++ library
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//
#ifndef COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__ZIP_ENTRY_SOURCE
#define COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__ZIP_ENTRY_SOURCE

#include <Balau/Util/Zip.hpp>

#include <boost/iostreams/categories.hpp>
#include <boost/iostreams/concepts.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/iostreams/device/file_descriptor.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/filtering_stream.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>

namespace Balau::Resource::Impl {

///
/// Boost IO streams zip entry source, used in the zip entry input stream.
///
class ZipEntrySource : public boost::iostreams::source {
	private: Util::Unzipper & archive;
	private: unsigned long long index;
	private: std::shared_ptr<void> zipFile;

	public: ZipEntrySource(Util::Unzipper & archive_, unsigned long long index_);

	public: ZipEntrySource(const ZipEntrySource & copy) = default;

	public: std::streamsize read(char * s, std::streamsize n);

	////////////////////////// Private implementation /////////////////////////

	private: std::shared_ptr<void> createZipFilePointer(Util::Unzipper & archive);
};

//
// Boost IO streams zip entry sink, used in the zip entry output stream.
//
// todo implement this class
//
//class ZipEntrySink : public boost::iostreams::sink {
//	private: Util::Zipper & archive;
//	private: unsigned long long index;
//	private: void * zipFile;
//
//	public: ZipEntrySink(Util::Zipper & archive_, unsigned long long index_);
//
//	public: ~ZipEntrySink();
//
//	public: std::streamsize write(char * s, std::streamsize n);
//};

} // namespace Balau::Resource::Impl

#endif // COM_BORA_SOFTWARE__BALAU_RESOURCE_IMPL__ZIP_ENTRY_SOURCE
