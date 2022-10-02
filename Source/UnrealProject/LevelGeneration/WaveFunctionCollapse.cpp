#include "WaveFunctionCollapse.h"

#include "Json.h"
#include "Misc/FileHelper.h"
#include "Engine/StaticMeshActor.h"
#include "Components/StaticMeshComponent.h"
#include "Engine/AssetManager.h" 
#include "Kismet/KismetSystemLibrary.h" 
#include "Kismet/GameplayStatics.h" 

void AWaveFunctionCollapse::BeginPlay()
{
	PlaceClouds();
}

bool AWaveFunctionCollapse::SolveGrid()
{
	GenerateBeginAndEndCoords();

	TArray<TArray<TArray<FTile>>> possibleTiles{};
	do
	{
		for (size_t i = 0; i < possibleTiles.Num(); i++)
		{
			for (size_t j = 0; j < possibleTiles[i].Num(); j++)
			{
				possibleTiles[i][j].Empty();
			}
			possibleTiles[i].Empty();
		}
		possibleTiles.Empty();

		SetupUpStartingGrid(possibleTiles);

		while (!IsSolved(possibleTiles))
		{
			int row, col; //coord of next tile to fill in
			GetLeastPossibilitiesTile(possibleTiles, row, col);
			FillInTile(possibleTiles, row, col);
			AdjustNeighbouringTiles(possibleTiles, row, col);
		}
	} while (!CheckAccessibility(possibleTiles)); //only allow solutions where every tile is accessible

	
	PlaceMeshes(possibleTiles);

	AdjustNavMeshVolume();

	return true;
}

void AWaveFunctionCollapse::CreateLevel()
{
	for (AActor* actor : PlacedMeshes)
	{
		actor->Destroy();
	}
	PlacedMeshes.Empty();

	SolveGrid();
}

void AWaveFunctionCollapse::CreateLevelStepThrough()
{
	for (AActor* actor : PlacedMeshes)
	{
		actor->Destroy();
	}
	PlacedMeshes.Empty();

	StepThroughArray.Empty();
	SetupUpStartingGrid(StepThroughArray);
}

void AWaveFunctionCollapse::ExecuteGenerationStep()
{
	if (!IsSolved(StepThroughArray))
	{
		int row, col; //coord of next tile to fill in
		GetLeastPossibilitiesTile(StepThroughArray, row, col);
		FillInTile(StepThroughArray, row, col);
		PlaceMesh(row, col);
		AdjustNeighbouringTiles(StepThroughArray, row, col);
	}
}

bool AWaveFunctionCollapse::IsSolved(const TArray<TArray<TArray<FTile>>>& possibleOptions)
{
	for (size_t i = 0; i < possibleOptions.Num(); i++)
	{
		for (size_t j = 0; j < possibleOptions[i].Num(); j++)
		{
			if (possibleOptions[i][j].Num() != 1)
				return false;
		}
	}

	return true;
}

