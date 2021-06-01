// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
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

#include "MimeTypes.hpp"
#include "../../Util/Strings.hpp"

namespace Balau::Network {

std::shared_ptr<MimeTypes> MimeTypes::defaultMimeTypes = std::shared_ptr<MimeTypes>(
	new MimeTypes(
		std::unordered_map<std::string, std::string> {
			  std::pair<std::string, std::string>("7z",    "application/x-7z-compressed")
			, std::pair<std::string, std::string>("avi",   "video/x-msvideo")
			, std::pair<std::string, std::string>("bdml",  "text/xml")
			, std::pair<std::string, std::string>("bmp",   "image/bmp")
			, std::pair<std::string, std::string>("bz",    "application/x-bzip")
			, std::pair<std::string, std::string>("bz2",   "application/x-bzip2")
			, std::pair<std::string, std::string>("css",   "text/css")
			, std::pair<std::string, std::string>("csv",   "text/csv")
			, std::pair<std::string, std::string>("doc",   "application/msword")
			, std::pair<std::string, std::string>("ear",   "application/java-archive")
			, std::pair<std::string, std::string>("eps",   "application/postscript")
			, std::pair<std::string, std::string>("epub",  "application/epub+zip")
			, std::pair<std::string, std::string>("flv",   "video/x-flv")
			, std::pair<std::string, std::string>("gif",   "image/gif")
			, std::pair<std::string, std::string>("gz",    "application/x-gzip")
			, std::pair<std::string, std::string>("gzip",  "application/x-gzip")
			, std::pair<std::string, std::string>("htm",   "text/html")
			, std::pair<std::string, std::string>("html",  "text/html")
			, std::pair<std::string, std::string>("ico",   "image/x-icon")
			, std::pair<std::string, std::string>("jar",   "application/java-archive")
			, std::pair<std::string, std::string>("jpg",   "image/jpeg")
			, std::pair<std::string, std::string>("jpeg",  "image/jpeg")
			, std::pair<std::string, std::string>("js",    "application/javascript")
			, std::pair<std::string, std::string>("json",  "application/json")
			, std::pair<std::string, std::string>("mml",   "text/mathml")
			, std::pair<std::string, std::string>("mp3",   "audio/mpeg")
			, std::pair<std::string, std::string>("mp4",   "video/mp4")
			, std::pair<std::string, std::string>("mpg",   "video/mpeg")
			, std::pair<std::string, std::string>("mpeg",  "video/mpeg")
			, std::pair<std::string, std::string>("ogg",   "audio/ogg")
			, std::pair<std::string, std::string>("pdf",   "application/pdf")
			, std::pair<std::string, std::string>("png",   "image/png")
			, std::pair<std::string, std::string>("ps",    "application/postscript")
			, std::pair<std::string, std::string>("rtf",   "application/rtf")
			, std::pair<std::string, std::string>("svg",   "image/svg+xml")
			, std::pair<std::string, std::string>("svgz",  "image/svg+xml")
			, std::pair<std::string, std::string>("tar",   "application/x-tar")
			, std::pair<std::string, std::string>("tif",   "image/tiff")
			, std::pair<std::string, std::string>("tiff",  "image/tiff")
			, std::pair<std::string, std::string>("txt",   "text/plain")
			, std::pair<std::string, std::string>("war",   "application/java-archive")
			, std::pair<std::string, std::string>("woff",  "font/woff")
			, std::pair<std::string, std::string>("woff2", "font/woff2")
			, std::pair<std::string, std::string>("xhtml", "application/xhtml+xml")
			, std::pair<std::string, std::string>("xml",   "text/xml")
			, std::pair<std::string, std::string>("zip",   "application/zip")
		}
	)
);

std::string_view MimeTypes::lookup(const std::string & path) const {
	const auto lastDotPosition = path.rfind('.');

	if (lastDotPosition == std::string::npos) {
		return "";
	}

	auto match = data.find(Balau::Util::Strings::toLower(path.substr(lastDotPosition + 1)));

	if (match != data.end()) {
		return match->second;
	}

	return "";
}

} // namespace Balau::Network
