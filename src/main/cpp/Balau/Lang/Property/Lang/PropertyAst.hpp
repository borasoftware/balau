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
/// @file PropertyAst.hpp
///
/// The Property parser AST node classes.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_AST
#define COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_AST

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Property/Lang/PropertyToken.hpp>
#include <Balau/Lang/Property/Util/PropertyVisitor.hpp>
#include <Balau/Lang/Common/CodeSpan.hpp>
#include <Balau/Type/Character.hpp>

#include <memory>
#include <string>
#include <vector>

namespace Balau::Lang::Property::AST {

///
/// Abstract base class of Property AST node classes.
///
class PropertyNode { // Abstract
	///
	/// Destroy a Property AST node.
	///
	public: virtual ~PropertyNode() = default;

	///
	/// Get the code span of the node.
	///
	/// @return the node's code span
	///
	public: const CodeSpan & getCodeSpan() const {
		return codeSpan;
	}

	///
	/// Returns true if the current node is equal to the supplied node.
	///
	/// Overridden in concrete classes.
	///
	/// @return true if the current node is equal to the supplied node.
	///
	public: virtual bool operator == (const PropertyNode & rhs) const {
		return codeSpan == rhs.codeSpan;
	}

	///
	/// Returns true if the current node is not equal to the supplied node.
	///
	/// Overridden in concrete classes.
	///
	/// @return true if the current node is not equal to the supplied node.
	///
	public: bool operator != (const PropertyNode & rhs) const {
		return ! operator == (rhs);
	}

	///
	/// Visit the node.
	///
	/// @payload the visitor specific payload (cast in the concrete visitor class)
	/// @param visitor the visitor
	///
	public: virtual void visit(Payload & payload, PropertyVisitor & visitor) const = 0;

	///
	/// Removing escaping, line continuation, and leading blanks.
	///
	/// This function is normally called internally, but it can be called
	/// externally if required.
	///
	/// @param text the input text
	/// @return the normalised text
	///
	public: static std::string normalise(std::string_view input) {
		// The output string will be at most the same length as the input string.
		std::string output = std::string(input);
		int inOffset = 0;
		int outOffset = 0;
		char32_t c;

		while (inOffset < (int) input.length()) {
			c = Character::getNextUtf8Safe(input, inOffset);

			if (c == U'\\') {
				if (inOffset == (int) input.length()) {
					break; // Last character is a backslash.
				}

				c = Character::getNextUtf8Safe(input, inOffset);

				switch (c) {
					case U'\r':
					case U'\n': {
						skipLineContinuation(input, inOffset, output, outOffset);
						break;
					}

					default: {
						Character::setUtf8AndAdvanceOffset(output, outOffset, c);
						break;
					}
				}

			} else {
				Character::setUtf8AndAdvanceOffset(output, outOffset, c);
			}
		}

		output.erase((size_t) outOffset);
		return output;
	}

	///////////////////////// Private implementation //////////////////////////

	public: PropertyNode(const PropertyNode & copy) = delete;
	public: PropertyNode & operator = (const PropertyNode & copy) = delete;

	protected: CodeSpan codeSpan;

	protected: explicit PropertyNode(const CodeSpan & codeSpan_)
		: codeSpan(codeSpan_) {}

	protected: PropertyNode(PropertyNode && rhs) noexcept
		: codeSpan(rhs.codeSpan) {}

	private: static void skipLineContinuation(const std::string_view & input,
	                                          int & inOffset,
	                                          std::string & output,
	                                          int & outOffset) {
		if (inOffset == (int) input.length()) {
			return; // Last character pair is an escaped CR or LF.
		}

		char32_t c = Character::getNextUtf8Safe(input, inOffset);

		if (c == U'\r' || c == U'\n') {
			if (inOffset == (int) input.length()) {
				return; // Last character triplet is an escaped CRLF or LFCR.
			}

			c = Character::getNextUtf8Safe(input, inOffset);
		}

		while (Character::isBlank(c)) {
			if (inOffset == (int) input.length()) {
				return; // End of string.
			}

			c = Character::getNextUtf8Safe(input, inOffset);
		}

		Character::setUtf8AndAdvanceOffset(output, outOffset, c);
	}
};

///
/// The outer structure. A single instance of this
/// class represents the entire parsed properties text.
///
class Properties final : public PropertyNode {
	///
	/// Create a properties node.
	///
	/// The entire input text is moved to and owned by the properties node.
	///
	public: Properties(const CodeSpan & codeSpan_,
	                   std::string && text_,
	                   std::vector<std::unique_ptr<PropertyNode>> && nodes_)
		: PropertyNode(codeSpan_)
		, text(std::move(text_))
		, nodes(std::move(nodes_)) {}

	public: const std::string & getText() const {
		return text;
	}

	public: const std::vector<std::unique_ptr<PropertyNode>> & getNodes() const {
		return nodes;
	}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const Properties *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = text == o->text
				&& PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	private: std::string text;
	private: std::vector<std::unique_ptr<PropertyNode>> nodes;
};

///
/// Simple name-value property node.
///
class ValueProperty final : public PropertyNode {
	///
	/// Create a name-value property node with the supplied data.
	///
	public: ValueProperty(const CodeSpan & codeSpan_, std::string_view name_, std::string_view value_)
		: PropertyNode(codeSpan_)
		, name(name_)
		, value(value_) {}

	///
	/// Construct a name-value property node by moving the contents of the supplied node.
	///
	public: ValueProperty(ValueProperty && rhs) noexcept
		: PropertyNode(std::move(rhs))
		, name(rhs.name)
		, value(rhs.value) {}

	///
	/// Get the string view pointing to the name of the value property.
	///
	/// @return the string view pointing to the name of the value property
	///
	public: std::string_view getName() const {
		return name;
	}

