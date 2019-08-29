// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#ifndef COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_MAP
#define COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_MAP

#include <Balau/Application/Impl/Binding.hpp>

namespace Balau::Impl {

//
// A simple hash map implementation used in the injector.
//
// This map uses the BindingKey type as the key type, but accepts a
// BindingKeyView type in the get call.
//
// Building the map is not optimised, as it only occurs once during injector
// instantiation.
//
class BindingMap {
	private: static const size_t InitialBucketCount = 4;
	private: static const size_t BucketCountDelta = 1024;

	private: struct Entry {
		Impl::BindingKey key;
		std::unique_ptr<Impl::AbstractBinding> value;

		Entry(Impl::BindingKey key_, std::unique_ptr<Impl::AbstractBinding> && value_)
			: key(std::move(key_))
			, value(std::move(value_)) {}

		Entry(Entry && rhs) noexcept
			: key(std::move(rhs.key))
			, value(std::move(rhs.value)) {}
	};

	public: class ConstIterator {
		friend class BindingMap;

		private: explicit ConstIterator(const BindingMap * parent_)
			: parent(parent_)
			, bucketIndex(0)
			, entryIndex(0) {
			while (bucketIndex < parent->buckets.size() && parent->buckets[bucketIndex].empty()) {
				++bucketIndex;
			}
		}

		private: ConstIterator(const BindingMap * parent_, size_t bucketIndex_, size_t entryIndex_)
			: parent(parent_)
			, bucketIndex(bucketIndex_)
			, entryIndex(entryIndex_) {}

		public: const Entry & operator * () const {
			return parent->buckets[bucketIndex][entryIndex];
		}

		public: const Entry * operator -> () const {
			return &parent->buckets[bucketIndex][entryIndex];
		}

		public: bool operator == (const ConstIterator & rhs) const {
			return parent == rhs.parent && bucketIndex == rhs.bucketIndex && entryIndex == rhs.entryIndex;
		}

		public: bool operator != (const ConstIterator & rhs) const {
			return ! operator == (rhs);
		}

		public: ConstIterator & operator ++ () {
			++entryIndex;

			if (entryIndex == parent->buckets[bucketIndex].size()) {
				entryIndex = 0;
				++bucketIndex;

				while (bucketIndex < parent->buckets.size() && parent->buckets[bucketIndex].empty()) {
					++bucketIndex;
				}
			}

			return *this;
		}

		private: const BindingMap * parent;
		private: size_t bucketIndex;
		private: size_t entryIndex;
	};

	public: BindingMap()
		: buckets(InitialBucketCount)
		, entryCount(0) {}

	public: const std::unique_ptr<Impl::AbstractBinding> & get(const BindingKeyView & key) const {
		auto bucketIndex = key.hashcode() % buckets.size();
		auto & bucket = buckets[bucketIndex];

		for (const auto & entry : bucket) {
			if (entry.key == key) {
				return entry.value;
			}
		}

		ThrowBalauException(Exception::NoBindingException, key.toKey());
	}

	public: const std::unique_ptr<Impl::AbstractBinding> & get(const BindingKey & key) const {
		auto bucketIndex = key.hashcode() % buckets.size();
		auto & bucket = buckets[bucketIndex];

		for (const auto & entry : bucket) {
			if (entry.key == key) {
				return entry.value;
			}
		}

		ThrowBalauException(Exception::NoBindingException, key);
	}

	public: const std::unique_ptr<Impl::AbstractBinding> * find(const BindingKey & key) const {
		auto bucketIndex = key.hashcode() % buckets.size();
		auto & bucket = buckets[bucketIndex];

		for (const auto & entry : bucket) {
			if (entry.key == key) {
				return &entry.value;
			}
		}

		return nullptr;
	}

	public: const std::unique_ptr<Impl::AbstractBinding> * find(const BindingKeyView & key) const {
		auto bucketIndex = key.hashcode() % buckets.size();
		auto & bucket = buckets[bucketIndex];

		for (const auto & entry : bucket) {
			if (entry.key == key) {
				return &entry.value;
			}
		}

		return nullptr;
	}

	public: bool hasBinding(const BindingKey & key) const {
		return find(key) != nullptr;
	}

	public: bool hasBinding(const BindingKeyView & key) const {
		return find(key) != nullptr;
	}

	public: ConstIterator begin() const {
		return ConstIterator(this);
	}

	public: ConstIterator end() const {
		return ConstIterator(this, buckets.size(), 0);
	}

	public: void put(const BindingKey & key, std::unique_ptr<Impl::AbstractBinding> && value) {
		if ((double) entryCount >= (double) buckets.size() * 0.7) { // 70% occupancy
			increaseBucketCount();
		}

		++entryCount;

		auto bucketIndex = key.hashcode() % buckets.size();
		auto & bucket = buckets[bucketIndex];

		// Replace?
		for (auto & entry : bucket) {
			if (entry.key == key) {
				entry.value = std::move(value);
				return;
			}
		}

		// Add.
		bucket.emplace_back(Entry(key, std::move(value)));
	}

	private: void increaseBucketCount() {
		const size_t newBucketCount = buckets.size() < BucketCountDelta
			? buckets.size() * 2
			: buckets.size() + BucketCountDelta;

		std::vector<std::vector<Entry>> newBuckets(newBucketCount);

		for (auto & bucket : buckets) {
			for (auto & entry : bucket) {
				auto bucketIndex = entry.key.hashcode() % newBuckets.size();
				newBuckets[bucketIndex].emplace_back(std::move(entry));
			}
		}

		buckets = std::move(newBuckets);
	}

	private: std::vector<std::vector<Entry>> buckets;
	private: size_t entryCount;
};

} // namespace Balau::Impl

#endif // COM_BORA_SOFTWARE__BALAU_APPLICATION_IMPL__BINDING_MAP
