#pragma once
#include "common/ExplicitSingleton.h"
#include "common/String.h"
#include "client/http/PostData.h"
#include <atomic>
#include <cstdint>
#include <thread>
#include <vector>
#include <memory>
#include <mutex>
#include <condition_variable>
#include <optional>
#include <utility>

namespace http
{
	class Request;

	struct RequestHandle
	{
	protected:
		struct CtorTag
		{
		};

	public:
		ByteString uri;
		std::optional<ByteString> verb;
		bool isPost = false;
		PostData postData;
		std::vector<Header> headers;

		enum State
		{
			ready,
			running,
			done,
			dead,
		};
		State state = ready;
		std::mutex stateMx;
		std::condition_variable stateCv;
		std::atomic<int64_t> bytesTotal = -1;
		std::atomic<int64_t> bytesDone = 0;
		int statusCode = 0;
		ByteString responseData;
		std::vector<Header> responseHeaders;
		std::optional<ByteString> error;
		std::optional<ByteString> failEarly;

		RequestHandle(CtorTag)
		{
		}

		RequestHandle(const RequestHandle &) = delete;
		RequestHandle &operator =(const RequestHandle &) = delete;

		void MarkDone();

		static std::shared_ptr<RequestHandle> Create();
	};

	class RequestManager;
	struct RequestManagerDeleter
	{
		void operator ()(RequestManager *ptr) const;
	};
	using RequestManagerPtr = std::unique_ptr<RequestManager, RequestManagerDeleter>;
	class RequestManager : public ExplicitSingleton<RequestManager>
	{
	protected:
		ByteString proxy;
		ByteString cafile;
		ByteString capath;
		ByteString userAgent;
		bool disableNetwork;

		RequestManager(ByteString newProxy, ByteString newCafile, ByteString newCapath, bool newDisableNetwork);

		void RegisterRequestImpl(Request &request);
		void UnregisterRequestImpl(Request &request);

	public:
		void RegisterRequest(Request &request);
		void UnregisterRequest(Request &request);

		bool DisableNetwork() const
		{
			return disableNetwork;
		}

		const ByteString &Cafile() const
		{
			return cafile;
		}

		const ByteString &Capath() const
		{
			return capath;
		}

		static RequestManagerPtr Create(ByteString newProxy, ByteString newCafile, ByteString newCapath, bool newDisableNetwork);
	};
}
