#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>
#include <unistd.h> // For usleep

#define FPS 5 // Frames per second

void clear_screen() {
    printf("\033[H\033[J");
}

void wait_for_next_frame(clock_t start_time) {
    clock_t end_time = start_time + CLOCKS_PER_SEC / FPS;
    while (clock() < end_time) {
        // Busy-wait until the desired time has passed
    }
}
void printMat(int **cells, int rows, int cols){
  for (int i=0;i<rows;++i) {
    for (int j = 0;j<cols; ++j) {
      printf("%d, ", cells[i][j]);
    }
    printf("\n");
  }
}
bool withinBounds(int rows, int cols, int i, int j){
  return i>=0 && i<rows && j>=0 && j<cols;
}

int **fillMat(int **cells, int X, int Y, int h, int w, int H, int W){
  
  int **matrix = calloc(h, sizeof(int *));  // Allocate array of row pointers
  for (int i = 0; i < h; i++) {
    matrix[i] = calloc(w, sizeof(int));   // Allocate each row
  }
  for (int i=0+X;i<h;i++) {
    for (int j=0+Y;j<w;j++) {
      if(withinBounds(H, W, i-X, j-Y)) matrix[i][j] = cells[i-X][j-Y];
    }
  }
  return matrix;
}

int **get_generation (int **cells, int generations, int *rowptr, int *colptr)
{
  int x = 0, X =0, y =0, Y=0;
  for (int i=0;i<*colptr; i++){
    if (X >= 3);
    else{
      X = (X + cells[0][i])*cells[0][i];
    }
    if (x>=3);
    else{
      x = (x + cells[*rowptr - 1][i])*cells[*rowptr - 1][i];
    }
  }
  for (int i=0;i<*rowptr; i++){
    if (Y>=3);
    else{
      Y = (Y + cells[i][0])*cells[i][0];
    }
    if(y>=3);
    else{
      y = (y + cells[i][*colptr-1])*cells[i][*colptr-1];
    }
  }
  int h=*rowptr, w=*colptr;
  if (X>=3) *rowptr+=1, X=1;
  if (x>=3) *rowptr+=1, x=1;
  if (Y>=3) *colptr+=1, Y=1;
  if (y>=3) *colptr+=1, y=1;
  
  if(X==1 || x==1 || Y==1 || y==1){
    //get_generation(fillMat(cells,X,Y,*rowptr,*colptr, h, w),generations,rowptr,colptr);
    int **new_matrix = fillMat(cells, X, Y, *rowptr, *colptr, h, w);
    int **result = get_generation(new_matrix, generations, rowptr, colptr);
    
    for (int i = 0; i < h; i++) {
        free(new_matrix[i]);
    }
    free(new_matrix);
    
    return result;
  }else{
    int **matrix = calloc(*rowptr, sizeof(int *));  // Allocate array of row pointers
    for (int i = 0; i < *rowptr; i++) {
      matrix[i] = calloc(*colptr, sizeof(int));   // Allocate each row
    }

    for (int i = 0;i<(*rowptr);i++) {
      for (int j=0;j<(*colptr);j++) {
        int neighbors = 0;
        if(withinBounds(*rowptr, *colptr, i-1, j) && cells[i-1][j] == 1)    neighbors++;
        if(withinBounds(*rowptr, *colptr, i+1, j) && cells[i+1][j] == 1)    neighbors++;

        if(withinBounds(*rowptr, *colptr, i, j-1) && cells[i][j-1] == 1)    neighbors++;
        if(withinBounds(*rowptr, *colptr, i, j+1) && cells[i][j+1] == 1)    neighbors++;

        if(withinBounds(*rowptr, *colptr, i-1, j-1) && cells[i-1][j-1] == 1)    neighbors++;
        if(withinBounds(*rowptr, *colptr, i+1, j+1) && cells[i+1][j+1] == 1)    neighbors++;
          
        if(withinBounds(*rowptr, *colptr, i-1, j+1) && cells[i-1][j+1] == 1)    neighbors++;
        if(withinBounds(*rowptr, *colptr, i+1, j-1) && cells[i+1][j-1] == 1)    neighbors++;

        if (cells[i][j]==1) {
          if (neighbors == 2 || neighbors ==3) {
            matrix[i][j]=1;
          }else {
            matrix[i][j]=0;
          }
        }else if (cells[i][j] == 0)
        {
          if (neighbors == 3) {
            matrix[i][j] = 1;
          }else {
            matrix[i][j] = 0;
          }
        }
      }
    }
    
    if(generations == 1)  return matrix;
  
    get_generation(matrix, generations - 1, rowptr, colptr);
    
  }
}

char *htmlize(int **cells, int rows, int cols)
{
  char *str = malloc((rows + 1) * cols * sizeof "▓▓" + 1), *ptr = str;
  for (int row = 0; row < rows; row++) {
    for (int col = 0; col < cols; col++)
      ptr += sprintf(ptr, "%s", cells[row][col] ? "▓▓" : "░░");
    *(ptr++) = '\n';
  }
  *ptr = '\0';
  return str;
}

int main(int argc, char *argv[])
{
  int rows=11, cols = 37, frame =1;
  int *gen_0[] = {
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
    (int[]){0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};
  char *universe = htmlize(gen_0, rows, cols);
  printf("%s", universe);
  free(universe);
 
  /*
  int **test = get_generation(gen_0, 9, &rows, &cols);
  char *universee = htmlize(test, rows, cols);
  printf("%s \n", universee);
  free(universee); 
*/
  
  while (1) {
    clock_t start_time = clock();
    rows = 11;
    cols = 37;

    int **test = get_generation(gen_0, frame, &rows, &cols);
    char *universee = htmlize(test, rows, cols);
    
    clear_screen();
    printf("Frame %d\n", frame);
    printf("%s \n", universee);
    
    free(universee);
    for (int i = 0; i < rows; i++) free(test[i]);
    free(test);
    
    frame++; 
    wait_for_next_frame(start_time);
  }
  return 1;
}

