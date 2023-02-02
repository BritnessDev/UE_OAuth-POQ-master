#include "HttpClient/ApiOperations.h"
#include "HttpClient/Helpers.h"

#include "Dom/JsonObject.h"
#include "HttpModule.h"
#include "PlatformHttp.h"
#include "Templates/SharedPointer.h"

namespace UnrealHttp
{
    namespace api
    {
        FString HttpService::PoqGetAccessTokenRequest::ComputePath() const
        {
            return FString(TEXT("api/oauth2/token"));
        }

        void HttpService::PoqGetAccessTokenRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
        {
            static const TArray<FString> Consumes = {TEXT("application/x-www-form-urlencoded")};
            /*static const TArray<FString> Consumes = {TEXT("application/json")};*/

            HttpRequest->SetVerb(TEXT("POST"));

            // Default to Json Body request
            if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json"))) {
                FString JsonBody;
                JsonWriter Writer = TJsonWriterFactory<>::Create(&JsonBody);
                /*Writer->WriteObjectStart();*/
                Writer->WriteValue("client_id", *client_id);
                Writer->WriteValue("client_secret", *client_secret);
                Writer->WriteValue("grant_type", *grant_type);
                Writer->WriteValue("code", *code);
                Writer->WriteValue("redirect_uri", *redirect_uri);
                /*Writer->WriteObjectEnd();*/
                Writer->Close();
                HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
                HttpRequest->SetContentAsString(JsonBody);
            } else if (Consumes.Contains(TEXT("multipart/form-data"))) {
            } else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded"))) {
                FString postData = "";
                postData = TEXT("grant_type=") + grant_type;
                postData = postData + TEXT("&code=") + code;
                postData = postData + TEXT("&client_id=") + client_id;
                postData = postData + TEXT("&client_secret=") + client_secret;
                postData = postData + TEXT("&redirect_uri=") + redirect_uri;
                
                HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
                HttpRequest->SetContentAsString(postData);
            } else {
                /*UE_LOG(LogStratisAPI, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));*/
            }
        }

        void HttpService::PoqGetAccessTokenResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
        {
            Response::SetHttpResponseCode(InHttpResponseCode);
            switch ((int)InHttpResponseCode) {
            case 200:
                SetResponseString(TEXT("Success"));
                break;
            case 400:
                SetResponseString(TEXT("Bad Request"));
                break;
            }
        }

        bool HttpService::PoqGetAccessTokenResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
        {
            return TryGetJsonValue(JsonValue, Content);
        }

        FString HttpService::PoqGetWalletRequest::ComputePath() const
        {
            return FString(TEXT("api/v1/users/me"));
        }

        void HttpService::PoqGetWalletRequest::SetupHttpRequest(const FHttpRequestRef& HttpRequest) const
        {
            static const TArray<FString> Consumes = {TEXT("application/x-www-form-urlencoded")};
            /*static const TArray<FString> Consumes = {TEXT("application/json")};*/

            HttpRequest->SetVerb(TEXT("GET"));

            // Default to Json Body request
            if (Consumes.Num() == 0 || Consumes.Contains(TEXT("application/json"))) {
                FString JsonBody;
                JsonWriter Writer = TJsonWriterFactory<>::Create(&JsonBody);
                /*Writer->WriteValue("client_id", *client_id);*/
                Writer->Close();
                HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/json; charset=utf-8"));
                HttpRequest->SetContentAsString(JsonBody);
            } else if (Consumes.Contains(TEXT("multipart/form-data"))) {
            } else if (Consumes.Contains(TEXT("application/x-www-form-urlencoded"))) {
                FString getData = "";
                
                HttpRequest->SetHeader(TEXT("Content-Type"), TEXT("application/x-www-form-urlencoded"));
                
                HttpRequest->SetContentAsString(getData);
            } else {
                /*UE_LOG(LogStratisAPI, Error, TEXT("Request ContentType not supported (%s)"), *FString::Join(Consumes, TEXT(",")));*/
            }
        }

        void HttpService::PoqGetWalletResponse::SetHttpResponseCode(EHttpResponseCodes::Type InHttpResponseCode)
        {
            Response::SetHttpResponseCode(InHttpResponseCode);
            switch ((int)InHttpResponseCode) {
            case 200:
                SetResponseString(TEXT("Success"));
                break;
            case 400:
                SetResponseString(TEXT("Bad Request"));
                break;
            }
        }

        bool HttpService::PoqGetWalletResponse::FromJson(const TSharedPtr<FJsonValue>& JsonValue)
        {
            return TryGetJsonValue(JsonValue, Content);
        }
    }
}