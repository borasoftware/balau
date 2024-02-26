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

///
/// @file RoutingHttpWebApp.hpp
///
/// An HTTP web application handler that routes to other handlers.
///

#ifndef COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_ROUTING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_ROUTING_HTTP_WEB_APP

#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Network/Http/Server/HttpWebApp.hpp>

namespace Balau::Network::Http::HttpWebApps {

///
/// An HTTP web application handler that routes to other handlers.
///
/// The routing handler works by selecting the most specific handler tuple for a
/// particular path. If there is no match, not found is returned.
///
/// When the most specific handler tuple has been selected, the tuple's handler
/// for the HTTP method in the request is called.
///
/// If a most specific match is not suitable for all paths, the resolved handlers
/// may nevertheless return not found / bad request responses for invalid paths.
///
/// There are two ways to use the routing HTTP handler: automatically or manually.
///
/// The routing handler is automatically instantiated when the HTTP server is
/// created via environment configuration. In this case, the HTTP server will
/// create a populated routing handler from the HTTP web applications specified
/// in the environment configuration. The HTTP server environment configuration
/// thus specifies the routing hierarchy via the location properties in each web
/// application.
///
/// Manual creation of a routing handler may also be performed if required, by
/// constructing a routing trie with each node containing a routing node. Routing
/// nodes are std::tuple objects, the first element containing a string component
/// in the path and the second, third, and fourth elements containing shared
/// pointers of the handlers to route to for GET, HEAD, and POST requests.
///
/// As the HTTP server will assign the same handler instance to all three GET,
/// HEAD, and POST pointers for a particular location, manual creation can be
/// useful when different handlers are required for each HTTP method in a single
/// location.
///
/// To manually construct the routing trie, the routingNode() convenience functions
/// may be used. If the routing trie is fixed at compile time, the fluent API of the
/// ObjectTrie class can be used to construct the routing.
///
/// As the handlers are kept in shared pointers, handler instances may be shared
/// between multiple nodes in the trie if required.
///
class RoutingHttpWebApp : public HttpWebApp {
	///
	/// Shared pointer container for web app instances.
	///
	public: using HttpWebAppPtr = std::shared_ptr<HttpWebApp>;

	///
	/// The type of the routing node values added to the routing trie.
	///
	public: using Value = std::tuple<std::string, HttpWebAppPtr, HttpWebAppPtr, HttpWebAppPtr>;

	///
	/// The type of the routing nodes in the routing trie.
	///
	public: using Node = Container::ObjectTrieNode<Value>;

	///
	/// The type of the routing trie supplied to the routing handler constructor.
	///
	public: using Routing = Container::ObjectTrie<Value>;

	public: static constexpr size_t KeyIndex = 0;
	public: static constexpr size_t GetHandlerIndex = 1;
	public: static constexpr size_t HeadHandlerIndex = 2;
	public: static constexpr size_t PostHandlerIndex = 3;

	///
	/// Construct a routing HTTP handler, by supplying a preformed routing trie.
	///
	public: RoutingHttpWebApp(Routing && routing_) : routing(std::move(routing_)) {}

	public: void handleGetRequest(HttpSession & session,
	                              const StringRequest & request,
	                              std::map<std::string, std::string> & variables) override {
		HttpWebApp * handler = resolve(session, request);

		if (handler != nullptr) {
			handler->handleGetRequest(session, request, variables);
		}
	}

	public: void handleHeadRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override {
		HttpWebApp * handler = resolve(session, request);

		if (handler != nullptr) {
			handler->handleHeadRequest(session, request, variables);
		}
	}

	public: void handlePostRequest(HttpSession & session,
	                               const StringRequest & request,
	                               std::map<std::string, std::string> & variables) override {
		HttpWebApp * handler = resolve(session, request);

		if (handler != nullptr) {
			handler->handlePostRequest(session, request, variables);
		}
	}

	///////////////////////// Private implementation //////////////////////////

	//private: HttpWebApp * resolve(HttpSession & session, const StringRequest & request);
	private: HttpWebApp * resolve(HttpSession & session, const StringRequest & request) {
		const std::string_view & path = std::string_view(request.target().data(), request.target().length());
		auto pathComponents = Util::Strings::split(path, "/");
		std::vector<std::string_view> components;
		components.reserve(pathComponents.size() + 1);
		components.emplace_back("");
		Util::Vectors::append(components, pathComponents);

		Node * node = routing.findNearest(components, [] (auto & lhs, auto & rhs) { return std::get<KeyIndex>(lhs) == rhs; }, false);

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
		sendNotFoundResponse(session, request);
		return nullptr;
	}

	private: void sendNotFoundResponse(HttpSession & session, const StringRequest & request);

	private: Routing routing;
};

///
/// Convenience function to make a routing node from a handler type, a string
/// key, and the handler constructor arguments.
///
/// The same handler will be used for GET, HEAD, and POST requests.
///
template <typename HandlerT, typename ... ParamT>
inline RoutingHttpWebApp::Value routingNode(const std::string & key, ParamT && ... param) {
	auto handler = std::shared_ptr<HttpWebApp>(new HandlerT(std::move(param) ...));
	return RoutingHttpWebApp::Value(key, handler, handler, handler);
}

///
/// Convenience function to make routing nodes from a handler type, a string
/// key, and no handler constructor arguments.
///
/// The same handler will be used for GET, HEAD, and POST requests.
///
template <typename HandlerT>
inline RoutingHttpWebApp::Value routingNode(const std::string & key) {
	auto handler = std::shared_ptr<HttpWebApp>(new HandlerT);
	return RoutingHttpWebApp::Value(key, handler, handler, handler);
}

///
/// Convenience function to make null routing nodes.
///
inline RoutingHttpWebApp::Value routingNode(const std::string & key) {
	auto handler = std::shared_ptr<HttpWebApp>(nullptr);
	return RoutingHttpWebApp::Value(key, handler, handler, handler);
}

} // namespace Balau::Network::Http::HttpWebApps

#endif // COM_BORA_SOFTWARE_BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS_ROUTING_HTTP_WEB_APP
