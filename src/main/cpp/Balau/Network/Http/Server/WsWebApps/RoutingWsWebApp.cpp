// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "RoutingWsWebApp.hpp"
#include "../WsSession.hpp"

namespace Balau::Network::Http::WsWebApps {

RoutingWsWebApp::RoutingWsWebApp(Routing && routing_) : routing(std::move(routing_)) {}

void RoutingWsWebApp::handleTextMessage(WsSession & session) {
//	WsWebApp * handler = resolve(session);
//
//	if (handler != nullptr) {
//		handler->handleTextMessage(session);
//	}
}

void RoutingWsWebApp::handleBinaryMessage(WsSession & session) {
//	WsWebApp * handler = resolve(session);
//
//	if (handler != nullptr) {
//		handler->handleBinaryMessage(session);
//	}
}

void RoutingWsWebApp::handleClose(WsSession & session) {
//	WsWebApp * handler = resolve(session);
//
//	if (handler != nullptr) {
//		handler->handleClose(session);
//	}
}

void RoutingWsWebApp::handlePing(WsSession & session) {
//	WsWebApp * handler = resolve(session);
//
//	if (handler != nullptr) {
//		handler->handlePing(session);
//	}
}

void RoutingWsWebApp::handlePong(WsSession & session) {
//	WsWebApp * handler = resolve(session);
//
//	if (handler != nullptr) {
//		handler->handlePong(session);
//	}
}

WsWebApp * RoutingWsWebApp::resolve(WsSession & session) {
//	const auto & path = request.target();
//	auto components = Util::Strings::split(path, "/");
//
//	Node * node = routing.findNearest(
//		components, [] (auto & lhs, auto & rhs) { return std::get<KeyIndex>(lhs) == rhs; }
//	);
//
//	if (node != nullptr) {
//		WsWebApp * handler;
//
//		switch (request.method()) {
//			case Method::get: {
//				handler = std::get<GetHandlerIndex>(node->value).get();
//				break;
//			}
//
//			case Method::head: {
//				handler = std::get<HeadHandlerIndex>(node->value).get();
//				break;
//			}
//
//			case Method::post: {
//				handler = std::get<PostHandlerIndex>(node->value).get();
//				break;
//			}
//
//			default: {
//				handler = nullptr;
//				break;
//			}
//		}
//
//		if (handler != nullptr) {
//			return handler;
//		}
//	}
//
//	// No handler found for method.
//
//	if (request.method() == Method::head) {
//		session.sendResponse(createNotFoundHeadResponse(session, request));
//	} else {
//		session.sendResponse(createNotFoundStringResponse(session, request));
//	}

	return nullptr;
}

} // namespace Balau::Network::Http::WsWebApps
