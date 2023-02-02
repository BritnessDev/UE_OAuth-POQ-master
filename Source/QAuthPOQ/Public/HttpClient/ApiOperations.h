#pragma once

#include "BaseModel.h"
#include "HttpService.h"

#include "BlockModel.h"

namespace UnrealHttp
{
	namespace api
	{
		class QAUTHPOQ_API HttpService::PoqGetAccessTokenRequest : public Request
		{
		public:
			virtual ~PoqGetAccessTokenRequest() {}
			void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
			FString ComputePath() const final;
			
			FString client_id = "pmQegCYcs446TVEvQA2M";
			FString client_secret = "t15kzzbnssg47b0ldf6pt47j6hkkp4044";
			FString grant_type = "authorization_code";
			FString code;
			FString redirect_uri = "http://localhost:8080/get";
			
		};

		class QAUTHPOQ_API HttpService::PoqGetAccessTokenResponse : public Response
		{
		public:
			virtual ~PoqGetAccessTokenResponse() {}
			void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
			bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

			BlockModel Content;
		};

		class QAUTHPOQ_API HttpService::PoqGetWalletRequest : public Request
		{
		public:
			virtual ~PoqGetWalletRequest() {}
			void SetupHttpRequest(const FHttpRequestRef& HttpRequest) const final;
			FString ComputePath() const final;
			
			FString access_token;
			
		};

		class QAUTHPOQ_API HttpService::PoqGetWalletResponse : public Response
		{
		public:
			virtual ~PoqGetWalletResponse() {}
			void SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode) final;
			bool FromJson(const TSharedPtr<FJsonValue>& JsonValue) final;

			BlockModel Content;
		};
	}
}