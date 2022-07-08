// Fill out your copyright notice in the Description page of Project Settings.


#include "SphereFactory.h"

// Sets default values
ASphereFactory::ASphereFactory()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent")));

}

ASphereFactory::~ASphereFactory()
{

}

// Called when the game starts or when spawned
void ASphereFactory::BeginPlay()
{
	Super::BeginPlay();
	
	createSpheres();
	setupInput();
}

// Called every frame
void ASphereFactory::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void ASphereFactory::setupInput() {
	InputComponent = NewObject<UInputComponent>(this, TEXT("InputComponent"));
	InputComponent->RegisterComponent();
	if (InputComponent)
	{
		InputComponent->BindAction("Stop", IE_Pressed, this, &ASphereFactory::Stop);
		InputComponent->BindAction("Remove", IE_Pressed, this, &ASphereFactory::Remove);
		APlayerController* player = GetWorld()->GetFirstPlayerController();
		if (player)
			EnableInput(player);
	}
}

void ASphereFactory::Stop()
{
	TArray<AActor*> spheres;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ASphereActor::StaticClass(), spheres);

	TArray<AActor*> cylinders;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACylinderFilter::StaticClass(), cylinders);

	// Удаление выкатившихся шаров
	for (auto sphere : spheres) {
		bool in_cylinder = false;

		UPrimitiveComponent* primitive_component = Cast<UPrimitiveComponent>(sphere->GetComponentByClass(UPrimitiveComponent::StaticClass()));
		primitive_component->SetSimulatePhysics(false);

		for (auto cylinder : cylinders)
		{
			ASphereActor* sphere_obj = Cast<ASphereActor>(sphere);
			AACylinderFilter *cylinder_obj = Cast<AACylinderFilter>(cylinder);

			FVector sphere_pos = sphere_obj->GetActorLocation();
			FVector cylinder_pos = cylinder_obj->GetActorLocation();
			
			float sphere_z = sphere_pos.Z;
			float cylinder_z = cylinder_pos.Z;

			sphere_pos.Z = 0.0f;
			cylinder_pos.Z = 0.0f;

			for (int i = 0; i < cylinder_obj->RadialVertices; i++)
			{
				// Проверка нахождения сферы в одном из фильтров по формуле проверки, находится ли точка внутри треугольника
				FVector p1(0, 0, 0);
				FVector p2(cos(2 * PI * (double)i / cylinder_obj->RadialVertices) * cylinder_obj->OuterCylinderD / 2, sin(2 * PI * (double)i / cylinder_obj->RadialVertices) * cylinder_obj->OuterCylinderD / 2, 0);
				FVector p3(cos(2 * PI * (double)(i + 1) / cylinder_obj->RadialVertices) * cylinder_obj->OuterCylinderD / 2, sin(2 * PI * (double)(i + 1) / cylinder_obj->RadialVertices) * cylinder_obj->OuterCylinderD / 2, 0);
				p1 += cylinder_pos;
				p2 += cylinder_pos;
				p3 += cylinder_pos;

				FVector res1 = FVector::CrossProduct(p1 - p2, sphere_pos - p2);
				FVector res2 = FVector::CrossProduct(p3 - p1, sphere_pos - p1);
				FVector res3 = FVector::CrossProduct(p2 - p3, sphere_pos - p3);

				if (
					res1.Z <= 0 && res2.Z <= 0 && res3.Z <= 0 &&
					sphere_z < cylinder_z + cylinder_obj->CylinderH && sphere_z > cylinder_z
					)
				{
					in_cylinder = true;

					p1 = FVector(0, 0, 0);
					p2 = FVector(cos(2 * PI * (double)i / cylinder_obj->RadialVertices) * cylinder_obj->InnerCylinderD / 2, sin(2 * PI * (double)i / cylinder_obj->RadialVertices) * cylinder_obj->InnerCylinderD / 2, 0);
					p3 = FVector(cos(2 * PI * (double)(i + 1) / cylinder_obj->RadialVertices) * cylinder_obj->InnerCylinderD / 2, sin(2 * PI * (double)(i + 1) / cylinder_obj->RadialVertices) * cylinder_obj->InnerCylinderD / 2, 0);
					p1 += cylinder_pos;
					p2 += cylinder_pos;
					p3 += cylinder_pos;

					res1 = FVector::CrossProduct(p1 - p2, sphere_pos - p2);
					res2 = FVector::CrossProduct(p3 - p1, sphere_pos - p1);
					res3 = FVector::CrossProduct(p2 - p3, sphere_pos - p3);

					if (
						res1.Z <= 0 && res2.Z <= 0 && res3.Z <= 0 &&
						sphere_z < cylinder_z + cylinder_obj->CylinderH && sphere_z > cylinder_z + cylinder_obj->CylinderDelta
						)
					{
						in_cylinder = false;
					}
				}
			}
		}

		if (! in_cylinder)
		{
			GetWorld()->DestroyActor(sphere);
		}
	}
}

void ASphereFactory::Remove()
{
	TArray<AActor*> cylinders;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AACylinderFilter::StaticClass(), cylinders);

	for (auto cylinder : cylinders) {
		AACylinderFilter* cylinder_obj = Cast<AACylinderFilter>(cylinder);
		cylinder_obj->Remove();
	}

	TArray<AActor*> water_generators;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AWaterGenerator::StaticClass(), water_generators);

	for (auto water_generator : water_generators) {
		AWaterGenerator* water_generator_obj = Cast<AWaterGenerator>(water_generator);
		water_generator_obj->CreateWater();
	}
}

void ASphereFactory::createSpheres() {
	ASphereActor* sphere;
	float sqlen = (SpawnBoxElementsCount - 1) * SpawnDensity;

	for (int x = 0; x < SpawnBoxElementsCount; x++)
	{
		for (int y = 0; y < SpawnBoxElementsCount; y++)
		{
			for (int z = 0; z < SpawnBoxElementsCount; z++)
			{

				FActorSpawnParameters params;
				params.Owner = this;
				sphere = GetWorld()->SpawnActor<ASphereActor>(params);
				FVector sphere_static_size = sphere->VisualMesh->GetStaticMesh()->GetBoundingBox().GetSize();
				double real_sphere_d_tolerance = FMath::FRandRange(-SphereDiameterTolerance, SphereDiameterTolerance);
				double real_offset_tolerance = FMath::FRandRange(-SpawnOffsetsTolerance, SpawnOffsetsTolerance);

				sphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
				sphere->SetActorRelativeLocation(FVector(x * SpawnDensity + real_offset_tolerance - sqlen / 2, y * SpawnDensity + real_offset_tolerance - sqlen / 2, z * SpawnDensity + real_offset_tolerance - sqlen / 2));
				sphere->SetActorScale3D(FVector(SphereDiameter / (sphere_static_size.X + real_sphere_d_tolerance), SphereDiameter / (sphere_static_size.Y + real_sphere_d_tolerance), SphereDiameter / (sphere_static_size.Z + real_sphere_d_tolerance)));
				Spheres.push_back(sphere);
			}
		}
	}
}