	///
	/// Get the string view pointing to the value of the value property.
	///
	/// @return the string view pointing to the value of the value property
	///
	public: std::string_view getValue() const {
		return value;
	}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const ValueProperty *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = name == o->name
				&& value == o->value
				&& PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	///////////////////////// Private implementation //////////////////////////

	public: ValueProperty() = delete;
	public: ValueProperty(const ValueProperty & copy) = delete;
	public: ValueProperty & operator = (const ValueProperty & copy) = delete;

	private: std::string_view name;
	private: std::string_view value;
};

///
/// Composite name-value property node.
///
class CompositeProperty final : public PropertyNode {
	///
	/// Create a composite property node with the supplied data.
	///
	public: CompositeProperty(const CodeSpan & codeSpan_,
	                          std::string_view name_,
	                          std::vector<std::unique_ptr<PropertyNode>> && nodes_)
		: PropertyNode(codeSpan_)
		, name(name_)
		, nodes(std::move(nodes_)) {}

	///
	/// Construct a composite property node by moving the contents of the supplied node.
	///
	public: CompositeProperty(CompositeProperty && rhs) noexcept
		: PropertyNode(std::move(rhs))
		, name(rhs.name)
		, nodes(std::move(rhs.nodes)) {}

	///
	/// Get the string view pointing to the name of the composite property.
	///
	/// @return the string view pointing to the name of the composite property
	///
	public: std::string_view getName() const {
		return name;
	}

	///
	/// Get the string view pointing to the value of the composite property.
	///
	/// @return the string view pointing to the value of the composite property
	///
	public: const std::vector<std::unique_ptr<PropertyNode>> & getNodes() const {
		return nodes;
	}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const CompositeProperty *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = name == o->name
				&& nodes == o->nodes
				&& PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	///////////////////////// Private implementation //////////////////////////

	public: CompositeProperty() = delete;
	public: CompositeProperty(const CompositeProperty & copy) = delete;
	public: CompositeProperty & operator = (const CompositeProperty & copy) = delete;

	private: std::string_view name;
	private: std::vector<std::unique_ptr<PropertyNode>> nodes;
};

///
/// Include property node.
///
class IncludePropertyNode final : public PropertyNode {
	///
	/// Create an include property node with the supplied data.
	///
	public: IncludePropertyNode(const CodeSpan & codeSpan_, std::string_view text_)
		: PropertyNode(codeSpan_)
		, text(text_) {}

	///
	/// Construct an include property node by moving the contents of the supplied node.
	///
	public: IncludePropertyNode(IncludePropertyNode && rhs) noexcept
		: PropertyNode(std::move(rhs))
		, text(rhs.text) {}

	///
	/// Get the string view pointing to the text of the include property.
	///
	/// @return the string view pointing to the text of the include property
	///
	public: std::string_view getText() const {
		return text;
	}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const IncludePropertyNode *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = text == o->text
				&& PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	///////////////////////// Private implementation //////////////////////////

	public: IncludePropertyNode() = delete;
	public: IncludePropertyNode(const IncludePropertyNode & copy) = delete;
	public: IncludePropertyNode & operator = (const IncludePropertyNode & copy) = delete;

	private: std::string_view text;
};

///
/// Comment property node.
///
class CommentPropertyNode final : public PropertyNode {
	///
	/// Create a comment property node with the supplied data.
	///
	public: CommentPropertyNode(const CodeSpan & codeSpan_, std::string_view text_)
		: PropertyNode(codeSpan_)
		, text(text_) {}

	///
	/// Construct a comment property node by moving the contents of the supplied node.
	///
	public: CommentPropertyNode(CommentPropertyNode && rhs) noexcept
		: PropertyNode(std::move(rhs))
		, text(rhs.text) {}

	///
	/// Get the string view pointing to the text of the comment property.
	///
	/// @return the string view pointing to the text of the comment property
	///
	public: std::string_view getText() const {
		return text;
	}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const CommentPropertyNode *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = text == o->text
				&& PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	///////////////////////// Private implementation //////////////////////////

	public: CommentPropertyNode() = delete;
	public: CommentPropertyNode(const CommentPropertyNode & copy) = delete;
	public: CommentPropertyNode & operator = (const CommentPropertyNode & copy) = delete;

	private: std::string_view text;
};

///
/// Represents a blank line in a property file.
///
class EmptyLinePropertyNode final : public PropertyNode {
	///
	/// Create a blank line property node with the supplied data.
	///
	public: explicit EmptyLinePropertyNode(const CodeSpan & codeSpan_)
		: PropertyNode(codeSpan_) {}

	///
	/// Construct a blank line property node by moving the contents of the supplied node.
	///
	public: EmptyLinePropertyNode(EmptyLinePropertyNode && rhs) noexcept
		: PropertyNode(std::move(rhs)) {}

	public: bool operator == (const PropertyNode & rhs) const override {
		auto * o = dynamic_cast<const EmptyLinePropertyNode *>(&rhs);

		bool ret;
		if (o == nullptr) {
			ret = false;
		} else {
			ret = PropertyNode::operator == (rhs);
		}

		return ret;
	}

	public: void visit(Payload & payload, PropertyVisitor & visitor) const override {
		visitor.visit(payload, *this);
	}

	///////////////////////// Private implementation //////////////////////////

	public: EmptyLinePropertyNode() = delete;
	public: EmptyLinePropertyNode(const EmptyLinePropertyNode & copy) = delete;
	public: EmptyLinePropertyNode & operator = (const EmptyLinePropertyNode & copy) = delete;
};

} // namespace Balau::Lang::Property::AST

#endif // COM_BORA_SOFTWARE__BALAU_LANG_PROPERTY_LANG__PROPERTY_AST
