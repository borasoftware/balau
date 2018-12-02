// @formatter:off
//
// Balau core C++ library
//
// Copyright (C) 2017 Bora Software (contact@borasoftware.com)
//
// Licensed under the Boost Software License - Version 1.0 - August 17th, 2003.
// See the LICENSE file for the full license text.
//

#include "CurlEmailSender.hpp"
#include "CurlInitializer.hpp"
#include "../../../../../System/SystemClock.hpp"
#include "../../../../../Type/UUID.hpp"
#include "../../../../../Util/DateTime.hpp"
#include "../../../../../Type/OnScopeExit.hpp"
#include "../../../../../Util/Strings.hpp"
#include "../../../../../Logging/Logger.hpp"

#include <curl/curl.h>

namespace Balau::Network::Http::HttpWebApps::Impl {

Logger & CurlEmailSender::log = Logger::getLogger("balau.network.curl");

CurlEmailSender::CurlEmailSender(std::string host_,
                                 unsigned short port_,
                                 std::string user_,
                                 std::string pw_,
                                 std::string userAgent_,
                                 bool verifyCertificate_)
	: host(std::move(host_))
	, port(port_)
	, user(std::move(user_))
	, pw(std::move(pw_))
	, userAgent(std::move(userAgent_))
	, verifyCertificate(verifyCertificate_) {}

struct EmailData {
	const std::string data;
	size_t bytesTransferred;

	explicit EmailData(std::string && data_) : data(std::move(data_)), bytesTransferred(0) {}
};

size_t dataSourceCallback(void * ptr, size_t size, size_t count, void * obj) {
	auto * const emailData = (EmailData *) obj;
	const size_t bytes = size * count;
	const size_t availableBytes = emailData->data.length() - emailData->bytesTransferred;

	if (bytes == 0 || availableBytes == 0) {
		return 0;
	}

	const size_t requiredBytes = bytes < availableBytes ? bytes : availableBytes;
	memcpy(ptr, emailData->data.data(), requiredBytes);
	emailData->bytesTransferred += requiredBytes;
	return requiredBytes;
}

int debugCallback(CURL * handle, curl_infotype type, char * data, size_t size, void * ) {
	switch (type) {
		case CURLINFO_TEXT: {
			// The data is informational text.
			CurlEmailSender::log.debug("Curl: {}", std::function([&data, &size] () { return std::string(data, size); }));
			break;
		}

		case CURLINFO_HEADER_IN: {
			// The data is header (or header-like) data received from the peer.

			break;
		}

		case CURLINFO_HEADER_OUT: {
			// The data is header (or header-like) data sent to the peer.

			break;
		}

		case CURLINFO_DATA_IN: {
			// The data is protocol data received from the peer.

			break;
		}

		case CURLINFO_DATA_OUT: {
			// The data is protocol data sent to the peer.

			break;
		}

		case CURLINFO_SSL_DATA_IN: {
			// The data is SSL/TLS (binary) data received from the peer.
			CurlEmailSender::log.trace("Curl: {} bytes of SSL/TLS binary data received", size);
			break;
		}

		case CURLINFO_SSL_DATA_OUT: {
			// The data is SSL/TLS (binary) data sent to the peer.
			CurlEmailSender::log.trace("Curl: {} bytes of SSL/TLS binary data sent", size);
			break;
		}

		case CURLINFO_END: {
			// ??
			break;
		}
	}

	return 0;
}

void CurlEmailSender::sendEmail(const std::string & from,
                                const std::string & to,
                                const std::vector<std::string> & cc,
                                const std::string & subject,
                                const std::string & body) {
	// RFC5322: CRLF everywhere.
	std::string payload =
		"Date: " + Util::DateTime::toString("%a, %d %b %Y %T %z", System::SystemClock().now()) + "\r\n" // TODO clock
		"From: " + from + "\r\n"
		"To: " + to + "\r\n"
		+ Util::Strings::prefixSuffixJoin(cc, "Cc: ", "\r\n") +
		"Message-ID: <" + UUID().asString() + "@" + host + ">\r\n"
		+ (userAgent.empty() ? "" : "User-Agent: " + userAgent + "\r\n") +
		"Subject: " + subject + "\r\n"
		"\r\n" // RFC5322: empty line between header and body.
		+ body + "\r\n";

	EmailData emailData(std::move(payload));
	CURL * curl;
	curl_slist * recipients = nullptr;
	CURLcode res;

	ensureCurlInitialization();

	curl = curl_easy_init();

	if (curl == nullptr) {
		log.error("Failed to create Curl context.");
		return;
	}

	// Set up the clean up.
	OnScopeExit cleanUp(
		[&curl, &recipients] () {
			// Free the list of recipients.
			// Is null check required here for the Curl free call?
			if (recipients != nullptr) {
				curl_slist_free_all(recipients);
			}

			// Always cleanup.
			curl_easy_cleanup(curl);
		}
	);

	if (log.debugEnabled()) {
		curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);
		curl_easy_setopt(curl, CURLOPT_DEBUGFUNCTION, debugCallback);
	}

	curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

	const std::string connectionString = "smtps://" + host + ":" + ::toString(port);

	curl_easy_setopt(curl, CURLOPT_USERNAME, user.c_str());
	curl_easy_setopt(curl, CURLOPT_PASSWORD, pw.c_str());
	curl_easy_setopt(curl, CURLOPT_URL, connectionString.c_str());

	/* In this example, we'll start with a plain text connection, and upgrade
	 * to Transport Layer Security (TLS) using the STARTTLS command. Be careful
	 * of using CURLUSESSL_TRY here, because if TLS upgrade fails, the transfer
	 * will continue anyway - see the security discussion in the libcurl
	 * tutorial for more details. */
	// TODO
	curl_easy_setopt(curl, CURLOPT_USE_SSL, (long) CURLUSESSL_ALL);

	if (!verifyCertificate) {
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYPEER, 0L);
		curl_easy_setopt(curl, CURLOPT_SSL_VERIFYHOST, 0L);
	} else {
		/* If your server doesn't have a valid certificate, then you can disable
		 * part of the Transport Layer Security protection by setting the
		 * CURLOPT_SSL_VERIFYPEER and CURLOPT_SSL_VERIFYHOST options to 0 (false).
		 * That is, in general, a bad idea. It is still better than sending your
		 * authentication details in plain text though.  Instead, you should get
		 * the issuer certificate (or the host certificate if the certificate is
		 * self-signed) and add it to the set of certificates that are known to
		 * libcurl using CURLOPT_CAINFO and/or CURLOPT_CAPATH. See docs/SSLCERTS
		 * for more information. */
		// TODO
		curl_easy_setopt(curl, CURLOPT_CAINFO, "/path/to/certificate.pem");
	}

	curl_easy_setopt(curl, CURLOPT_MAIL_FROM, from.c_str());

	recipients = curl_slist_append(recipients, to.c_str());

	for (const auto & s : cc) {
		recipients = curl_slist_append(recipients, s.c_str());
	}

	curl_easy_setopt(curl, CURLOPT_MAIL_RCPT, recipients);
	curl_easy_setopt(curl, CURLOPT_READFUNCTION, dataSourceCallback);
	curl_easy_setopt(curl, CURLOPT_READDATA, &emailData);
	curl_easy_setopt(curl, CURLOPT_UPLOAD, 1L);

	// Send the message.
	res = curl_easy_perform(curl);

	if (res != CURLE_OK) {
		log.error("Failed to send email due to Curl error: {}", curl_easy_strerror(res));
	}
}

} // namespace Balau::Network::Http::HttpWebApps::Impl