void AWaveFunctionCollapse::SetupUpStartingGrid(TArray<TArray<TArray<FTile>>>& possibleOptions)
{
	//Setup array sizes
	for (size_t i = 0; i < Rows; i++)
	{
		possibleOptions.Add({});
		for (size_t j = 0; j < Columns; j++)
		{
			possibleOptions[i].Add({});
		}
	}


	//Setup start options assuming the outer border is all walls
	for (size_t i = 0; i < Rows; i++)
	{
		for (size_t j = 0; j < Columns; j++)
		{
			if (possibleOptions[i][j].Num() == 0)
			{
				WallType Front = WallType::DEFAULT, Back = WallType::DEFAULT, Right = WallType::DEFAULT, Left = WallType::DEFAULT; 
				GetNeighbouringWallTypes(possibleOptions, i, j, Front, Back, Right, Left); 

				possibleOptions[i][j] = TileSet.FilterByPredicate([Front, Back, Right, Left](const FTile& tile) 
					{
						return (Front == WallType::DEFAULT || tile.SideTypes[Side::FRONT] == Front) && 
							(Back == WallType::DEFAULT || tile.SideTypes[Side::BACK] == Back) &&
							(Right == WallType::DEFAULT || tile.SideTypes[Side::RIGHT] == Right) && 
							(Left == WallType::DEFAULT || tile.SideTypes[Side::LEFT] == Left);
					});
			}
		}
	}


	//let start and end tile only have 1 open side
	auto predicate = [](const FTile& tile)
	{
		int openSides{ 0 };
		for (auto pair : tile.SideTypes)
		{
			if (pair.Value == WallType::OPEN)
				openSides++;
		}
		return openSides == 1;
	};
	possibleOptions[StartRow][StartColumn] = possibleOptions[StartRow][StartColumn].FilterByPredicate(predicate);
	possibleOptions[EndRow][EndColumn] = possibleOptions[EndRow][EndColumn].FilterByPredicate(predicate);
}

void AWaveFunctionCollapse::GenerateBeginAndEndCoords()
{
	//start and end coords on opposite sides
	int rand = FMath::RandRange(0, 3);
	switch (rand)
	{
	case 0:
		StartRow = 0;
		StartColumn = FMath::RandRange(0, Columns - 1);
		EndRow = Rows-1;
		EndColumn = FMath::RandRange(0, Columns - 1);
		break;
	case 1:
		StartRow = Rows-1;
		StartColumn = FMath::RandRange(0, Columns - 1);
		EndRow = 0;
		EndColumn = FMath::RandRange(0, Columns - 1);
		break;
	case 2:
		StartColumn = 0;
		StartRow = FMath::RandRange(0, Rows - 1);
		EndColumn = Columns-1;
		EndRow = FMath::RandRange(0, Rows - 1);
		break;
	case 3:
		StartColumn = Columns-1;
		StartRow = FMath::RandRange(0, Rows - 1);
		EndColumn = 0;
		EndRow = FMath::RandRange(0, Rows - 1);
		break;
	default:
		break;
	}

	if (StartTarget != nullptr)
	{
		FVector tileCenter{ BotLeft };
		tileCenter.X += StartRow * TileSize + (TileSize / 2);
		tileCenter.Y += StartColumn * TileSize + (TileSize / 2);
		StartTarget->SetActorLocation(tileCenter);
	}
	if (EndTarget != nullptr)
	{
		FVector tileCenter{ BotLeft };
		tileCenter.X += EndRow * TileSize + (TileSize / 2);
		tileCenter.Y += EndColumn * TileSize + (TileSize / 2);
		EndTarget->SetActorLocation(tileCenter);
	}

}

void AWaveFunctionCollapse::GetNeighbouringWallTypes(const TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col, WallType& front, WallType& back, WallType& right, WallType& left)
{
	//front
	if (row + 1 >= possibleOptions.Num()) // check bounds
		front = WallType::WALL; // border of the grid has to be a wall
	else if (possibleOptions[row + 1][col].Num() == 1)
		front = possibleOptions[row + 1][col][0].SideTypes[Side::BACK]; //front of current tile and back of the tile above must match
		//if tile above has multiple options front can be anything so we keep it default

	//back
	if (row - 1 < 0)
		back = WallType::WALL;
	else if (possibleOptions[row - 1][col].Num() == 1)
		back = possibleOptions[row - 1][col][0].SideTypes[Side::FRONT];

	//right
	if (col + 1 >= possibleOptions[0].Num())
		right = WallType::WALL;
	else if (possibleOptions[row][col + 1].Num() == 1)
		right = possibleOptions[row][col + 1][0].SideTypes[Side::LEFT];

	//left
	if (col - 1 < 0)
		left = WallType::WALL;
	else if (possibleOptions[row][col - 1].Num() == 1)
		left = possibleOptions[row][col - 1][0].SideTypes[Side::RIGHT];
}

