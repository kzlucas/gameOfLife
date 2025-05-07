#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "grid.h"


static Vector2Int matrixSize = { 0,0 };
int* prevFrameNodesPointer = NULL;
int* nodesPointer = NULL;



void SetMatrixSize(Vector2Int newSize)
{
	matrixSize = newSize;
    FreeMem();
    AllocateMemory();
    srand(time(0)); // seed rand's
}


int GetNode(Vector2Int coords)
{
    int offset = CoordsToPointerOffset(coords);
    return nodesPointer[offset] == 1;
}


int GetPrevNode(Vector2Int coords)
{
    if (coords.x < 0 || coords.y < 0 || coords.x >= matrixSize.x || coords.y >= matrixSize.y) {
        return 0;
    }
    int offset = CoordsToPointerOffset(coords);
    return prevFrameNodesPointer[offset] == 1;
}


void SetNode(Vector2Int coords, int value)
{
    int offset = CoordsToPointerOffset(coords);
    nodesPointer[offset] = value;
}


void GetNodesNextStates()
{
    SavePrevMatrix();
    for (int i = 0; i < matrixSize.x; i++)
    {
        for (int j = 0; j < matrixSize.y; j++)
        {
            Vector2Int node = { i, j };
            int aliveNeighbors = GetAliveNeighbors(node);

            // if is dead and has 3 alive neighbors
            if ((GetPrevNode(node) == 0) && (aliveNeighbors == 3))
            {
                SetNode(node, 1); // birth
            }
            // if is alive and has not 2 or 3 alive neighbors
            else if ((GetPrevNode(node) == 1)
                && !((aliveNeighbors == 2) || (aliveNeighbors == 3)))
            {
                SetNode(node, 0); // death
            }
        }
    }
}   


static int GetAliveNeighbors(Vector2Int coords)
{
    int out = 0;

    Vector2Int tln = { coords.x - 1, coords.y + 1 };
    out += GetPrevNode(tln);

    Vector2Int tn = { coords.x, coords.y + 1 };
    out += GetPrevNode(tn);

    Vector2Int trn = { coords.x + 1, coords.y + 1 };
    out += GetPrevNode(trn);

    Vector2Int ln = { coords.x - 1, coords.y };
    out += GetPrevNode(ln);

    Vector2Int rn = { coords.x + 1, coords.y };
    out += GetPrevNode(rn);

    Vector2Int bln = { coords.x - 1, coords.y - 1 };
    out += GetPrevNode(bln);

    Vector2Int bn = { coords.x, coords.y - 1 };
    out += GetPrevNode(bn);

    Vector2Int brn = { coords.x + 1, coords.y - 1 };
    out += GetPrevNode(brn);

    return out;
}


static int CoordsToPointerOffset(Vector2Int coords)
{
    int offset = 0;
    offset += coords.y * matrixSize.x;
    offset += coords.x;
    return offset;
}


void RandomizeNodeStates() {

    ClearMatrix();
    for (int i = 0; i < matrixSize.x; i++)
    {
        for (int j = 0; j < matrixSize.y; j++)
        {
            if (rand() & 1 && rand() & 1) // 1/8
            {
                Vector2Int coords = { i, j };
                SetNode(coords, 1);
            }
        }
    }
};


void ClearMatrix(){

    for (int i = 0; i < matrixSize.x; i++)
    {
        for (int j = 0; j < matrixSize.y; j++)
        {
             Vector2Int coords = { i, j };
             SetNode(coords, 0);
        }
    }
    SavePrevMatrix();
}



void SavePrevMatrix()
{
    for (int i = 0; i < matrixSize.x; i++)
    {
        for (int j = 0; j < matrixSize.y; j++)
        {
            Vector2Int coords = { i, j };
            int offset = CoordsToPointerOffset(coords);
            prevFrameNodesPointer[offset] = GetNode(coords);
        }
    }
}

void AllocateMemory()
{
    int nodesMallocSize = sizeof(Vector2Int) * matrixSize.x * matrixSize.y;
    nodesPointer = malloc(nodesMallocSize);
    prevFrameNodesPointer = malloc(nodesMallocSize);
    if (nodesPointer == NULL || prevFrameNodesPointer == NULL)
    {
        printf("Mem allocation fails\n");
        exit(0);
    }
}


void FreeMem()
{
    free(nodesPointer);
    free(prevFrameNodesPointer);
}