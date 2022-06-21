// Fill out your copyright notice in the Description page of Project Settings.


#include "ACylinderFilter.h"

// Sets default values
AACylinderFilter::AACylinderFilter()
{
	scene = CreateDefaultSubobject<USceneComponent>(TEXT("SceneComponent"));
	RootComponent = scene;
	mesh = CreateDefaultSubobject<UProceduralMeshComponent>(TEXT("GeneratedMesh"));
	mesh->SetupAttachment(scene);
	// Multi-threaded PhysX cooking.
	mesh->bUseAsyncCooking = true;
}

// Update visual every time property changed
void AACylinderFilter::PostRegisterAllComponents()
{
	Super::PostRegisterAllComponents();
	CreateMesh(InnerCylinderD, OuterCylinderD, CylinderH, CylinderDelta, RadialVertices);
}

// This is called when actor is already in level and map is opened
void AACylinderFilter::PostLoad()
{
	Super::PostLoad();
	CreateMesh(InnerCylinderD, OuterCylinderD, CylinderH, CylinderDelta, RadialVertices);
}

void AACylinderFilter::BeginPlay() {
	Super::BeginPlay();

}

void AACylinderFilter::Remove()
{
	GetWorld()->DestroyActor(this);
}

void AACylinderFilter::CreateMesh(float innerCylinderD, float outerCylinderD, float cylinderH, float cylinderDelta, int radialVertices)
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

	/* Generate outer floor */
	int vertices_count = 0;
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(0, 0, 0));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, 0));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, 0));

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

	/* Generate inner floor */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(0, 0, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, cylinderDelta));

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
	}

	/* Generate outer wall */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, 0));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, 0));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, cylinderH));

		Triangles.Add(vertices_count);
		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 1);

		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 3);
		Triangles.Add(vertices_count + 1);

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

	/* Generate inner wall */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, cylinderDelta));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, cylinderH));

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

	/* Generate top border */
	for (int i = 0; i < radialVertices; i++) {
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * outerCylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * outerCylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)i / radialVertices) * innerCylinderD / 2, cylinderH));
		vertices.Add(FVector(cos(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, sin(2 * PI * (double)(i + 1) / radialVertices) * innerCylinderD / 2, cylinderH));

		Triangles.Add(vertices_count);
		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 1);

		Triangles.Add(vertices_count + 2);
		Triangles.Add(vertices_count + 3);
		Triangles.Add(vertices_count + 1);

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
