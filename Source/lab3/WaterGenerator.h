// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include <vector>
#include "KismetProceduralMeshLibrary.h"
#include "WaterActor.h"
#include "ProceduralMeshComponent.h"
#include "GameFramework/Actor.h"
#include "WaterGenerator.generated.h"

UCLASS()
class LAB3_API AWaterGenerator : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mesh;

	std::vector<AWaterActor*> Spheres;
	bool start_spawn;
	float spawn_time, last_generated;

public:	
	// Sets default values for this actor's properties
	AWaterGenerator();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float CylinderD = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float CylinderH = 600.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float CylinderDelta = 30.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		int RadialVertices = 16;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float SphereDiameter = 10.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float SphereDiameterTolerance = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float SpawnTime = 5.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float SpawnEvery = 0.3f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "WaterSpawnerSettings")
		float SpawnDensity = 60.0f;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;
	virtual void PostRegisterAllComponents() override;
	virtual void PostLoad() override;
	void CreateMesh(float cylinderD, float cylinderH, float cylinderDelta, int radialVertices);

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void CreateWater();
};
