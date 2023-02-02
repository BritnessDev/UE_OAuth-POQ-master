// Fill out your copyright notice in the Description page of Project Settings.


#include "MyActor.h"
#include "HttpClient/ApiOperations.h"

// Sets default values
AMyActor::AMyActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	httpService = new UnrealHttp::api::HttpService();
}

// Called when the game starts or when spawned
void AMyActor::BeginPlay()
{
	Super::BeginPlay();
	UnrealHttp::api::HttpService::PoqGetAccessTokenRequest request;
	request.code = "";
	httpService->getAccessToken(request);
}

// Called every frame
void AMyActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}
