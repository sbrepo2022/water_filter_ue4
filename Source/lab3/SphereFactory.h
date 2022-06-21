// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "Kismet/GameplayStatics.h"
#include "GameFramework/Actor.h"
#include "SphereActor.h"
#include "ACylinderFilter.h"
#include "SphereFactory.generated.h"

UCLASS()
class LAB3_API ASphereFactory : public AActor
{
	GENERATED_BODY()
	

private:
	std::vector<ASphereActor*> Spheres;

	void createSpheres();
	void setupInput();

public:	
	// Sets default values for this actor's properties
	ASphereFactory();
	~ASphereFactory();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
		float SpawnDensity = 100.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
		float SpawnOffsetsTolerance = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
		int SpawnBoxElementsCount = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
		float SphereDiameter = 75.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "SpawnSettings")
		float SphereDiameterTolerance = 20.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	void Stop();
	void Remove();
	void CreateWater();

};
