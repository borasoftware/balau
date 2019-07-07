// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "FileServingHttpWebApp.hpp"

#include "../HttpSession.hpp"
#include "../../../../Application/EnvironmentProperties.hpp"

namespace Balau::Network::Http::HttpWebApps {

FileServingHttpWebApp::FileServingHttpWebApp(Resource::File documentRoot_, std::string defaultFile_)
	: documentRoot(std::move(documentRoot_))
	, defaultFile(std::move(defaultFile_)) {}

Resource::File determineDocumentRoot(const EnvironmentProperties & configuration) {
	if (!configuration.hasUnique<Resource::Uri>("root")) {
		ThrowBalauException(
			Exception::NetworkException, "Missing \"root\" configuration property for file server."
		);
	}

	auto uri = configuration.getUnique<Resource::Uri>("root");
	auto file = dynamic_cast<Resource::File *>(uri.get());

	if (!file || !file->exists() || !file->isRegularDirectory()) {
		ThrowBalauException(
			  Exception::NetworkException
			, "File server document \"root\" property must be a directory on the local file system."
		);
	}

	return *file;
}

FileServingHttpWebApp::FileServingHttpWebApp(const EnvironmentProperties & configuration, const BalauLogger & logger)
	: documentRoot(determineDocumentRoot(configuration))
	, defaultFile(configuration.getValue<std::string>("root", "index.html")) {}

void FileServingHttpWebApp::handleGetRequest(HttpSession & session,
                                             const StringRequest & request,
                                             std::map<std::string, std::string> & ) {
	Resource::File path = resolvePath(session, request);

	if (!path.exists() || !path.isRegularFile()) {
		session.sendResponse(createNotFoundStringResponse(session, request));
		return;
	}

	const auto pathStr = path.toRawString();
	auto body = getBody(session, request, pathStr);

	Response<FileBody> response{
		  std::piecewise_construct
		, std::make_tuple(std::move(body))
		, std::make_tuple(Status::ok, request.version())
	};

	auto timestamp = std::chrono::time_point_cast<std::chrono::seconds>(session.configuration().clock->now());

	auto mimeType = session.configuration().mimeTypes->lookup(pathStr);

	if (!mimeType.empty()) {
		response.set(Field::content_type, mimeType);
	}

	response.set(Field::server, session.configuration().serverId);
	response.set(Field::cache_control, "public,max-age=600"); // todo parameterise
	response.set(Field::date, Util::DateTime::toString("%a, %d %b %Y %T GMT", timestamp));
	response.prepare_payload();
	response.keep_alive(request.keep_alive());

	session.sendResponse(std::move(response));
}

void FileServingHttpWebApp::handleHeadRequest(HttpSession & session,
                                              const StringRequest & request,
                                              std::map<std::string, std::string> & ) {
	Resource::File path = resolvePath(session, request);
	const auto pathStr = path.toRawString();

	if (!path.exists() || !path.isRegularFile()) {
		session.sendResponse(createNotFoundHeadResponse(session, request));
		return;
	}

	Response<EmptyBody> response { Status::ok, request.version() };

	auto mimeType = session.configuration().mimeTypes->lookup(pathStr);

	if (!mimeType.empty()) {
		response.set(Field::content_type, mimeType);
	}

	response.set(Field::server, session.configuration().serverId);
	response.keep_alive(request.keep_alive());
	session.sendResponse(std::move(response));
}

void FileServingHttpWebApp::handlePostRequest(HttpSession & session,
                                              const StringRequest & request,
                                              std::map<std::string, std::string> & ) {
	session.sendResponse(createNotFoundStringResponse(session, request));
}

Resource::File FileServingHttpWebApp::resolvePath(HttpSession & session, const StringRequest & request) {
	const auto target = request.target();

	Resource::File path = documentRoot / std::string(target.begin(), target.end());

	// Append default file if no file specified?
	if (!defaultFile.empty() && target.back() == '/') {
		path /= defaultFile;
	}

	return path;
}

FileBodyValue FileServingHttpWebApp::getBody(HttpSession & session,
                                            const StringRequest & request,
                                            const std::string & pathStr) {
	FileBodyValue body;
	BoostSystemErrorCode errorCode;
	body.open(pathStr.c_str(), FileMode::scan, errorCode);

	if (errorCode == boost::system::errc::no_such_file_or_directory) {
		session.sendResponse(createNotFoundStringResponse(session, request));
	} else if (errorCode) {
		session.sendResponse(createServerErrorResponse(session, request, errorCode.message()));
	}

	return body;
}

} // namespace Balau::Network::Http::HttpWebApps
