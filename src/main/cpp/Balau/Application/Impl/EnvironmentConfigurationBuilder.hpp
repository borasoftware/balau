// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER

#include <Balau/Application/Impl/EnvironmentConfigurationBuilderUtils.hpp>

namespace Balau {

class EnvironmentConfiguration;

namespace Impl {

//
// The environment configuration build() method delegates to the build method of this class.
//
class EnvironmentConfigurationBuilder {
	public: using PropertyStringHierarchy = Container::ObjectTrie<Impl::PropertyString>;
	public: using PropertyBindingBuilderFactoryPtrVector = std::vector<Impl::PropertyBindingBuilderFactoryPtr>;
	public: using BuilderVector = std::vector<std::shared_ptr<Impl::BindingBuilderBase>>;

	friend class ::Balau::EnvironmentConfiguration;
	friend struct EnvironmentConfigurationBuilderTest;

	// The main build method. Run after the configure() method by the base class.
	private: static BuilderVector build(const std::vector<std::shared_ptr<Resource::Uri>> & typeSpecificationsUris,
	                                    const PropertyBindingBuilderFactoryPtrVector & derivedBindingBuilderFactoryVector,
	                                    const PropertyStringHierarchy & propertyStringsHierarchy) {
		PropertyBindingBuilderFactoryPtrVector bindingBuilderFactoryVector;

		// Create factories for all type specification source files.
		for (auto & uri : typeSpecificationsUris) {
			Impl::PropertyTypeSpecificationVisitorPayload payload;
			Impl::PropertyTypeSpecificationVisitor visitor(uri);
			visitor.execute(payload);

			for (auto & factory : payload.bindingBuilderFactoriesVector) {
				appendFactories(bindingBuilderFactoryVector, factory);
			}
		}

		// Transfer the factories from hard wired type specifications.
		for (auto & factory : derivedBindingBuilderFactoryVector) {
			appendFactories(bindingBuilderFactoryVector, factory);
		}

		//
		// At this point the following data is available:
		//  - propertyStringsHierarchy;
		//  - bindingBuilderFactoryVector.
		//
		// The property string trie represents the environment's value property values.
		//
		// The binding builder factory vector represents the root type specifications
		// supplied either via type specification files or hard wired in the configure
		// methods of the derived class.
		//
		// Each of the entries from the property strings hierarchy will be converted
		// into a typed binding if a corresponding binding builder factory is defined
		// or an untyped binding otherwise.
		//
		// Subsequently, each of the type specifications that have a default value
		// and have not been used already to create a binding will be converted
		// into a typed binding.
		//

		BuilderVector builders;
		Util::Vectors::move(builders, buildProvidedValues(bindingBuilderFactoryVector, propertyStringsHierarchy));
		Util::Vectors::move(builders, buildDefaultValues(builders, bindingBuilderFactoryVector));
		return builders;
	}

	//
	// Create the final builder factory hierarchy before building.
	// This call is for the root level.
	//
	private: static void appendFactories(PropertyBindingBuilderFactoryPtrVector & bindingBuilderFactoryVector,
	                                     const Impl::PropertyBindingBuilderFactoryPtr & factory) {
		if (!factory->isComposite()) {
			// Simple factory.

			Util::Vectors::pushBackOrReplace(
				  bindingBuilderFactoryVector
				, factory
				, [] (const auto & lhs, const auto & rhs) { return lhs->name == rhs->name; }
			);

		} else {
			// Composite factory.

			const auto & compositeFactory = *static_cast<const CompositePropertyBindingBuilderFactory *>(factory.get());

			// Don't create an empty composite child.
			if (!compositeFactory.children.empty()) {
				Util::Vectors::pushBackOrReplace(
					  bindingBuilderFactoryVector
					, factory
					, [] (const auto & lhs, const auto & rhs) { return lhs->name == rhs->name; }
					, [] (auto & dst, const auto & src) { EnvironmentConfigurationBuilder::cascade(dst, src); }
				);
			}
		}
	}

