#pragma once

#include "BaseModel.h"
#include "CoreMinimal.h"

namespace UnrealHttp
{
	namespace api
	{
		class QAUTHPOQ_API HttpService
		{
		public:
			HttpService();

			void AddHeaderParam(const FString& Key, const FString& Value);
			void ClearHeaderParams();

			void SetHttpRetryManager(FHttpRetrySystem::FManager& RetryManager);
			FHttpRetrySystem::FManager& GetHttpRetryManager();
	
			class PoqGetAccessTokenRequest;
			class PoqGetAccessTokenResponse;
			class PoqGetWalletRequest;
			class PoqGetWalletResponse;
	
			FHttpRequestPtr getAccessToken(const PoqGetAccessTokenRequest& request);
			FHttpRequestPtr getWallet(const PoqGetWalletRequest& request);
			
			void onAccessTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, TFunction<FHttpRequestPtr(const PoqGetWalletRequest&)> requestMethod) const;
			void onWalletResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded) const;

			
			FHttpRequestRef CreateHttpRequest(const Request& Request) const;   // const Request& Request
	
			private:
			TMap<FString, FString> AdditionalHeaderParams;
			mutable FHttpRetrySystem::FManager* RetryManager = nullptr;
			mutable TUniquePtr<HttpRetryManager> DefaultRetryManager;

			const FString prefixUrl = "https://s2w-dev-firebase.herokuapp.com/"; 
		};
	}
}