// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_KEY
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_KEY

#include <Balau/Exception/BalauException.hpp>
#include <Balau/Util/Enums.hpp>
#include <Balau/Util/Strings.hpp>

#include <typeindex>

namespace Balau {

namespace Impl {

class BindingBuilderBase;

//
// A meta type used in the binding key.
//
enum class BindingMetaType : unsigned int {
	// Non-polymorphic stack based new instance bindings.
	Value = 0

	// Polymorphic heap based new instance bindings.
	, Unique = 1

	// Provided reference bindings.
	, Reference = 2

	// Singleton and thread-local singleton bindings.
	, Shared = 3

	// Temporary meta-type promoted to Shared during injector instantiation.
	, WeakPromotion = 4

	// Temporary meta-type indicating that the binding meta-type is not yet set.
	, Unset = 5
};

template <typename AllocatorT>
inline U8String<AllocatorT> toString(const BindingMetaType metaType) {
	switch (metaType) {
		case BindingMetaType::Value:         return "Value";
		case BindingMetaType::Unique:        return "Unique";
		case BindingMetaType::Reference:     return "Reference";
		case BindingMetaType::Shared:        return "Shared";
		case BindingMetaType::WeakPromotion: return "WeakPromotion";
		case BindingMetaType::Unset:         return "Unset";

		default: ThrowBalauException(Exception::BugException, "Unhandled BindingMetaType token in switch statement.");
	}
}

inline std::string toString(const BindingMetaType metaType) {
	return toString<std::allocator<char>>(metaType);
}

///////////////////////////////////////////////////////////////////////////////

//
// Instantiations of this template class are used in the binding key in order
// to encapsulate the const qualifier and the meta-type information inside a
// typeid field.
//

template <BindingMetaType metaType, typename T> struct BindingKeyType {
	static const bool isConst;
};

template <BindingMetaType metaType, typename T> struct BindingKeyType<metaType, const T> {
	static const bool isConst;
};

template <BindingMetaType metaType, typename T> const bool BindingKeyType<metaType, T>::isConst = false;
template <BindingMetaType metaType, typename T> const bool BindingKeyType<metaType, const T>::isConst = true;

///////////////////////////////////////////////////////////////////////////////

//
// Attempts to pretty print the string returned by boost::core::demangle.
//
// Returns the original string if it cannot be pretty printed.
//
// A typical input to this function is:
//   Balau::Impl::BindingKeyType<(Balau::Impl::BindingMetaType)3, Balau::Injector>
//
// This function will need to be reimplemented for different platforms/compilers.
//
inline std::string demangleBindingKeyTypeString(const std::string & s) {
	if (!Util::Strings::startsWith(s, "Balau::Impl::BindingKeyType<(Balau::Impl::BindingMetaType)")
	    || s.length() < 58 + 5 // min valid length for this platform/compiler
	    || !Util::Strings::endsWith(s, ">")) {
		return s; // failed
	}

	std::string str = s.substr(58); // strlen("Balau::Impl::BindingKeyType<(Balau::Impl::BindingMetaType)")

	if (!std::isdigit(static_cast<unsigned char>(str[0]))) {
		return s; // failed
	}

	std::size_t pos;
	const int metaTypeOrdinal = std::stoi(str, &pos); // never throws because str[0] is a digit

	if (metaTypeOrdinal < 0 || metaTypeOrdinal > 5) {
		return s; // failed
	}

	const BindingMetaType metaType = static_cast<BindingMetaType>(metaTypeOrdinal);

	str = str.substr(pos);

	if (!Util::Strings::startsWith(str, ", ")) {
		return s; // failed
	}

	str = str.substr(2, str.length() - 3);

	return toString(metaType) + ", " + str;
}

///////////////////////////////////////////////////////////////////////////////

//
// Unique identification of a binding.
//
class BindingKey final {
	public: template <typename T> static BindingKey createValueKey(const std::string & name_) {
		return BindingKey(typeid(BindingKeyType<BindingMetaType::Value, T>), name_);
	}

