// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_UTILS
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_UTILS

#include <Balau/Application/EnvironmentProperties.hpp>
#include <Balau/Application/Impl/PropertyTypeSpecificationVisitor.hpp>
#include <Balau/Util/Containers.hpp>

namespace Balau {

namespace Impl {

// For testing.
struct EnvironmentConfigurationBuilderTest;

//
// Utility methods for environment configuration building.
//
class EnvironmentConfigurationBuilderUtils {
	private: class PropertyTrieCreatorPayload : public Lang::Property::Payload {
		public: Container::ObjectTrie<PropertyString> trie;
		public: std::stack<Container::ObjectTrieNode<PropertyString> *> nodeStack;

		public: PropertyTrieCreatorPayload() {
			nodeStack.push(&trie.root());
		}
	};

	private: class PropertyTrieCreator : public Lang::Property::PropertyVisitor {
		public: PropertyTrieCreator(const std::shared_ptr<Resource::Uri> & uri_)
			: uri(uri_) {}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::Properties & object) override {
			for (auto & node : object.getNodes()) {
				node->visit(payload, *this);
			}
		}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::ValueProperty & object) override {
			auto & pl = static_cast<PropertyTrieCreatorPayload &>(payload);
			pl.nodeStack.top()->add(PropertyString(std::string(object.getName()), std::string(object.getValue())));
		}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CompositeProperty & object) override {
			auto & pl = static_cast<PropertyTrieCreatorPayload &>(payload);
			auto & node = pl.nodeStack.top()->addAndReturnChild(PropertyString(std::string(object.getName())));
			pl.nodeStack.push(&node);

			for (auto & childNode : object.getNodes()) {
				childNode->visit(payload, *this);
			}

			pl.nodeStack.pop();
		}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::IncludePropertyNode & object) override {
			std::shared_ptr<Resource::Uri> resolvedUri = uri->resolve(Util::Strings::trim(object.getText()));
			auto properties = Lang::Property::PropertyParserService::parse(resolvedUri);
			visit(payload, properties);
		}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::CommentPropertyNode & object) override {
			// NOP
		}

		public: void visit(Lang::Property::Payload & payload, const Lang::Property::AST::EmptyLinePropertyNode & object) override {
			// NOP
		}

		private: const std::shared_ptr<Resource::Uri> uri;
	};

	// Parse the properties source and create an object trie containing property string objects.
	public: static Container::ObjectTrie<PropertyString> createPropertyStrings(const std::shared_ptr<Resource::Uri> & input) {
		auto properties = Lang::Property::PropertyParserService::parse(input);
		PropertyTrieCreatorPayload payload {};
		PropertyTrieCreator visitor(input);
		visitor.visit(payload, properties);
		return payload.trie;
	}

	// Utility method: adds the children to the parent.
	public: template <typename ... BindingBuilderFactoryPtrT>
	static void addToComposite(CompositePropertyBindingBuilderFactory & composite,
	                           PropertyBindingBuilderFactoryPtr & firstChild,
	                           BindingBuilderFactoryPtrT & ... remainingChildren) {
		composite.children.emplace_back(firstChild);
		addToComposite(composite, remainingChildren ...);
	}

	// Adds the children to the parent (end case).
	public: static void addToComposite(CompositePropertyBindingBuilderFactory & composite,
	                                   PropertyBindingBuilderFactoryPtr & lastChild) {
		composite.children.emplace_back(lastChild);
	}

	// Increments the levels of the tokens during configuration prep.
	public: template <typename ... BindingBuilderFactoryPtrT>
	static void incrementLevel(PropertyBindingBuilderFactoryPtr & first, BindingBuilderFactoryPtrT & ... remainder) {
		++first->level;

		if (first->isComposite()) {
			auto composite = std::reinterpret_pointer_cast<CompositePropertyBindingBuilderFactory>(first);

			std::for_each(
				  composite->children.begin()
				, composite->children.end()
				, [] (auto & child) { incrementLevel(child); }
			);
		}

		incrementLevel(remainder ...);
	}

	// Increments the levels of the tokens during configuration prep (end case).
	public: static void incrementLevel(PropertyBindingBuilderFactoryPtr & last) {
		++last->level;

		if (last->isComposite()) {
			auto composite = std::reinterpret_pointer_cast<CompositePropertyBindingBuilderFactory>(last);

			std::for_each(
				  composite->children.begin()
				, composite->children.end()
				, [] (auto & child) { incrementLevel(child); }
			);
		}
	}

	private: EnvironmentConfigurationBuilderUtils() = delete;
	private: EnvironmentConfigurationBuilderUtils(const EnvironmentConfigurationBuilderUtils & ) = delete;
	private: EnvironmentConfigurationBuilderUtils & operator = (const EnvironmentConfigurationBuilderUtils & ) = delete;
};

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER_UTILS