	//
	// Cascading of a source composite builder onto an existing destination composite builder.
	// This is a recursive operation.
	//
	private: static void cascade(Impl::PropertyBindingBuilderFactoryPtr & dst,
	                             const Impl::PropertyBindingBuilderFactoryPtr & src) {
		auto & dstComposite = *static_cast<CompositePropertyBindingBuilderFactory *>(dst.get());
		const auto & srcComposite = *static_cast<const CompositePropertyBindingBuilderFactory *>(src.get());

		for (const auto & srcChildFactory : srcComposite.children) {
			if (!srcChildFactory->isComposite()) {
				// Simple factory.

				auto existingDstIter = std::find_if(
					  dstComposite.children.begin()
					, dstComposite.children.end()
					, [&srcChildFactory] (const auto & dstChildFactory) { return dstChildFactory->name == srcChildFactory->name; }
				);

				if (existingDstIter != dstComposite.children.end()) {
					if ((*existingDstIter)->isComposite()) {
						// TODO propagate the name hierarchy through the lambda for error reporting.
						ThrowBalauException(
							  Exception::EnvironmentConfigurationException
							, "Incompatible type specifications found."
						);
					}

					*existingDstIter = srcChildFactory;
				} else {
					dstComposite.children.emplace_back(srcChildFactory);
				}
			} else {
				// Composite factory.

				const auto & srcChildCompositeFactory = *static_cast<const CompositePropertyBindingBuilderFactory *>(
					srcChildFactory.get()
				);

				if (srcChildCompositeFactory.children.empty()) {
					// Don't create an empty composite child.
					continue;
				}

				auto existingDstIter = std::find_if(
					  dstComposite.children.begin()
					, dstComposite.children.end()
					, [&srcChildFactory] (const auto & dstChildFactory) { return dstChildFactory->name == srcChildFactory->name; }
				);

				if (existingDstIter != dstComposite.children.end()) {
					if (!(*existingDstIter)->isComposite()) {
						// TODO propagate the name hierarchy through the lambda for error reporting.
						ThrowBalauException(
							  Exception::EnvironmentConfigurationException
							, "Incompatible type specifications found."
						);
					}

					// Recurse.
					cascade(*existingDstIter, srcChildFactory);
				} else {
					dstComposite.children.emplace_back(srcChildFactory);
				}
			}
		}
	}

	// A binding builder for environment properties instances.
	private: class EnvironmentPropertiesBuilder : public Impl::BindingBuilderBase {
		public: EnvironmentPropertiesBuilder(std::string name, std::shared_ptr<EnvironmentProperties> & instance_)
			: BindingBuilderBase(std::move(name))
			, instance(std::move(instance_)) {
			setKeyType<Impl::BindingKeyType<Impl::BindingMetaType::Shared, EnvironmentProperties>>();
		}

		private: std::unique_ptr<Impl::AbstractBinding> build() override {
			return std::unique_ptr<Impl::AbstractBinding>(
				new Impl::ProvidedSingletonBinding<EnvironmentProperties>(std::move(key), instance)
			);
		}

		friend class EnvironmentConfigurationBuilder;

		private: std::shared_ptr<EnvironmentProperties> instance;
	};

	// Create the builders for the root injector level from property values.
	private: static BuilderVector buildProvidedValues(const PropertyBindingBuilderFactoryPtrVector & bindingBuilderFactoryVector,
	                                                  const PropertyStringHierarchy & propertyStringsHierarchy) {
		BuilderVector newBuilders;

		std::deque<const Container::ObjectTrieNode<Impl::PropertyString> *> nodeHierarchy;
		const Container::ObjectTrieNode<Impl::PropertyString> & node = propertyStringsHierarchy.root();

		for (size_t m = 0; m < node.count(); m++) {
			const auto & childNode = node[m];
			const auto & object = childNode.value;
			const auto & name = object.name;

			nodeHierarchy.push_back(&childNode);

			auto factoryNodeIter = std::find_if(
				  bindingBuilderFactoryVector.begin()
				, bindingBuilderFactoryVector.end()
				, [&name] (const auto & ptr) { return ptr->name == name; }
			);

			switch (object.type) {
				case Impl::PropertyString::Type::Value: {
					if (factoryNodeIter != bindingBuilderFactoryVector.end()) {
						newBuilders.emplace_back((*factoryNodeIter)->create(object.value));
					} else {
						newBuilders.emplace_back(Impl::ValuePropertyBindingBuilderFactory<std::string>{object.name}.create(object.value));
					}

					break;
				}

				case Impl::PropertyString::Type::Composite: {
					if (factoryNodeIter != bindingBuilderFactoryVector.end() && !(*factoryNodeIter)->isComposite()) {
						Impl::InjectorLogger::log.warn(
							"Typed property factory declaration found for composite environment configuration"
							"node (property name hierarchy = {}). Please remove this typed property factory "
							"declaration from the associated environment configuration class in order to avoid "
							"this warning message."
							, ::toString(
								Util::Containers::map<std::string, const Container::ObjectTrieNode<Impl::PropertyString> *>(
									nodeHierarchy, [] (const Container::ObjectTrieNode<Impl::PropertyString> * node) {
										return node->value.name;
									}
								)
							)
						);
					}

					if (factoryNodeIter != bindingBuilderFactoryVector.end()) {
						auto childLevel = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);
						auto & ptr = *factoryNodeIter;
						const auto & compositeBindingBuilderFactory = *static_cast<const CompositePropertyBindingBuilderFactory *>(ptr.get());
						Util::Vectors::move(newBuilders, buildProvidedValues(compositeBindingBuilderFactory.children, childLevel, nodeHierarchy));
						auto instance = std::shared_ptr<EnvironmentProperties>(new EnvironmentProperties(std::move(childLevel)));
						newBuilders.emplace_back(std::shared_ptr<Impl::BindingBuilderBase>(new EnvironmentPropertiesBuilder(childNode.value.name, instance)));
					} else {
						// No composite factory is defined. All descendant value properties will be created as strings.
						auto childLevel = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);
						Util::Vectors::move(newBuilders, buildProvidedValues(PropertyBindingBuilderFactoryPtrVector(), childLevel, nodeHierarchy));
						auto instance = std::shared_ptr<EnvironmentProperties>(new EnvironmentProperties(std::move(childLevel)));
						newBuilders.emplace_back(std::shared_ptr<Impl::BindingBuilderBase>(new EnvironmentPropertiesBuilder(childNode.value.name, instance)));
					}

					break;
				}

				case Impl::PropertyString::Type::NotSet: {
					ThrowBalauException(
						Exception::BugException
					, "Unset property string type occurred during environment configuration processing."
					);
				}
			}