	public: template <typename T> static BindingKey createUniqueKey(const std::string & name_) {
		return BindingKey(typeid(BindingKeyType<BindingMetaType::Unique, T>), name_);
	}

	public: template <typename T> static BindingKey createSharedKey(const std::string & name_) {
		return BindingKey(typeid(BindingKeyType<BindingMetaType::Shared, T>), name_);
	}

	public: BindingKey(std::type_index type_, const std::string & name_)
		: type(type_)
		, name(name_)
		#ifdef BALAU_DEBUG
		, typeStr(demangleBindingKeyTypeString(boost::core::demangle(type.name())))
		#endif
		{}

	public: BindingKey(std::type_index type_, std::string && name_)
		: type(type_)
		, name(std::move(name_))
		#ifdef BALAU_DEBUG
		, typeStr(demangleBindingKeyTypeString(boost::core::demangle(type.name())))
		#endif
		{}

	public: BindingKey(std::string && name_)
		: type(typeid(BindingKey)) // Not set.
		, name(std::move(name_))
		#ifdef BALAU_DEBUG
		, typeStr(demangleBindingKeyTypeString(boost::core::demangle(type.name())))
		#endif
		{}

	public: BindingKey()
		: type(typeid(BindingKey)) // Not set.
		, name()
		#ifdef BALAU_DEBUG
		, typeStr(demangleBindingKeyTypeString(boost::core::demangle(type.name())))
		#endif
		{}

	public: BindingKey(const BindingKey & rhs) = default;
	public: BindingKey(BindingKey && rhs) = default;

	public: BindingKey & operator = (const BindingKey & rhs) = default;
	public: BindingKey & operator = (BindingKey && rhs) = default;

	public: std::type_index getType() const {
		return type;
	}

	public: const std::string & getName() const {
		return name;
	}

	public: size_t hashcode() const noexcept {
		return std::hash<std::type_index>()(type) ^ (41 * std::hash<std::string>()(name));
	}

	friend class BindingBuilderBase;

	private: void setType(std::type_index type_) {
		type = type_;
		#ifdef BALAU_DEBUG
		typeStr = demangleBindingKeyTypeString(boost::core::demangle(type.name()));
		#endif
	}

	private: std::type_index type;
	private: std::string name;

	#ifdef BALAU_DEBUG
	// Displays the demangled binding key type in the debugger.
	private: std::string typeStr;
	#endif
};

template <typename AllocatorT>
inline U8String<AllocatorT> toString(const BindingKey & bindingKey) {
	using ::toString;

	return toString<AllocatorT>(
		"{ "
		, demangleBindingKeyTypeString(boost::core::demangle(bindingKey.getType().name()))
		, ", \""
		, bindingKey.getName()
		, "\" }"
	);
}

inline std::string toString(const BindingKey & bindingKey) {
	return toString<std::allocator<char>>(bindingKey);
}

inline bool operator == (const BindingKey & lhs, const BindingKey & rhs) {
	return lhs.getType() == rhs.getType() && lhs.getName() == rhs.getName();
}

inline bool operator != (const BindingKey & lhs, const BindingKey & rhs) {
	return ! operator == (lhs, rhs);
}

///////////////////////////////////////////////////////////////////////////////

// Create a "Value" binding key.
template <typename T> struct CreateBindingKey {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Value, T>), std::move(name));
	}
};

// Create a "Value" binding key.
template <typename T> struct CreateBindingKey<const T> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Value, T>), std::move(name));
	}
};

// Create a "Unique" binding key.
template <typename T> struct CreateBindingKey<std::unique_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Unique, T>), std::move(name));
	}
};

// Create a "Unique" binding key.
template <typename T> struct CreateBindingKey<const std::unique_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Unique, T>), std::move(name));
	}
};

// Create a "Unique" binding key.
template <typename T> struct CreateBindingKey<std::unique_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Unique, T>), std::move(name));
	}
};

