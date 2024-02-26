// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
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

#ifndef COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_MESSAGE_ITEMS
#define COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_MESSAGE_ITEMS

#include <Balau/Logging/Impl/LoggerAllocator.hpp>

namespace Balau::LoggingSystem {

///////////////////////// Message text format classes /////////////////////////

using ParameterList = LoggerStringVector;
using ParameterListIterator = ParameterList::const_iterator;

class MessageItem {
	public: virtual std::string_view get(std::string_view messageText, std::string_view parameter) const = 0;
	public: virtual std::string_view get(std::string_view messageText, ParameterListIterator & parameterIterator) const = 0;
	public: virtual ~MessageItem() = default;
};

class StringMessageItem : public MessageItem {
	private: const size_t textStart;
	private: const size_t textEnd;

	public: explicit StringMessageItem(size_t textStart_, size_t textEnd_)
		: textStart(textStart_), textEnd(textEnd_) {}

	public: std::string_view get(std::string_view messageText, std::string_view) const override {
		return messageText.substr(textStart, textEnd - textStart);
	}

	public: std::string_view get(std::string_view messageText, ParameterListIterator &) const override {
		return messageText.substr(textStart, textEnd - textStart);
	}
};

class ParameterMessageItem : public MessageItem {
	public: std::string_view get(std::string_view, std::string_view parameter) const override {
		return parameter;
	}

	public: std::string_view get(std::string_view, ParameterListIterator & parameterIterator) const override {
		return *parameterIterator++;
	}
};

} // namespace Balau::LoggingSystem

#endif // COM_BORA_SOFTWARE_BALAU_LOGGING_IMPL_MESSAGE_ITEMS
