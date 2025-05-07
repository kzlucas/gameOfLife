#pragma once

typedef struct Vector2Int Vector2Int;
struct Vector2Int
{
    int x;
    int y;
};


void SetMatrixSize(Vector2Int newSize);

void SetNode(Vector2Int coords, int value);

int GetNode(Vector2Int coords);

int CoordsToPointerOffset(Vector2Int coords);

void AllocateMemory();

void FreeMem();

void RandomizeNodeStates();

void GetNodesNextStates();

void SavePrevMatrix();

void ClearMatrix();

static int GetAliveNeighbors(Vector2Int coords);