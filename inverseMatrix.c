#include "inverseMatrix.h"
#include <stdlib.h>
#include <stdio.h>
#include <math.h>

int initQMatrix(QMatrix* matrix, const unsigned int dimension)
{
  int iMatrixRow;

  matrix->dimension = dimension;
  matrix->elements = malloc(sizeof(float*) * dimension);
  for(iMatrixRow = 0; iMatrixRow < dimension; iMatrixRow++){
    matrix->elements[iMatrixRow] = malloc(sizeof(float) * dimension);
  }

  return 0;
}

int qMatrixScanf(QMatrix* matrix)
{
  int iMatrixRow;
  int jMatrixElement;

  for(iMatrixRow = 0; iMatrixRow < matrix->dimension; iMatrixRow++){
    for(jMatrixElement = 0;
        jMatrixElement < matrix->dimension;
        jMatrixElement++){
      scanf("%f", &(matrix->elements[iMatrixRow][jMatrixElement]));
    }
  }

  return 0;
}

int qMatrixPrintf(QMatrix matrix)
{
  int iMatrixRow;
  int jMatrixElement;

  for(iMatrixRow = 0; iMatrixRow < matrix.dimension; iMatrixRow++){
    for(jMatrixElement = 0;
        jMatrixElement < matrix.dimension;
        jMatrixElement++){
      if(jMatrixElement == matrix.dimension - 1){
        printf("%f", matrix.elements[iMatrixRow][jMatrixElement]);
      }
      else{
        printf("%f ", matrix.elements[iMatrixRow][jMatrixElement]);
      }
    }
    printf("\n");
  }

  return 0;
}

int qMatrixSwapRows(QMatrix* matrix,
                    const unsigned int row1,
                    const unsigned int row2)
{
  int   iMatrixElement;
  float buffer;

  for(iMatrixElement = 0;
      iMatrixElement < matrix->dimension;
      iMatrixElement++){
    buffer = matrix->elements[row1][iMatrixElement];
    matrix->elements[row1][iMatrixElement] =
      matrix->elements[row2][iMatrixElement];
    matrix->elements[row2][iMatrixElement] = buffer;
  }

  return 0;
}

int qMatrixSwapColumns(QMatrix* matrix,
                       const unsigned int column1,
                       const unsigned int column2)
{
  int   iMatrixElement;
  float buffer;

  for(iMatrixElement = 0;
      iMatrixElement < (*matrix).dimension;
      iMatrixElement++){
    buffer = matrix->elements[iMatrixElement][column1];
    matrix->elements[iMatrixElement][column1] =
      matrix->elements[iMatrixElement][column2];
    matrix->elements[iMatrixElement][column2] = buffer;
  }

  return 0;
}

float qMatrixMainElementLQ(QMatrix matrix,
                         const unsigned int limiter,
                         int* mainElementRow,
                         int* mainElementColumn)
{
  int iMatrixRow;
  int jMatrixElement;
  float mainElement;

  *mainElementRow = *mainElementColumn = limiter;
  mainElement = abs(matrix.elements[limiter][limiter]);

  for(iMatrixRow = limiter ; iMatrixRow < matrix.dimension; iMatrixRow++){
    for(jMatrixElement = limiter;
        jMatrixElement < matrix.dimension;
        jMatrixElement++){
      if(mainElement < abs(matrix.elements[iMatrixRow][jMatrixElement])){
        mainElement = abs(matrix.elements[iMatrixRow][jMatrixElement]);
        *mainElementRow = iMatrixRow;
        *mainElementColumn = jMatrixElement;
      }
    }
  }
  return mainElement;
}

float* qMatrixExtractRow (QMatrix matrix, const unsigned int row)
{
  float* extractedRow;
  int iMatrixElement;

  extractedRow = malloc(sizeof(float) * matrix.dimension);

  for(iMatrixElement = 0; iMatrixElement < matrix.dimension; iMatrixElement++){
    extractedRow[iMatrixElement] = matrix.elements[row][iMatrixElement];
  }

  return extractedRow;
}

