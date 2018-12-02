// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RoutingHttpWebApp.hpp"
#include "../HttpSession.hpp"

namespace Balau::Network::Http::HttpWebApps {

RoutingHttpWebApp::RoutingHttpWebApp(Routing && routing_) : routing(std::move(routing_)) {}

void RoutingHttpWebApp::handleGetRequest(HttpSession & session, const StringRequest & request) {
	HttpWebApp * handler = resolve(session, request);

	if (handler != nullptr) {
		handler->handleGetRequest(session, request);
	}
}

void RoutingHttpWebApp::handleHeadRequest(HttpSession & session, const StringRequest & request) {
	HttpWebApp * handler = resolve(session, request);

	if (handler != nullptr) {
		handler->handleHeadRequest(session, request);
	}
}

void RoutingHttpWebApp::handlePostRequest(HttpSession & session, const StringRequest & request) {
	HttpWebApp * handler = resolve(session, request);

	if (handler != nullptr) {
		handler->handlePostRequest(session, request);
	}
}

HttpWebApp * RoutingHttpWebApp::resolve(HttpSession & session, const StringRequest & request) {
	const std::string_view & path = std::string_view(request.target().data(), request.target().length());
	auto components = Util::Strings::split(path, "/");

	Node * node = routing.findNearest(
		components, [] (auto & lhs, auto & rhs) { return std::get<KeyIndex>(lhs) == rhs; }
	);

	if (node != nullptr) {
		HttpWebApp * handler;

		switch (request.method()) {
			case Method::get: {
				handler = std::get<GetHandlerIndex>(node->value).get();
				break;
			}

			case Method::head: {
				handler = std::get<HeadHandlerIndex>(node->value).get();
				break;
			}

			case Method::post: {
				handler = std::get<PostHandlerIndex>(node->value).get();
				break;
			}

			default: {
				handler = nullptr;
				break;
			}
		}

		if (handler != nullptr) {
			return handler;
		}
	}

	// No handler found for method.

	if (request.method() == Method::head) {
		session.sendResponse(createNotFoundHeadResponse(session, request));
	} else {
		session.sendResponse(createNotFoundStringResponse(session, request));
	}

	return nullptr;
}

} // namespace Balau::Network::Http::HttpWebApps
