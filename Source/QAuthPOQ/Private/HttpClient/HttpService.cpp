// Copyright 2018 Colorado Stark

#include "HttpClient/HttpService.h"
#include "HttpClient/ApiOperations.h"

#include "HttpModule.h"
#include "Serialization/JsonSerializer.h"

namespace UnrealHttp
{
	namespace api
	{
		HttpService::HttpService()
		{
	
		}
 
		void HttpService::AddHeaderParam(const FString& Key, const FString& Value)
		{
			AdditionalHeaderParams.Add(Key, Value);
		}

		void HttpService::ClearHeaderParams()
		{
			AdditionalHeaderParams.Reset();
		}

		void HttpService::SetHttpRetryManager(FHttpRetrySystem::FManager& InRetryManager)
		{
			if (RetryManager != &GetHttpRetryManager()) {
				DefaultRetryManager.Reset();
				RetryManager = &InRetryManager;
			}
		}

		FHttpRetrySystem::FManager& HttpService::GetHttpRetryManager()
		{
			checkf(RetryManager, TEXT("UnrealApi: RetryManager is null.  You may have meant to set it with SetHttpRetryManager first, or you may not be using a custom RetryManager at all.")) return *RetryManager;
		}

		FHttpRequestPtr HttpService::getAccessToken(const PoqGetAccessTokenRequest& request)
		{
			FHttpRequestRef HttpRequest = CreateHttpRequest((const Request&)request);   // Request
			HttpRequest->SetURL(*(prefixUrl + request.ComputePath()));
			
			for (const auto& It : AdditionalHeaderParams) {
				HttpRequest->SetHeader(It.Key, It.Value);
			}

			request.SetupHttpRequest(HttpRequest);

			HttpRequest->OnProcessRequestComplete().BindLambda([this] (FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded)
			{
				this->onAccessTokenResponse(HttpRequest, HttpResponse, bSucceeded, std::bind(
						&HttpService::getWallet,
						this,
						std::placeholders::_1
					));
			});

			HttpRequest->ProcessRequest();
			return HttpRequest;

			return HttpRequest;
		}

		void HttpService::onAccessTokenResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded, TFunction<FHttpRequestPtr(const PoqGetWalletRequest&)> requestMethod) const
		{
			UE_LOG(LogTemp, Display, TEXT("content Type: %s"), *HttpResponse->GetContentType());
			if (bSucceeded && HttpResponse->GetContentType().Contains("application/json"))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

				FString str = HttpResponse->GetContentAsString();
				UE_LOG(LogTemp, Display, TEXT("content Type: %s"), *str);
				TSharedRef<TJsonReader<TCHAR>> JsonReader =  TJsonReaderFactory<TCHAR>::Create(HttpResponse->GetContentAsString());

				FJsonSerializer::Deserialize(JsonReader, JsonObject);
				str.Empty();

				FString token = JsonObject->GetStringField("access_token");

				UE_LOG(LogTemp, Display, TEXT("access_token = '%s'"), *token);
				
				if(!token.IsEmpty())
				{
					PoqGetWalletRequest request;
					request.access_token = token;
					requestMethod(request);
				}
			}
			else
			{
				// Handle error here
			}
			
		}

		FHttpRequestPtr HttpService::getWallet(const PoqGetWalletRequest& request)
		{
			FHttpRequestRef HttpRequest = CreateHttpRequest((const Request&)request);   // Request
			HttpRequest->SetURL(*(prefixUrl + request.ComputePath()));

			AddHeaderParam("Authorization", "Bearer " + request.access_token);
			
			for (const auto& It : AdditionalHeaderParams) {
				HttpRequest->SetHeader(It.Key, It.Value);
			}

			request.SetupHttpRequest(HttpRequest);
			
			HttpRequest->OnProcessRequestComplete().BindRaw(this, &HttpService::onWalletResponse);
			HttpRequest->ProcessRequest();
			return HttpRequest;

			return HttpRequest;
		}

		void HttpService::onWalletResponse(FHttpRequestPtr HttpRequest, FHttpResponsePtr HttpResponse, bool bSucceeded) const
		{
			UE_LOG(LogTemp, Display, TEXT("content Type: %s"), *HttpResponse->GetContentType());
			if (bSucceeded && HttpResponse->GetContentType().Contains("application/json"))
			{
				TSharedPtr<FJsonObject> JsonObject = MakeShareable(new FJsonObject());

				FString str = HttpResponse->GetContentAsString();
				UE_LOG(LogTemp, Display, TEXT("content Type: %s"), *str);
				TSharedRef<TJsonReader<TCHAR>> JsonReader =  TJsonReaderFactory<TCHAR>::Create(HttpResponse->GetContentAsString());

				FJsonSerializer::Deserialize(JsonReader, JsonObject);
				str.Empty();

				FString result = JsonObject->GetStringField("email");

				UE_LOG(LogTemp, Display, TEXT("email = '%s'"), *result);
			}
			else
			{
				// Handle error here
			}
			/*Unity3dGetOwnedNftsGetResponse Response;
			HandleResponse(HttpResponse, bSucceeded, Response);*/
		}

		
		FHttpRequestRef HttpService::CreateHttpRequest(const Request& Request) const   // const Request& Request
		{
			if (!Request.GetRetryParams().IsSet()) {
				return FHttpModule::Get().CreateRequest();
			} else {
				if (!RetryManager) {
					// Create default retry manager if none was specified
					DefaultRetryManager = MakeUnique<HttpRetryManager>(6, 60);
					RetryManager = DefaultRetryManager.Get();
				}

				const HttpRetryParams& Params = Request.GetRetryParams().GetValue();
				return RetryManager->CreateRequest(Params.RetryLimitCountOverride, Params.RetryTimeoutRelativeSecondsOverride, Params.RetryResponseCodes, Params.RetryVerbs, Params.RetryDomains);
			}
		}
	}
}