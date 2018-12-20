// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "MimeTypes.hpp"
#include "../../Util/Strings.hpp"

namespace Balau::Network {

std::shared_ptr<MimeTypes> MimeTypes::defaultMimeTypes = std::shared_ptr<MimeTypes>(
	new MimeTypes(
		std::unordered_map<std::string, std::string> {
			  std::pair("7z",    "application/x-7z-compressed")
			, std::pair("avi",   "video/x-msvideo")
			, std::pair("bdml",  "text/xml")
			, std::pair("bmp",   "image/bmp")
			, std::pair("bz",    "application/x-bzip")
			, std::pair("bz2",   "application/x-bzip2")
			, std::pair("css",   "text/css")
			, std::pair("csv",   "text/csv")
			, std::pair("doc",   "application/msword")
			, std::pair("ear",   "application/java-archive")
			, std::pair("eps",   "application/postscript")
			, std::pair("epub",  "application/epub+zip")
			, std::pair("flv",   "video/x-flv")
			, std::pair("gif",   "image/gif")
			, std::pair("gz",    "application/x-gzip")
			, std::pair("gzip",  "application/x-gzip")
			, std::pair("htm",   "text/html")
			, std::pair("html",  "text/html")
			, std::pair("ico",   "image/x-icon")
			, std::pair("jar",   "application/java-archive")
			, std::pair("jpg",   "image/jpeg")
			, std::pair("jpeg",  "image/jpeg")
			, std::pair("js",    "application/javascript")
			, std::pair("json",  "application/json")
			, std::pair("mml",   "text/mathml")
			, std::pair("mp3",   "audio/mpeg")
			, std::pair("mp4",   "video/mp4")
			, std::pair("mpg",   "video/mpeg")
			, std::pair("mpeg",  "video/mpeg")
			, std::pair("ogg",   "audio/ogg")
			, std::pair("pdf",   "application/pdf")
			, std::pair("png",   "image/png")
			, std::pair("ps",    "application/postscript")
			, std::pair("rtf",   "application/rtf")
			, std::pair("svg",   "image/svg+xml")
			, std::pair("svgz",  "image/svg+xml")
			, std::pair("tar",   "application/x-tar")
			, std::pair("tif",   "image/tiff")
			, std::pair("tiff",  "image/tiff")
			, std::pair("txt",   "text/plain")
			, std::pair("war",   "application/java-archive")
			, std::pair("woff",  "font/woff")
			, std::pair("woff2", "font/woff2")
			, std::pair("xhtml", "application/xhtml+xml")
			, std::pair("xml",   "text/xml")
			, std::pair("zip",   "application/zip")
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
