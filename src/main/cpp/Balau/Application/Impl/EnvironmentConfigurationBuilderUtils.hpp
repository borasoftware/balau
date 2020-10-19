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

	// Parse the properties sources and create an object trie containing property string objects.
	// The property hierarchies from all the input files are cascaded together in this function.
	public: static Container::ObjectTrie<PropertyString> createPropertyStrings(const std::vector<std::shared_ptr<Resource::Uri>> & inputs) {
		Container::ObjectTrie<PropertyString> propertyTrie;

		for (const auto & input : inputs) {
			auto properties = Lang::Property::PropertyParserService::parse(input);
			PropertyTrieCreatorPayload payload {};
			PropertyTrieCreator visitor(input);
			visitor.visit(payload, properties);
			propertyTrie.cascade(payload.trie);
		}

		return propertyTrie;
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
			auto * composite = reinterpret_cast<CompositePropertyBindingBuilderFactory *>(first.get());

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
			auto * composite = reinterpret_cast<CompositePropertyBindingBuilderFactory *>(last.get());

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