// Create a "Unique" binding key.
template <typename T> struct CreateBindingKey<const std::unique_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Unique, T>), std::move(name));
	}
};

// Create a "Reference" binding key.
template <typename T> struct CreateBindingKey<T &> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Reference, T>), std::move(name));
	}
};

// Create a const "Reference" binding key.
template <typename T> struct CreateBindingKey<const T &> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Reference, const T>), std::move(name));
	}
};

// Create a "Shared" binding key.
template <typename T> struct CreateBindingKey<std::shared_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Shared, T>), std::move(name));
	}
};

// Create a const "Shared" binding key.
template <typename T> struct CreateBindingKey<const std::shared_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Shared, T>), std::move(name));
	}
};

// Create a "Shared" const binding key.
template <typename T> struct CreateBindingKey<std::shared_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Shared, const T>), std::move(name));
	}
};

// Create a const "Shared" const binding key.
template <typename T> struct CreateBindingKey<const std::shared_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::Shared, const T>), std::move(name));
	}
};

// Create a "Shared" weak binding key.
template <typename T> struct CreateBindingKey<std::weak_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::WeakPromotion, T>), std::move(name));
	}
};

// Create a const weak "Shared" binding key.
template <typename T> struct CreateBindingKey<const std::weak_ptr<T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::WeakPromotion, T>), std::move(name));
	}
};

// Create a weak "Shared" const binding key.
template <typename T> struct CreateBindingKey<std::weak_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::WeakPromotion, const T>), std::move(name));
	}
};

// Create a const weak "Shared" const binding key.
template <typename T> struct CreateBindingKey<const std::weak_ptr<const T>> {
	Impl::BindingKey operator () (std::string name) const {
		return Impl::BindingKey(typeid(BindingKeyType<Impl::BindingMetaType::WeakPromotion, const T>), std::move(name));
	}
};

//
// A transient binding key type that is used in get instance calls and other transiently defined data structures.
//
// Using this type prevents the need for string creation.
//
struct BindingKeyView {
	BindingKeyView(std::type_index type_, std::string_view name_)
		: type(type_)
		, name(name_) {}

	BindingKeyView(const BindingKey & key)
		: type(key.getType())
		, name(key.getName()) {}

	//
	// ////// C++ specification: std::string and std::string_view hashes //////
	//
	// These [std::string] hashes equal the hashes of corresponding std::basic_string_view
	// classes: If S is one of these string types, SV is the corresponding string view type,
	// and s is an object of type S, then std::hash<S>()(s) == std::hash<SV>()(SV(s)).
	//
	size_t hashcode() const noexcept {
		return std::hash<std::type_index>()(type) ^ (41 * std::hash<std::string_view>()(name));
	}

	BindingKey toKey() const {
		return BindingKey(type, std::string(name));
	}

	std::type_index type;
	std::string_view name;
};

inline bool operator == (const BindingKey & lhs, const BindingKeyView & rhs) {
	return lhs.getType() == rhs.type && lhs.getName() == rhs.name;
}

inline bool operator == (const BindingKeyView & lhs, const BindingKey & rhs) {
	return lhs.type == rhs.getType() && lhs.name == rhs.getName();
}

inline bool operator == (const BindingKeyView & lhs, const BindingKeyView & rhs) {
	return lhs.type == rhs.type && lhs.name == rhs.name;
}

} // namespace Impl

} // namespace Balau

namespace std { // NOLINT

template <> struct hash<Balau::Impl::BindingKey> {
	size_t operator () (const Balau::Impl::BindingKey & bindingKey) const noexcept {
		return hash<type_index>()(bindingKey.getType()) ^ (41 * hash<std::string>()(bindingKey.getName()));
	}
};

template <> struct equal_to<Balau::Impl::BindingKey> {
	inline bool operator () (const Balau::Impl::BindingKey & lhs, const Balau::Impl::BindingKey & rhs) const {
		return lhs == rhs;
	}
};

} // namespace std

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_KEY
