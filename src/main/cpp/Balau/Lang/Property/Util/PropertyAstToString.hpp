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
/// @file PropertyAstToString.hpp
///
/// Visitor based property AST to-string function.
///

#ifndef COM_BORA_SOFTWARE__BALAU_LANG_CSS_UTIL__PROPERTY_AST_TO_STRING
#define COM_BORA_SOFTWARE__BALAU_LANG_CSS_UTIL__PROPERTY_AST_TO_STRING

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Lang/Property/Lang/PropertyAst.hpp>

namespace Balau::Lang::Property::AST {

///
/// The payload used in the property AST toString implementation.
///
class PropertyAstToStringPayload : public Payload {
	private: const std::string indentString = "\t";

	private: std::ostream & builder;
	private: size_t indentCount = 0;

	public: explicit PropertyAstToStringPayload(std::ostream & stream) : builder(stream) {}

	public: void write(const std::string_view & text) {
		builder << text;
	}

	public: void incrementIndent() {
		++indentCount;
	}

	public: void decrementIndent() {
		--indentCount;
	}

	public: void writeIndent() {
		for (size_t m = 0; m < indentCount; m++) {
			builder << indentString;
		}
	}
};

///
/// The property AST visitor used in the property AST toString implementation.
///
class PropertyAstToString : public PropertyVisitor {
	public: void visit(Payload & payload, const Properties & object) override {
		for (auto & node : object.getNodes()) {
			node->visit(payload, *this);
		}
	}

	public: void visit(Payload & payload, const ValueProperty & object) override {
		auto & pl = static_cast<PropertyAstToStringPayload &>(payload);
		pl.writeIndent();
		pl.write(object.getName());
		pl.write(" = ");
		pl.write(object.getValue());
		pl.write("\n");
	}

	public: void visit(Payload & payload, const CompositeProperty & object) override {
		auto & pl = static_cast<PropertyAstToStringPayload &>(payload);
		pl.writeIndent();
		pl.write(object.getName());
		pl.write(" {\n");
		pl.incrementIndent();

		for (const auto & node : object.getNodes()) {
			node->visit(payload, *this);
		}

		pl.decrementIndent();
		pl.writeIndent();
		pl.write("}\n");
	}

	public: void visit(Payload & payload, const IncludePropertyNode & object) override {
		auto & pl = static_cast<PropertyAstToStringPayload &>(payload);
		pl.writeIndent();
		pl.write("@");
		pl.write(object.getText());
		pl.write("\n");
	}

	public: void visit(Payload & payload, const CommentPropertyNode & object) override {
		auto & pl = static_cast<PropertyAstToStringPayload &>(payload);
		pl.writeIndent();
		pl.write("#");
		pl.write(object.getText());
		pl.write("\n");
	}

	public: void visit(Payload & payload, const EmptyLinePropertyNode & object) override {
		auto & pl = static_cast<PropertyAstToStringPayload &>(payload);
		pl.write("\n");
	}
};

///
/// Pretty print the supplied AST to the output stream.
///
inline std::ostream & operator << (std::ostream & stream, const PropertyNode & object) {
	PropertyAstToStringPayload payload(stream);
	PropertyAstToString astToString;
	object.visit(payload, astToString);
	return stream;
}

///
/// The property AST toString function.
///
/// Pretty print the supplied AST and return as a UTF-8 string.
///
/// @return a string containing the represented properties
///
inline std::string toString(const PropertyNode & object) {
	std::ostringstream stream;
	stream << object;
	return stream.str();
}

///
/// The property AST toString<AllocatorT> function.
///
/// Pretty print the supplied AST and return as a UTF-8 string.
///
/// @return a string containing the represented properties
///
template <typename AllocatorT>
inline Balau::U8String<AllocatorT> toString(const PropertyNode & object) {
	U8OStringStream<AllocatorT> stream;
	stream << object;
	return stream.str();
}

} // namespace Balau::Lang::Property::AST

#endif // COM_BORA_SOFTWARE__BALAU_LANG_CSS_UTIL__PROPERTY_AST_TO_STRING