void AWaveFunctionCollapse::GetLeastPossibilitiesTile(const TArray<TArray<TArray<FTile>>>& possibleOptions, int& row, int& col)
{
	TArray<TTuple<int, int>> coordsWithLeastPossibleTiles{};

	uint16 leastPosibilities{ uint16(-1)};
	//find all coord with the least amount of possibilities
	for (size_t i = 0; i < possibleOptions.Num(); i++)
	{
		for (size_t j = 0; j < possibleOptions[i].Num(); j++)
		{
			if (possibleOptions[i][j].Num() > 1 && leastPosibilities > possibleOptions[i][j].Num()) //new low
			{
				coordsWithLeastPossibleTiles.Empty(1);
				coordsWithLeastPossibleTiles.Add(TTuple<int,int>( i,j ));
				leastPosibilities = possibleOptions[i][j].Num();
			}
			else if (leastPosibilities == possibleOptions[i][j].Num()) //old low
			{
				coordsWithLeastPossibleTiles.Add(TTuple<int, int>(i, j));
			}
		}
	}

	//pick random coord from options
	int idx = FMath::RandRange(0, coordsWithLeastPossibleTiles.Num() - 1);
	row = coordsWithLeastPossibleTiles[idx].Key;
	col = coordsWithLeastPossibleTiles[idx].Value;
}

void AWaveFunctionCollapse::FillInTile(TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col)
{
	auto nonClosingPossibilities = possibleOptions[row][col].FilterByPredicate([](const FTile& tile)
		{
			int openSides{};
			for (const auto& pair : tile.SideTypes)
			{
				if (pair.Value != WallType::WALL)
					openSides++;
			};
			return openSides > 1;
		}); //tiles with atleast 2 open sides to prevent closed of segments

	FTile selectedTile;
	if (nonClosingPossibilities.Num() == 0) //if no tiles with multiple open sides fit, use all tiles
	{
		nonClosingPossibilities = possibleOptions[row][col];
	}


	//weighted random selection
	float totalWeight{ 0.f };

	for (size_t i = 0; i < nonClosingPossibilities.Num(); i++)
	{
		totalWeight += nonClosingPossibilities[i].TileWeight;
	}

	float rand = FMath::FRandRange(0, totalWeight);
	float weightSoFar{ 0.f };
	for (size_t i = 0; i < nonClosingPossibilities.Num(); i++)
	{
		weightSoFar += nonClosingPossibilities[i].TileWeight;
		if (rand < weightSoFar)
		{
			selectedTile = nonClosingPossibilities[i];
			break;
		}
	}
	
	possibleOptions[row][col].Empty(1);
	possibleOptions[row][col].Add(selectedTile);
}

void AWaveFunctionCollapse::AdjustNeighbouringTiles(TArray<TArray<TArray<FTile>>>& possibleOptions, int row, int col)
{
	int rowToCheck{};
	int colToCheck{};
	for (int i = 0; i < 4; i++)
	{
		switch (i)
		{
		case 0:
			rowToCheck = 0;
			colToCheck = -1;
			break;
		case 1:
			rowToCheck = 0;
			colToCheck = 1;
			break;
		case 2:
			rowToCheck = -1;
			colToCheck = 0;
			break;
		case 3:
			rowToCheck = 1;
			colToCheck = 0;
			break;
		default:
			break;
		}
		if (row + rowToCheck < 0 || col + colToCheck < 0 || row + rowToCheck >= possibleOptions.Num() || col + colToCheck >= possibleOptions[row + rowToCheck].Num() || possibleOptions[row + rowToCheck][col + colToCheck].Num() == 1)
			continue;

		WallType Front = WallType::DEFAULT, Back = WallType::DEFAULT, Right = WallType::DEFAULT, Left = WallType::DEFAULT;
		GetNeighbouringWallTypes(possibleOptions, row + rowToCheck, col + colToCheck, Front, Back, Right, Left);


		int possibilitiesBefore{ possibleOptions[row + rowToCheck][col + colToCheck].Num()};
		possibleOptions[row + rowToCheck][col + colToCheck] = possibleOptions[row + rowToCheck][col + colToCheck].FilterByPredicate([Front, Back, Right, Left](const FTile& tile)
			{
				return (Front == WallType::DEFAULT || tile.SideTypes[Side::FRONT] == Front) &&
					(Back == WallType::DEFAULT || tile.SideTypes[Side::BACK] == Back) &&
					(Right == WallType::DEFAULT || tile.SideTypes[Side::RIGHT] == Right) &&
					(Left == WallType::DEFAULT || tile.SideTypes[Side::LEFT] == Left);				
			});

		if (possibilitiesBefore > possibleOptions[row + rowToCheck][col + colToCheck].Num()) //if possibilities are narrowed down for this tile
			AdjustNeighbouringTiles(possibleOptions, row + rowToCheck, col + colToCheck);    //adjust its neighbours
	}
}

