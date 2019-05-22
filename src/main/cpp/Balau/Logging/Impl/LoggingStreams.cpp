// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2008 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "LoggingStreams.hpp"
#include "../../Exception/ResourceExceptions.hpp"
#include "../../Util/Compression.hpp"
#include "../../Util/DateTime.hpp"
#include "../../Util/Files.hpp"
#include "../../Util/Hashing.hpp"
#include "../../Util/Strings.hpp"
#include "../../Util/Vectors.hpp"

namespace Balau {

using namespace Util;

namespace LoggingSystem {

std::regex FileLoggingStream::dateRegEx(
	"\\{ *date( +(compress|%[aAbBcCdeFgGhjmuUVwWyY]((-_)?%[aAbBcCdeFgGhjmuUVwWyY])*))* *\\}"
);

std::regex FileLoggingStream::dateRegExExact(
	"^\\{ *date( +(compress|%[aAbBcCdeFgGhjmuUVwWyY]((-_)?%[aAbBcCdeFgGhjmuUVwWyY])*))* *\\}$"
);

FileLoggingStream::FileLoggingStream(std::shared_ptr<System::Clock> clock_, std::string_view uri)
	: clock(std::move(clock_))
	, pathComponents(Util::Vectors::toStringVector(Strings::split(uri, dateRegEx, false)))
	, running(Strings::occurrences(uri, dateRegEx) == 0) {

	// Only run the updater thread if the URI contains date placeholders.
	if (running) {
		std::unique_lock<std::mutex> lock(syncMutex);
		updater = std::thread(updateFunction, this);
		// Wait until the updater thread unlocks this thread.
		syncCondition.wait(lock);
	}
}

FileLoggingStream::~FileLoggingStream() {
	if (running) {
		running = false;

		{
			std::lock_guard<std::mutex> lock(syncMutex);
			// Prep the updater thread for unlock.
			// The updater thread will unlock when this thread releases the lock.
			syncCondition.notify_all();
		}

		updater.join();
	}
}

void FileLoggingStream::createNewStream() {
	Date::year_month_day today = clock->today();
	std::ostringstream str;

	for (const auto & pathComponent : pathComponents) {
		if (std::regex_search(pathComponent, dateRegExExact)) {
			const std::string formatString = pathComponent.substr(5, pathComponent.length() - 5 - 1);
			Date::to_stream(str, formatString.c_str(), today);
		} else {
			str << pathComponent;
		}
	}

	// Lop off the "file://" prefix.
	const std::string newPathStr = str.str().substr(7);

	if (currentPath == newPathStr) {
		return; // spurious awaken.
	}

	const std::string previousPathString = currentPath;
	currentPath = newPathStr;

	boost::filesystem::path newPath(newPathStr);
	auto parentPath = newPath.parent_path();

	if (!boost::filesystem::exists(parentPath)) {
		try {
			std::cout << "Attempting to create logging folder: " << parentPath << std::endl;
			boost::filesystem::create_directories(parentPath);
		} catch (const boost::filesystem::filesystem_error & e) {
			ThrowBalauException(
				  Exception::CouldNotCreateException
				, "Failed to create parent directory of logging file"
				, Resource::File(newPath)
			);
		}
	}

	if (boost::filesystem::is_directory(newPath)) {
		ThrowBalauException(
			  Exception::CouldNotCreateException
			, "The specified logging file is a directory"
			, Resource::File(newPath)
		);
	}

	stream = std::make_shared<boost::filesystem::ofstream>(
		newPath, std::ios::app | std::ios::binary
	);

	if (!stream->is_open()) {
		ThrowBalauException(
			  Exception::CouldNotOpenException
			, "The specified logging file could not be opened for writing"
			, Resource::File(newPath)
		);
	}

	if (previousPathString.empty()) {
		return;
	}

	boost::filesystem::path previousPath(previousPathString);

	#ifdef BALAU_ZLIB_ENABLED

	if (boost::filesystem::exists(previousPath)) {
		boost::filesystem::path previousPathGzip(previousPath.string() + ".gz");
		Resource::File previousPathGzipFile(previousPathGzip);
		Resource::File previousPathFile(previousPath);


		// Zip up the file.
		GZip::gzip(previousPathFile, previousPathGzipFile);

		// Check the file was gzipped correctly before deleting the original.
		if (boost::filesystem::exists(previousPathGzip)) {
			std::unique_ptr<std::istream> decompressedStream;
			GZip::gunzip(previousPathGzipFile, decompressedStream);
			std::string decompressedFileHash = Hashing::md5(*decompressedStream);
			std::string originalFileHash = Hashing::md5(previousPathFile);

			if (decompressedFileHash == originalFileHash) {
				previousPathFile.removeFile();
			} else {
				// TODO report error
			}
		} else {
			// TODO report error
		}
	}

	#endif // BALAU_ZLIB_ENABLED
}

void FileLoggingStream::updateFunction(FileLoggingStream * const self) {
	std::unique_lock<std::mutex> lock(self->syncMutex);

	self->createNewStream();

	// Prep the constructor for unlock.
	// The constructor will unlock when wait is entered in this thread.
	self->syncCondition.notify_all();

	while (true) {
		// The destructor will notify this wait when the system shuts down.
		self->syncCondition.wait(lock);

		if (!self->running) {
			return;
		}

		self->createNewStream();
	}
}

} // namespace LoggingSystem

} // namespace Balau