			nodeHierarchy.pop_back();
		}

		return newBuilders;
	}

	// Create the builders for an intermediate level from property values.
	private: static BuilderVector buildProvidedValues(const PropertyBindingBuilderFactoryPtrVector & bindingBuilderFactoryVector,
	                                                  std::unique_ptr<Impl::BindingMap> & level,
	                                                  std::deque<const Container::ObjectTrieNode<Impl::PropertyString> *> & nodeHierarchy) {
		BuilderVector newBuilders;

		const auto & node = *nodeHierarchy.back();

		for (size_t m = 0; m < node.count(); m++) {
			const auto & childNode = node[m];
			const auto & object = childNode.value;
			const auto & name = object.name;

			nodeHierarchy.push_back(&childNode);

			auto factoryNodeIter = std::find_if(
				  bindingBuilderFactoryVector.begin()
				, bindingBuilderFactoryVector.end()
				, [&name] (const auto & ptr) { return ptr->name == name; }
			);

			switch (object.type) {
				case Impl::PropertyString::Type::Value: {
					if (factoryNodeIter == bindingBuilderFactoryVector.end()) {
						auto builder = Impl::ValuePropertyBindingBuilderFactory<std::string>{object.name}.create(object.value);
						auto key = builder->getKey();
						level->put(key, builder->build());
					} else {
						auto builder = (*factoryNodeIter)->create(object.value);
						auto key = builder->getKey();
						level->put(key, builder->build());
					}

					break;
				}

				case Impl::PropertyString::Type::Composite: {
					if (factoryNodeIter != bindingBuilderFactoryVector.end() && !(*factoryNodeIter)->isComposite()) {
						Impl::InjectorLogger::log.warn(
							"Typed property factory declaration found for composite environment configuration"
							"node (property name hierarchy = {}). Please remove this typed property factory "
							"declaration from the associated environment configuration class in order to avoid "
							"this warning message."
							, ::toString(
								Util::Containers::map<std::string, const Container::ObjectTrieNode<Impl::PropertyString> *>(
									nodeHierarchy, [] (const Container::ObjectTrieNode<Impl::PropertyString> * node) {
										return node->value.name;
									}
								)
							)
						);
					}

					if (factoryNodeIter != bindingBuilderFactoryVector.end()) {
						auto childLevel = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);
						auto & ptr = *factoryNodeIter;
						const auto & compositeBindingBuilderFactory = *static_cast<const CompositePropertyBindingBuilderFactory *>(ptr.get());
						Util::Vectors::move(newBuilders, buildProvidedValues(compositeBindingBuilderFactory.children, childLevel, nodeHierarchy));
						auto instance = std::shared_ptr<EnvironmentProperties>(new EnvironmentProperties(std::move(childLevel)));
						auto builder = std::shared_ptr<Impl::BindingBuilderBase>(new EnvironmentPropertiesBuilder(childNode.value.name, instance));
						auto key = builder->getKey();
						level->put(key, builder->build());
					} else {
						// No composite factory is defined. All descendant value properties will be created as strings.
						auto childLevel = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);
						Util::Vectors::move(newBuilders, buildProvidedValues(PropertyBindingBuilderFactoryPtrVector(), childLevel, nodeHierarchy));
						auto instance = std::shared_ptr<EnvironmentProperties>(new EnvironmentProperties(std::move(childLevel)));
						auto builder = std::shared_ptr<Impl::BindingBuilderBase>(new EnvironmentPropertiesBuilder(childNode.value.name, instance));
						auto key = builder->getKey();
						level->put(key, builder->build());
					}

					break;
				}

				case Impl::PropertyString::Type::NotSet: {
					ThrowBalauException(
						Exception::BugException
					, "Unset property string type occurred during environment configuration processing."
					);
				}
			}

