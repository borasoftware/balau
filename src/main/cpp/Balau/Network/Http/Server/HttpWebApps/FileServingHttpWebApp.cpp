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

namespace Balau::Network::Http::HttpWebApps {

void FileServingHttpWebApp::handleGetRequest(HttpSession & session, const StringRequest & request) {
	Resource::File path = resolvePath(session, request);

	if (!path.exists() || !path.isRegularFile()) {
		session.sendResponse(createNotFoundStringResponse(session, request));
		return;
	}

	const auto pathStr = path.toRawString();

	auto body = getBody(session, request, pathStr);

	auto mimeType = session.configuration().mimeTypes->lookup(pathStr);

	Response<FileBody> response{
		  std::piecewise_construct
		, std::make_tuple(std::move(body))
		, std::make_tuple(Status::ok, request.version())
	};

	auto timestamp = std::chrono::time_point_cast<std::chrono::seconds>(session.configuration().clock->now());

	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, mimeType);
	response.set(Field::cache_control, "public,max-age=600"); // todo parameterise
	response.set(Field::date, Util::DateTime::toString("%a, %d %b %Y %T GMT", timestamp));
	response.prepare_payload();
	response.keep_alive(request.keep_alive());

	session.sendResponse(std::move(response));
}

void FileServingHttpWebApp::handleHeadRequest(HttpSession & session, const StringRequest & request) {
	Resource::File path = resolvePath(session, request);

	if (!path.exists() || !path.isRegularFile()) {
		session.sendResponse(createNotFoundHeadResponse(session, request));
		return;
	}

	auto mimeType = session.configuration().mimeTypes->lookup(path.toRawString());

	Response<EmptyBody> response { Status::ok, request.version() };
	response.set(Field::server, session.configuration().serverId);
	response.set(Field::content_type, mimeType);
	//response.set(Field::content_length, path.size()); // TODO This is causing a deadlock.
	response.keep_alive(request.keep_alive());
	session.sendResponse(std::move(response));
}

void FileServingHttpWebApp::handlePostRequest(HttpSession & session, const StringRequest & request) {
	session.sendResponse(
		createServerErrorResponse(session, request, "Post requests are not implemented.")
	);
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
