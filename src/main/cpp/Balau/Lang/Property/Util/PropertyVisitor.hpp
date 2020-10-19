// @formatter:off
//
// Balau core C++ library
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
/// @file PropertyVisitor.hpp
///
/// Visitor interface for property AST nodes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_UTIL__PROPERTY_VISITOR
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_UTIL__PROPERTY_VISITOR

namespace Balau::Lang::Property {

namespace AST {

class Properties;
class ValueProperty;
class CompositeProperty;
class IncludePropertyNode;
class CommentPropertyNode;
class EmptyLinePropertyNode;

} // namespace AST

///
/// Base class of property visitor payloads.
///
/// There are no templated virtual methods in C++, so dispatchers must
/// ensure they downcast correctly.
///
class Payload {
	///
	/// Destroy the payload instance.
	///
	public: virtual ~Payload() = default;
};

///
/// Visitor interface for property AST nodes.
///
class PropertyVisitor {
	///
	/// Visit a Properties node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::Properties & object) = 0;

	///
	/// Visit a value property node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::ValueProperty & object) = 0;

	///
	/// Visit a composite property node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::CompositeProperty & object) = 0;

	///
	/// Visit an include property node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::IncludePropertyNode & object) = 0;

	///
	/// Visit a comment property node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::CommentPropertyNode & object) = 0;

	///
	/// Visit a blank line property node.
	///
	/// @param payload the payload of the concrete visitor class
	/// @param object the node
	///
	public: virtual void visit(Payload & payload, const AST::EmptyLinePropertyNode & object) = 0;
};

} // namespace Balau::Lang::Property

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_UTIL__PROPERTY_VISITOR