			nodeHierarchy.pop_back();
		}

		return newBuilders;
	}

	// Create the builders for the root injector level from default values.
	private: static BuilderVector buildDefaultValues(BuilderVector & builders,
	                                                 const PropertyBindingBuilderFactoryPtrVector & bindingBuilderFactoryVector) {
		BuilderVector newBuilders;

		std::deque<std::string> nameHierarchy;

		for (size_t m = 0; m < bindingBuilderFactoryVector.size(); m++) {
			const auto & factory = bindingBuilderFactoryVector[m];

			if (!factory->isComposite()) {
				if (factory->shouldBuildDefaultBinding()) {
					newBuilders.emplace_back(factory->createDefault());
				}
			} else {
				nameHierarchy.push_back(factory->name);
				Util::Vectors::move(newBuilders, buildDefaultValues(builders, nameHierarchy, factory));
				nameHierarchy.pop_back();
			}
		}

		return newBuilders;
	}

	// Create the builders for an intermediate level from default values.
	private: static BuilderVector buildDefaultValues(BuilderVector & builders,
	                                                 std::deque<std::string> & nameHierarchy,
	                                                 const Impl::PropertyBindingBuilderFactoryPtr & f) {
		BuilderVector newBuilders;

		const auto & compositeFactory = *static_cast<const CompositePropertyBindingBuilderFactory *>(f.get());

		for (size_t m = 0; m < compositeFactory.children.size(); m++) {
			const auto & factory = compositeFactory.children[m];

			if (!factory->isComposite()) {
				if (factory->shouldBuildDefaultBinding()) {
					// The corresponding environment properties instance needs to be found
					// or a builder needs to be created for it (and any missing intermediate
					// levels) if no environment properties instance is found.

					const auto & firstName = nameHierarchy.front();

					auto first = std::find_if(
						  builders.begin()
						, builders.end()
						, [&firstName] (const auto & builder) { return builder->getKey().getName() == firstName; }
					);

					EnvironmentProperties * compositeInstance = nullptr;

					if (first != builders.end()) {
						auto & ptr = *first;
						auto * builder = dynamic_cast<EnvironmentPropertiesBuilder *>(ptr.get());

						if (!builder) {
							ThrowBalauException(Exception::BugException, "Could not find EnvironmentPropertiesBuilder.");
						}

						compositeInstance = builder->instance.get();
					} else {
						// Create a new environment properties instance.
						auto level = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);

						std::shared_ptr<EnvironmentProperties> newCompositeInstance = std::shared_ptr<EnvironmentProperties>(
							new EnvironmentProperties(std::move(level))
						);

						compositeInstance = newCompositeInstance.get();

						builders.emplace_back(
							std::shared_ptr<Impl::BindingBuilderBase>(
								new EnvironmentPropertiesBuilder(firstName, newCompositeInstance)
							)
						);
					}

					auto iter = nameHierarchy.begin();
					++iter; // Skip the first one that has already been processed.

					while (iter != nameHierarchy.end()) {
						const auto & name = *iter;

						if (compositeInstance->hasComposite(name)) {
							compositeInstance = compositeInstance->getComposite(name).get();
						} else {
							// Create a new environment properties instance.
							auto childLevel = std::unique_ptr<Impl::BindingMap>(new Impl::BindingMap);

							auto newCompositeInstance = std::shared_ptr<EnvironmentProperties>(
								new EnvironmentProperties(std::move(childLevel))
							);

							auto binding = std::unique_ptr<Impl::AbstractBinding>(
								new Impl::ProvidedSingletonBinding<EnvironmentProperties>(
									Impl::BindingKey::createSharedKey<EnvironmentProperties>(name), newCompositeInstance
								)
							);

							auto * bindingMap = const_cast<Impl::BindingMap *>(compositeInstance->bindings.get());
							compositeInstance = newCompositeInstance.get();
							bindingMap->put(Impl::BindingKey::createSharedKey<EnvironmentProperties>(name), std::move(binding));
						}

						++iter;
					}

					auto * bindingMap = const_cast<Impl::BindingMap *>(compositeInstance->bindings.get());
					bindingMap->put(factory->createKey(), factory->createDefault()->build());
				}
			} else {
				nameHierarchy.push_back(factory->name);
				Util::Vectors::move(newBuilders, buildDefaultValues(builders, nameHierarchy, factory));
				nameHierarchy.pop_back();
			}
		}

		return newBuilders;
	}

	private: EnvironmentConfigurationBuilder() = delete;
	private: EnvironmentConfigurationBuilder(const EnvironmentConfigurationBuilder & ) = delete;
	private: EnvironmentConfigurationBuilder & operator = (const EnvironmentConfigurationBuilder & ) = delete;
};

} // namespace Impl

} // namespace Balau

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__ENVIRONMENT_CONFIGURATION_BUILDER