int multiplyRow(float* row, float scalar, int dimension)
{
  int iRowElement;

  for(iRowElement = 0; iRowElement < dimension; iRowElement++){
    row[iRowElement] *= scalar;
  }

  return 0;
}

int subtractRowQMatrix(QMatrix matrix,
                       int minuendRow,
                       float* subtrahend,
                       int limiter)
{
  int iMatrixElement;

  for(iMatrixElement = limiter;
      iMatrixElement < matrix.dimension;
      iMatrixElement++){
    matrix.elements[minuendRow][iMatrixElement] -= subtrahend[iMatrixElement];
  }

  return 0;
}

int initEQMatrix(QMatrix* matrix, const unsigned int dimension)
{
  int iMatrixRow;
  int jMatrixElement;

  initQMatrix(matrix, dimension);
  for(iMatrixRow = 0; iMatrixRow < dimension; iMatrixRow++){
    for(jMatrixElement = 0; jMatrixElement < dimension; jMatrixElement++){
      if(iMatrixRow == jMatrixElement){
        matrix->elements[iMatrixRow][jMatrixElement] = 1;
      }
      else{
        matrix->elements[iMatrixRow][jMatrixElement] = 0;
      }
    }
  }

  return 0;
}

QMatrix reverseMatrix(QMatrix matrix)
{
  QMatrix result;
  initEQMatrix(&result, matrix.dimension);
  int iMatrixRow;
  int jMatrixRow;
  int jMatrixElement;
  int mainElementRow;
  int mainElementColumn;

  /* Pass I */
  for(iMatrixRow = 0; iMatrixRow < matrix.dimension; iMatrixRow++){
    qMatrixMainElementLQ(matrix,
                         iMatrixRow,
                         &mainElementRow,
                         &mainElementColumn);
    if(mainElementRow != iMatrixRow){
      qMatrixSwapRows(&matrix, iMatrixRow, mainElementRow);
      qMatrixSwapRows(&result, iMatrixRow, mainElementRow);
    }
    if(mainElementColumn != iMatrixRow){
      qMatrixSwapRows(&matrix, iMatrixRow, mainElementColumn);
      qMatrixSwapRows(&result, iMatrixRow, mainElementColumn);
    }

    for(jMatrixRow = iMatrixRow+1; jMatrixRow < matrix.dimension; jMatrixRow++){
      float* buffer;
      float k = matrix.elements[jMatrixRow][iMatrixRow]/
                    matrix.elements[iMatrixRow][iMatrixRow];
      buffer = qMatrixExtractRow(matrix, iMatrixRow);
      multiplyRow(buffer, k, matrix.dimension);
      subtractRowQMatrix(matrix, jMatrixRow, buffer, iMatrixRow);
      free(buffer);

      buffer = qMatrixExtractRow(result, iMatrixRow);
      multiplyRow(buffer, k, matrix.dimension);
      subtractRowQMatrix(result, jMatrixRow, buffer, 0);
      free(buffer);
    }
  }

  // Pass II
  for(iMatrixRow = 0; iMatrixRow < matrix.dimension; iMatrixRow++){
    float d = matrix.elements[iMatrixRow][iMatrixRow];
    for(jMatrixElement = 0;
        jMatrixElement < matrix.dimension;
        jMatrixElement++){
      matrix.elements[iMatrixRow][jMatrixElement] /= d;
      result.elements[iMatrixRow][jMatrixElement] /= d;
    }
  }
  
  // Pass III 
  for(iMatrixRow = matrix.dimension-1; iMatrixRow >= 0; iMatrixRow--){
    for(jMatrixRow = iMatrixRow-1; jMatrixRow >= 0; jMatrixRow--){
      float* buffer;
      float k = matrix.elements[jMatrixRow][iMatrixRow];
      buffer = qMatrixExtractRow(matrix, iMatrixRow);
      multiplyRow(buffer, k, matrix.dimension);
      subtractRowQMatrix(matrix, jMatrixRow, buffer, 0);
      free(buffer);

      buffer = qMatrixExtractRow(result, iMatrixRow);
      multiplyRow(buffer, k, matrix.dimension);
      subtractRowQMatrix(result, jMatrixRow, buffer, 0);
      free(buffer);
    }
  }
  
  return result;
}
