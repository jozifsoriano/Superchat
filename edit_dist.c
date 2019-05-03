#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define min2(a, b) ((a) < (b) ? (a) : (b))
#define min(a, b, c) (min2(min2((a), (b)), (c)))
int edit_distance(char * first_string, char * second_string)
{
    int first_length = strlen(first_string);
    int second_length = strlen(second_string);
    int i, j, k, l, delta, x;
    int A[first_length+1][second_length+1];
    for( i=0; i<first_length+1; i++)
        for( j=0; j<second_length+1; j++)
            A[i][j]=0;

    for(i=0; i<first_length+1; i++)
        A[i][0] = i;
    for(j=0; j<second_length+1; j++)
        A[0][j] = j;
    for(i=1; i<first_length+1; i++)
    {
        for(j=1; j<second_length+1; j++)
        {
            if(first_string[i-1] == second_string[j-1])
            {
                delta = 0;
            }
            else
            {
                delta = 1;
            }
            A[i][j] = min(A[i-1][j-1]+delta, A[i-1][j] +1, A[i][j-1] +1);
        }
    }
    return A[first_length][second_length];
}
