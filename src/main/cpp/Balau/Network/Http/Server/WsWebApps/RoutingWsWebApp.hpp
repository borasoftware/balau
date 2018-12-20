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
/// @file RoutingWsWebApp.hpp
///
/// A WebSocket web application handler that routes to other handlers.
///

#ifndef COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ROUTING_WS_WEB_APP
#define COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ROUTING_WS_WEB_APP

#include <Balau/Container/ObjectTrie.hpp>
#include <Balau/Network/Http/Server/WsWebApp.hpp>

namespace Balau::Network::Http::WsWebApps {

///
/// A WebSocket web application handler that routes to other handlers.
///
/// The routing handler works by selecting the most specific handler tuple for a
/// particular path. If there is no match, not found is returned.
///
/// When the most specific handler tuple has been selected, the tuple's handler
/// for the appropriate WebSocket message type is called.
///
/// If a most specific match is not suitable for all paths, the resolved handlers
/// may nevertheless return not found / bad request responses for invalid paths.
///
/// In order to use the routing WebSocket handler, construct a routing trie, with each
/// node containing a routing node. Routing nodes are std::tuple objects, the first
/// element containing a string component in the path and the second, third, and
/// fourth elements containing shared pointers of the handlers to route to for TODO TODO.
///
/// To construct the routing trie, the routingNode() convenience functions may be
/// used. If the routing trie is fixed at compile time, the fluent API of the
/// ObjectTrie class can be used to construct the routing.
///
/// As the handlers are kept in shared pointers, handler instances may be shared
/// between multiple nodes in the trie if required.
///
class RoutingWsWebApp : public WsWebApp {
	///
	/// Shared pointer container for WebSocket web app instances.
	///
	public: using WsWebAppPtr = std::shared_ptr<WsWebApp>;

	///
	/// The type of the routing node values added to the routing trie.
	///
	public: using Value = std::tuple<std::string, WsWebAppPtr, WsWebAppPtr, WsWebAppPtr>;

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
	public: explicit RoutingWsWebApp(Routing && routing_);

	//////////////////////////////// Handlers /////////////////////////////////

	public: void handleTextMessage(WsSession & session, std::string_view path) override;

	public: void handleBinaryMessage(WsSession & session, std::string_view path) override;

	public: void handleClose(WsSession & session, std::string_view path) override;

	public: void handlePing(WsSession & session, std::string_view path) override;

	public: void handlePong(WsSession & session, std::string_view path) override;

	///////////////////////// Private implementation //////////////////////////

	private: WsWebApp * resolve(WsSession & session, std::string_view path);

	private: static constexpr size_t KeyIndex = 0;
	private: static constexpr size_t ControlHandlerIndex = 1;
	private: static constexpr size_t TextHandlerIndex = 2;

	private: Routing routing;
};

///
/// Convenience function to make a routing node from a handler type, a string
/// key, and the handler constructor arguments.
///
/// The same handler will be used for GET, HEAD, and POST requests.
///
template <typename HandlerT, typename ... ParamT>
inline RoutingWsWebApp::Value routingNode(const std::string & key, ParamT && ... param) {
	auto handler = std::shared_ptr<WsWebApp>(new HandlerT(std::move(param) ...));
	return RoutingWsWebApp::Value(key, handler, handler, handler);
}

///
/// Convenience function to make routing nodes from a handler type, a string
/// key, and no handler constructor arguments.
///
/// The same handler will be used for GET, HEAD, and POST requests.
///
template <typename HandlerT>
inline RoutingWsWebApp::Value routingNode(const std::string & key) {
	auto handler = std::shared_ptr<WsWebApp>(new HandlerT);
	return RoutingWsWebApp::Value(key, handler, handler, handler);
}

///
/// Convenience function to make null routing nodes.
///
inline RoutingWsWebApp::Value routingNode(const std::string & key) {
	auto handler = std::shared_ptr<WsWebApp>(nullptr);
	return RoutingWsWebApp::Value(key, handler, handler, handler);
}

} // namespace Balau::Network::Http::WsWebApps

#endif // COM_BORA_SOFTWARE__BALAU_NETWORK_HTTP_SERVER_WS_WEB_APPS__ROUTING_WS_WEB_APP
