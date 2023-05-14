#pragma once
#include "CoreMinimal.h"
#include "../CloudActor.h"
#include "NavMesh/NavMeshBoundsVolume.h" 

#include "WaveFunctionCollapse.generated.h"

UENUM(BlueprintType)
enum class Side : uint8
{
	FRONT, // +X
	BACK, // -X
	RIGHT, // +Y
	LEFT // -Y
};

UENUM(BlueprintType)
enum class WallType : uint8
{
	OPEN,
	WALL,
	DEFAULT
};

USTRUCT(Blueprintable, BlueprintType)
struct UNREALPROJECT_API FTile
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FSoftObjectPath> Meshes;
	UPROPERTY(EditAnywhere)
	TArray<float> MeshWeights;
	UPROPERTY(EditAnywhere)
	float TileWeight = 1;
	UPROPERTY(EditAnywhere)
	TMap<Side, WallType> SideTypes{ {{Side::FRONT, WallType::DEFAULT}, {Side::BACK, WallType::DEFAULT}, {Side::RIGHT, WallType::DEFAULT}, {Side::LEFT, WallType::DEFAULT}}};
};


UCLASS()
class UNREALPROJECT_API AWaveFunctionCollapse : public AActor //todo let inherit from uobject and make part of gamemode
{
	GENERATED_BODY()
public:

	void BeginPlay() override;

	//Main loop
	bool SolveGrid();

	UFUNCTION(CallInEditor)
	void CreateLevel();
	UFUNCTION(CallInEditor)
	void CreateLevelStepThrough();
	UFUNCTION(CallInEditor)
	void ExecuteGenerationStep();	

private:
	
	bool IsSolved(const TArray<TArray<TArray<FTile>>>& possibleOptions) const;

	void SetupUpStartingGrid(TArray<TArray<TArray<FTile>>>& possibleOptions) const;

	void GenerateBeginAndEndCoords();

	void GetNeighbouringWallTypes(const TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col, WallType& front, WallType& back, WallType& right, WallType& left) const;

	void GetLeastPossibilitiesTile(const TArray<TArray<TArray<FTile>>>& possibleOptions, int& row, int& col) const;

	void FillInTile(TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col) const;

	void AdjustNeighbouringTiles(TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col) const;

	bool CheckAccessibility(TArray<TArray<TArray<FTile>>>& possibleOptions) const;
	void AddConnectedCells(TArray<TArray<TArray<FTile>>>& possibleOptions, TArray<TArray<bool>>& connected, int row, int col) const;

	void PlaceMeshes(TArray<TArray<TArray<FTile>>>& possibleOptions);
	void PlaceMesh(int row, int col);

	void AdjustNavMeshVolume();

	void PlaceClouds() const;
	UPROPERTY(EditAnywhere)
	TSubclassOf<ACloudActor> CloudActor;
	UPROPERTY(EditAnywhere)
	float CloudSpacing = 1000.f;

	UPROPERTY(EditAnywhere)
	TArray<FTile> TileSet;
	UPROPERTY(EditAnywhere)
	float TileSize = 2000.f;
	UPROPERTY(EditAnywhere)
	FVector BotLeft = {0,0,0};
	UPROPERTY(EditAnywhere)
	int Columns;
	UPROPERTY(EditAnywhere)
	int Rows;
	int StartRow;
	int StartColumn;
	int EndRow;
	int EndColumn;
	TArray<AActor*> PlacedMeshes;
	TArray<TArray<TArray<FTile>>> StepThroughArray{};
	UPROPERTY(EditAnywhere)
	ANavMeshBoundsVolume* NavMeshBounds = nullptr;
	UPROPERTY(EditAnywhere)
	AActor* StartTarget = nullptr;
	UPROPERTY(EditAnywhere)
	AActor* EndTarget = nullptr;
};

