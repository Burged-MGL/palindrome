//
//  main.cpp
//  palindrome
//
//  Created by Hongyi Zhou on 3/9/21.
//  Copyright © 2021 Hongyi Zhou. All rights reserved.
//

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <cstring>


/* Palindrome has a great symmetry,
   so instead of looking at its start,
   we'd better look at its center.
   But for palindromes with even length,
   the center is the between two adjacent items,
   so we shall make it a concrete item.
 
   On the extended array, it suffices to look for
   longest palindromes centered at every item.
   A palindrome with an item as center has odd length,
   so we may define its radius by the length
   after the center till the end.
 
   We create a DP table to track
   the largest radius with each element as center,
   rolling from the start to the end of the array.
 
   Suppose we have filled up the table up to index i-1.
   Suppose currently the longest palindrome discovered
   (so with center C <= i-1) has radius R,
   then the longest palindrome centered at A[i]
   is either contained in the palindrome centered at C
   or exceeding that range.
 
   If it is contained, then the pattern of the
   longest palindrome centered at A[i] is the same
   as the one centered at A[2C-i].
   If it exceeds that range, then we initialize
   its radius as its distance to A[C+R],
   and we attempt the items to the right one by one,
   to find the farthest place we can go
   for the palindromes centered at A[i].
 
   Iterate that process until i = 2n+1.
 */


typedef struct {
  int start;
  int length;
} output;


/* Insert a character outside the alphabet
   to the gap between each adjacent pair.
   Runtime: O(n)
 */
char* extend(char *s, int n) {
  char *res = (char*) malloc(sizeof(char) * (2*n+1));
  for (int i = 0; i < n; i++) {
    res[2*i] = '#';
    res[2*i+1] = s[i];
  }
  res[2*n] = '#';
  return res;
}


int min(int a, int b) {
  if (a <= b) {
    return a;
  } else {
    return b;
  }
}

int max(int a, int b) {
  if (a >= b) {
    return a;
  } else {
    return b;
  }
}


/* Check if the indices accessed are
   within the proper range.
 */
bool valid(int len, int cen, int rad) {
  // valid center and radius
  return (rad <= len/2) && (cen < len - rad) && (cen >= rad);
}


/* Main work, create the DP table.
   Runtime: O(n)
 */
int* all_centered_palindromes(char *s, int n) {
  int len = 2 * n + 1;
  char *proc = extend(s, n);
  // printf("processed: %s\n", proc);
  int *result = (int*) calloc(len, sizeof(int));
  int currC = 0;
  int currR = 0;
  
  for (int i = 0; i < len; i++) {
    int isym = 2 * currC - i;
    if (currR > i) {
      result[i] = min(currR - i, result[isym]);
    } else {
      result[i] = 0;
    }
    
    /* will run into this loop only if
       previous process makes i + result[i] >= currR
       which forces currR to update,
       so the number of iterations in this loop
       is upper-bounded by the increment of currR;
       currR only goes up, bounded by 2n+1,
       so all total iterations <= 2n+1
     */
    while (valid(len, i, 1 + result[i])
           && proc[i+1+result[i]] == proc[i-1-result[i]]) {
      result[i]++;
    }
    if (i + result[i] > currR) {
      currR = i + result[i];
      currC = i;
    }
  }
  
  return result;
}


/* Find the longest over all centers.
   Runtime: O(n)
 */
output *find_longest_palindrome(char *s, int n) {
  //printf("input: %s\n", s);
  
  output *result = (output*) malloc(sizeof(output));
  result->start = 0;
  result->length = 1;
  int *all = all_centered_palindromes(s, n);
  int *temp = (int*) calloc(n, sizeof(int));
  
  int temp_start;
  int temp_length;
  for (int i = 1; i <= n; i++) {
    // all[2i-1] -> centered at s[i-1]
    temp_start = i - 1 - (all[2*i-1]/2);
    temp_length = all[2*i-1] + 1 - (all[2*i-1] & 1);
    temp[temp_start] = max(temp[temp_start], temp_length);
    
    // all[2i] -> centered between s[i-1] and s[i]
    temp_start = i - (all[2*i]/2);
    temp_length = all[2*i];
    temp[temp_start] = max(temp[temp_start], temp_length);
  }
  
  for (int j = 0; j < n; j++) {
    if (temp[j] > result->length) {
      result->start = j;
      result->length = temp[j];
    }
  }
  
  return result;
}




int main() {
  // insert code here...
  char buf[1000001];
  scanf("%s", buf);
  // printf("input taken\n");
  int len = (int) strlen(buf);
  // printf("length: %d\n", len);
  output *result = find_longest_palindrome(buf, len);
  printf("%d %d\n", result->start, result->length);
  return 0;
}
