// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

///
/// @file RoutingHttpWebApp.hpp
///
/// An HTTP web application handler that routes to other handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_HTTP_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_HTTP_WEB_APP

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
/// In order to use the routing HTTP handler, construct a routing trie, with each
/// node containing a routing node. Routing nodes are std::tuple objects, the first
/// element containing a string component in the path and the second, third, and
/// fourth elements containing shared pointers of the handlers to route to for GET,
/// HEAD, and POST requests.
///
/// To construct the routing trie, the routingNode() convenience functions may be
/// used. If the routing trie is fixed at compile time, the fluent API of the
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

	///
	/// Construct a routing HTTP handler, by supplying a preformed routing trie.
	///
	public: explicit RoutingHttpWebApp(Routing && routing_);

	public: void handleGetRequest(HttpSession & session, const StringRequest & request) override;

	public: void handleHeadRequest(HttpSession & session, const StringRequest & request) override;

	public: void handlePostRequest(HttpSession & session, const StringRequest & request) override;

	///////////////////////// Private implementation //////////////////////////

	private: HttpWebApp * resolve(HttpSession & session, const StringRequest & request);

	private: static constexpr size_t KeyIndex = 0;
	private: static constexpr size_t GetHandlerIndex = 1;
	private: static constexpr size_t HeadHandlerIndex = 2;
	private: static constexpr size_t PostHandlerIndex = 3;

	private: Routing routing;
};

///
/// Builds a routing
///
class RoutingBuilder {

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

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_HTTP_WEB_APPS__ROUTING_HTTP_WEB_APP