void AWaveFunctionCollapse::AddConnectedCells(TArray<TArray<TArray<FTile>>>& possibleOptions, TArray<TArray<bool>>& connected, int row, int col)
{
	//recursive function to get all tiles that are connected
	auto currentTile = possibleOptions[row][col][0];
	connected[row][col] = true;
	for (const auto& pair : currentTile.SideTypes)
	{
		if (pair.Value == WallType::OPEN)
		{
			switch (pair.Key)
			{
			case Side::FRONT:
				if (!connected[row + 1][col])
					AddConnectedCells(possibleOptions, connected, row + 1, col);
				break;
			case Side::BACK:
				if (!connected[row - 1][col])
					AddConnectedCells(possibleOptions, connected, row - 1, col);
				break;
			case Side::RIGHT:
				if (!connected[row][col + 1])
					AddConnectedCells(possibleOptions, connected, row, col + 1);
				break;
			case Side::LEFT:
				if (!connected[row][col - 1])
					AddConnectedCells(possibleOptions, connected, row, col - 1);
				break;
			default:
				break;
			}
		}
	}
}

bool AWaveFunctionCollapse::CheckAccessibility(TArray<TArray<TArray<FTile>>>& possibleOptions)
{
	TArray<TArray<bool>> connected{};
	for (size_t i = 0; i < possibleOptions.Num(); i++)
	{
		connected.Add(TArray<bool>{});
		for (size_t j = 0; j < possibleOptions[i].Num(); j++)
		{
			connected[i].Add(false);
		}
	}

	AddConnectedCells(possibleOptions, connected, 0, 0); //recursively adds connected tiles

	for (size_t i = 0; i < connected.Num(); i++)
	{
		for (size_t j = 0; j < connected[i].Num(); j++)
		{
			if (!connected[i][j])
				return false; //false means unaccessible
		}
	}
	return true;
}

