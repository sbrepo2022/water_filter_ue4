// Fill out your copyright notice in the Description page of Project Settings.


#include "WaterGenerator.h"

// Sets default values
AWaterGenerator::AWaterGenerator()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = scene;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	mesh->SetupAttachment(scene);
	// Multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
	//mesh->bUseComplexAsSimpleCollision = false;

	start_spawn = false;
	spawn_time = 0.0f;
}

// Update visual every time property changed
void AWaterGenerator::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	CreateMesh(CylinderD, CylinderH, CylinderDelta, RadialVertices);
}

// This is called when actor is already in level and map is opened
void AWaterGenerator::PostLoad()
{
	Super::PostLoad();
	CreateMesh(CylinderD, CylinderH, CylinderDelta, RadialVertices);
}

// Called when the game starts or when spawned
void AWaterGenerator::BeginPlay() {
	Super::BeginPlay();

	UPrimitiveComponent* primitive_component = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (primitive_component)
		primitive_component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

	mesh->SetMeshSectionVisible(0, false);
}

// Called every frame
void AWaterGenerator::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (start_spawn) {
		if (last_generated > SpawnEvery) {
			float cur_radius = SpawnDensity;
			while (cur_radius < CylinderD / 2) {
				for (int i = 0; i < RadialVertices; i++) {
					FActorSpawnParameters params;
					params.Owner = this;
					AWaterActor* sphere = GetWorld()->SpawnActor<AWaterActor>(params);
					FVector sphere_static_size = sphere->VisualMesh->GetStaticMesh()->GetBoundingBox().GetSize();
					double real_sphere_d_tolerance = FMath::FRandRange(-SphereDiameterTolerance, SphereDiameterTolerance);
					double real_sphere_size = sphere_static_size.X + real_sphere_d_tolerance;
					double real_sphere_scale = SphereDiameter / real_sphere_size;

					sphere->AttachToComponent(GetRootComponent(), FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true));
					sphere->SetActorRelativeLocation(FVector(cos(2 * PI * (double)i / RadialVertices) * cur_radius, sin(2 * PI * (double)i / RadialVertices) * cur_radius, CylinderH - real_sphere_size - 1.0f));
					sphere->SetActorScale3D(FVector(real_sphere_scale, real_sphere_scale, real_sphere_scale));
					Spheres.push_back(sphere);
				}
				cur_radius += SpawnDensity;
			}

			last_generated = 0.0f;
		}

		spawn_time += DeltaTime;
		last_generated += DeltaTime;
		if (spawn_time > SpawnTime) {
			start_spawn = false;
		}
	}
}

void AWaterGenerator::CreateWater()
{
	start_spawn = true;
	spawn_time = 0.0f;
	last_generated = 0.0f;

	mesh->SetMeshSectionVisible(0, true);

	UPrimitiveComponent* primitive_component = Cast<UPrimitiveComponent>(GetComponentByClass(UPrimitiveComponent::StaticClass()));
	if (primitive_component)
		primitive_component->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Block);
}

void AWaterGenerator::CreateMesh(float cylinderD, float cylinderH, float cylinderDelta, int radialVertices)
{
	TArray<FVector> vertices;
	TArray<int32> Triangles;
	TArray<FVector> normals;
	TArray<FVector2D> UV0;
	TArray<FProcMeshTangent> tangents;
	TArray<FLinearColor> vertexColors;

	/*
		«десь € вычисл€ю геометрию объекта, однако дл€ упрощени€ алгоритма (пренебрега€ производительностью и возможностью плавного сглаживани€) € создаю свой
		набор вершин дл€ каждого треугольника (или пр€моугольника, если речь про боковую сторону), индексный буфер при этом используетс€ только чтобы отсортировать
		пор€док вершин в рамках одного треугольника/пр€моугольника дл€ правильного отображени€ текстуры.
	*/

	int vertices_count = 0;
	/* Generate bottom floor */
	/*for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(0, 0, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * cylinderD / 2, sin(2 * PI * (double)i / radialVertices) * cylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, cylinderDelta));

		Triangles.Add(vertices_count);
		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 1);

		UV0.Add(FVector2D(0, 0));
		UV0.Add(FVector2D(10, 0));
		UV0.Add(FVector2D(0, 10));

		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

		vertices_count += 3;
	}*/

	/* Generate top floor */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(0, 0, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * cylinderD / 2, sin(2 * PI * (double)i / radialVertices) * cylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, cylinderH));

		Triangles.Add(vertices_count);
		Triangles.Add(vertices_count + 1);
		Triangles.Add(vertices_count + 2);

		UV0.Add(FVector2D(0, 0));
		UV0.Add(FVector2D(10, 0));
		UV0.Add(FVector2D(0, 10));

		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

		vertices_count += 3;
	}

	/* Generate wall */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * cylinderD / 2, sin(2 * PI * (double)i / radialVertices) * cylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * cylinderD / 2, sin(2 * PI * (double)i / radialVertices) * cylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * cylinderD / 2, cylinderH));

		Triangles.Add(vertices_count);
		Triangles.Add(vertices_count + 1);
		Triangles.Add(vertices_count + 2);

		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 1);
		Triangles.Add(vertices_count + 3);

		UV0.Add(FVector2D(10, 0));
		UV0.Add(FVector2D(0, 0));
		UV0.Add(FVector2D(10, 10));
		UV0.Add(FVector2D(0, 10));

		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));
		vertexColors.Add(FLinearColor(0.75, 0.75, 0.75, 1.0));

		vertices_count += 4;
	}

	UKismetProceduralMeshLibrary::CalculateTangentsForMesh(vertices, Triangles, UV0, normals, tangents);
	mesh->CreateMeshSection_LinearColor(0, vertices, Triangles, normals, UV0, vertexColors, tangents, true);

	// Enable collision data
	mesh->ContainsPhysicsTriMeshData(true);
}


