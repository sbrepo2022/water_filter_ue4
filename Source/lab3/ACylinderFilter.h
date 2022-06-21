// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "KismetProceduralMeshLibrary.h"
#include "ProceduralMeshComponent.h"
#include "WaterGenerator.h"
#include "GameFramework/Actor.h"
#include "ACylinderFilter.generated.h"

UCLASS()
class LAB3_API AACylinderFilter : public AActor
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere)
	USceneComponent* scene;

	UPROPERTY(VisibleAnywhere)
	UProceduralMeshComponent* mesh;

public:	
	// Sets default values for this actor's properties
	AACylinderFilter();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeometrySettings")
		float InnerCylinderD = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeometrySettings")
		float OuterCylinderD = 360.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeometrySettings")
		float CylinderH = 300.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeometrySettings")
		float CylinderDelta = 60.0f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "GeometrySettings")
		int RadialVertices = 16;

protected:
	// Called when the game starts or when spawned
	virtual void PostRegisterAllComponents() override;
	virtual void PostLoad() override;
	virtual void BeginPlay() override;
	void CreateMesh(float innerCylinderD = 300.0, float outerCylinderD = 360.0, float cylinderH = 300.0, float cylinderDelta = 60.0, int radialVertices = 16);

public:	
	// Called every frame
	//virtual void Tick(float DeltaTime) override;

	void Remove();
};