void AWaveFunctionCollapse::PlaceMeshes(TArray<TArray<TArray<FTile>>>& possibleOptions)
{
	for (size_t i = 0; i < possibleOptions.Num(); i++)
	{
		for (size_t j = 0; j < possibleOptions[i].Num(); j++)
		{
			FVector tileCenter{ BotLeft };
			tileCenter.X += i * TileSize + (TileSize / 2);
			tileCenter.Y += j * TileSize + (TileSize / 2);

			if (possibleOptions[i][j].Num() == 0)
				continue;

			auto actor = GetWorld()->SpawnActor<AStaticMeshActor>(tileCenter, FRotator{0,0,0}, FActorSpawnParameters{});
			actor->Tags.Add(FName("Tile"));
			PlacedMeshes.Add(actor);

			auto meshComp = actor->GetStaticMeshComponent();
			if (meshComp != nullptr)
			{
				if (possibleOptions[i][j][0].Meshes.Num() != possibleOptions[i][j][0].MeshWeights.Num()) // weights intialised to 1 if not set
					possibleOptions[i][j][0].MeshWeights.Init(1.f, possibleOptions[i][j][0].Meshes.Num());

				//weighted random
				float totalWeight{ 0.f };
				for (size_t k = 0; k < possibleOptions[i][j][0].MeshWeights.Num(); k++)
				{
					totalWeight += possibleOptions[i][j][0].MeshWeights[k];
				}

				float rand = FMath::FRandRange(0, totalWeight);
				float weightSoFar{ 0.f };
				for (size_t k = 0; k < possibleOptions[i][j][0].MeshWeights.Num(); k++)
				{
					weightSoFar += possibleOptions[i][j][0].MeshWeights[k];
					if (rand < weightSoFar)
					{
						meshComp->SetStaticMesh(Cast<UStaticMesh>(possibleOptions[i][j][0].Meshes[k].TryLoad()));
						break;
					}
				}

				meshComp->SetGenerateOverlapEvents(true);
			}
		}
	}
}

void AWaveFunctionCollapse::PlaceMesh(int row, int col)
{
	FVector tileCenter{ BotLeft };
	tileCenter.Y += row * TileSize + (TileSize / 2);
	tileCenter.X += col * TileSize + (TileSize / 2);
	auto actor = GetWorld()->SpawnActor<AStaticMeshActor>(tileCenter, FRotator{ 0,0,0 }, FActorSpawnParameters{});
	PlacedMeshes.Add(actor);

	auto meshComp = actor->GetStaticMeshComponent();
	if (meshComp != nullptr)
	{
		if (StepThroughArray[row][col][0].Meshes.Num() != StepThroughArray[row][col][0].MeshWeights.Num()) //weights set to 1 if they arent set
			StepThroughArray[row][col][0].MeshWeights.Init(1.f, StepThroughArray[row][col][0].Meshes.Num());

		//weigthed random
		float totalWeight{ 0.f };
		for (size_t k = 0; k < StepThroughArray[row][col][0].MeshWeights.Num(); k++)
		{
			totalWeight += StepThroughArray[row][col][0].MeshWeights[k];
		}

		float rand = FMath::FRandRange(0, totalWeight);
		float weightSoFar{ 0.f };
		for (size_t k = 0; k < StepThroughArray[row][col][0].MeshWeights.Num(); k++)
		{
			weightSoFar += StepThroughArray[row][col][0].MeshWeights[k];
			if (rand < weightSoFar)
			{
				meshComp->SetStaticMesh(Cast<UStaticMesh>(StepThroughArray[row][col][0].Meshes[k].TryLoad()));
				break;
			}
		}

		meshComp->SetGenerateOverlapEvents(true);
	}
}

void AWaveFunctionCollapse::AdjustNavMeshVolume()
{
	if (NavMeshBounds == nullptr)
		return;
	FVector center{ BotLeft };
	center.X += (Rows * TileSize) / 2;
	center.Y += (Columns * TileSize) / 2;
	NavMeshBounds->SetActorLocation(center);
	NavMeshBounds->SetActorScale3D(FVector(Rows * TileSize, Columns * TileSize, 10000.f));
}

void AWaveFunctionCollapse::PlaceClouds()
{
	float currentX = BotLeft.X - TileSize;
	float endX = BotLeft.X + (Columns+1) * TileSize;
	float currentY = BotLeft.Y - TileSize;
	float endY = BotLeft.Y + (Rows+1) * TileSize;

	for (currentX; currentX < endX; currentX += CloudSpacing)
	{
		for (currentY; currentY < endY; currentY += CloudSpacing)
		{
			GetWorld()->SpawnActor<AActor>(CloudActor, FVector( currentX, currentY, BotLeft.Z ), FRotator( 0, FMath::FRandRange(0, 360), 0));
		}
		currentY = BotLeft.Y - TileSize;
	}
}

