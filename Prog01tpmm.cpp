//******************************************************************************
//
// Computer Science 2018
//
// Spring 2018
//
// Assignment # 01
//
// Due Date:  Tuesday, February, 27
//
// Instructor: Dr. Hung-CHi Su; 
//
// Programmer: Ahmad Banisaeed
//
// Description: To simulate the memory system to run 2PMM program to sort an input
//              file with many records (integers)
//
// Input: unsorted records (integers) stored in a file
//
// Output: sorted records (integers) using 2PMM in Mem and store them in HD
//
//******************************************************************************
#include<iostream>
#include<fstream>
#include<string>
#include <stdlib.h>
#include<algorithm>
#include<unistd.h>
#include <iomanip>
#define MIN(a,b) ( (a)< (b) ? (a) :(b)); // macro to find minimum
using namespace std;
int smallest( int a, int b)  { // function to find smallest number
    int m;
    m = std::min(a,b);
    return m;
}
void merge(int arr[], int l, int m, int r) {
    int i, j, k;
    int n1 = m - l + 1;
    int n2 =  r - m;
    /* create temp arrays */
    int L[n1], R[n2];
    /* Copy data to temp arrays L[] and R[] */
    for (i = 0; i < n1; i++)
        L[i] = arr[l + i];
    for (j = 0; j < n2; j++)
        R[j] = arr[m + 1+ j];
    /* Merge the temp arrays back into arr[l..r]*/
    i = 0; // Initial index of first subarray
    j = 0; // Initial index of second subarray
    k = l; // Initial index of merged subarray
    while (i < n1 && j < n2) {
        if (L[i] <= R[j]){
            arr[k] = L[i];
            i++;
        } else {
            arr[k] = R[j];
            j++;
        }
        k++;
    }
    /* Copy the remaining elements of L[], if there
       are any */
    while (i < n1) {
        arr[k] = L[i];
        i++;
        k++;
    }
    /* Copy the remaining elements of R[], if there
       are any */
    while (j < n2) {
        arr[k] = R[j];
        j++;
        k++;
    }
}
void mergesort (int arr[], int s, int e) {
  if (s < e) {
    int mid = s + (e-s)/2;
    mergesort(arr, s,mid);
    mergesort(arr,mid+1, e);
    merge(arr,s,mid,e);
  }
}
void print_sub_list( int arr[], int n, int nn) {
    int  i = n;
    while ( i <= nn) {
        cout << arr[i] << "  ";
        i++;
    }
    cout <<"\n\n";
}
int main() {
    int i = 0;   // counter
    int Disk[8192];
    int *start = NULL;
    int *end = NULL;
    int *size = NULL;
    int *curr = NULL;
    ifstream File;   // file handler
    File.open("data.txt");
    while(!File.eof()) { // copying records from file to Disk
        i++;
        File >> Disk[i];
        File.ignore(1); // I supposed numbers are separted with colon ",". If separated by a space, no need for ignore()
    }
    i++;
    File >> Disk[i];
    Disk[0] = i - 1;   // number of records we have to sort
    File.close();
    int records = Disk[0] - 1;
    cout << "Data records from file: \n";
    for(int j = 1; j < records; j++) { // showing records on screen
        cout << Disk[j] << " ";
    }
    cout << "\n\n";   
    int M = 4; // number of Blocks
    int B = 2;  // number of Records per Block 
    int listSize = M * B;
    int sze = M * B;
    int input = M - 1;
    int listsInPhase1 = records / listSize;
    if (records % listSize > 0)
        listsInPhase1 += 1;
    cout << "Lists in phase One is " << listsInPhase1 << endl;
    start = (int *) malloc (listsInPhase1 * sizeof(int));
    end = (int *) malloc (listsInPhase1 * sizeof(int));
    size = (int *) malloc (listsInPhase1 * sizeof(int));
    curr = (int *) malloc (listsInPhase1 * sizeof(int));
    start[1]  = records + 1;
    size[1] = listSize;
    end[1] = start[1] + size[1] - 1;
    for (int i = 2; i <= listsInPhase1; i++) {
        start[i] = end[(i - 1)] + 1;
        size[i] = listSize;
        if ( i * listSize > records)
            size[i] = records - ( (i - 1) * listSize);
        end[i] = start[i] + size[i] - 1;
        }       
    cout << "Phase One *****************************************************************\n";
    cout << "lists we have to sort in memory :\n";
    for(int j = 1; j <= records; j++) {
        cout << Disk[j] << " ";
        if ( j % listSize == 0) {
            cout << "\n";
            sleep(1);
        }
    }
    cout << "\n\n";
    int x = 1;
    for (int i = 1; i <= listsInPhase1; i++)
         cout << "List " << i << " starts at " << start[i] << " and ends at " << end[i] << " with size " << size[i] << endl;
    cout << endl;
    for ( int i = 1; i <= listsInPhase1; i++) {
        int index = start[i];
        while ( index <= end[i]) {
            Disk[index] = Disk[x];
            x++;
            index++;
        }
        mergesort( Disk, start[i], end[i]);  // sorting each list in phase one using merge sort
        print_sub_list( Disk, start[i], end[i]);
        sleep(1);
    }
    cout << "\n\n";
    cout << "\n\nPhase Two  *******************************************************************\n";
    int xy = listsInPhase1;
    int xyz = listsInPhase1 / input;
    xy += xyz;
    xyz = xyz + listsInPhase1 % input;
    int xx = xyz;
    while (xx > input) {
          xx = xyz / input;
          xy += xx;
          xx = xyz % input;
    }
    xy += 1;
    cout << "total lists needed = " << xy << endl;
    int *start1 = NULL;
    int *end1 = NULL;
    int *size1 = NULL;
    int *curr1 = NULL;
    start1 = (int *) malloc (xy * sizeof(int));
    end1 = (int *) malloc (xy * sizeof(int));
    size1 = (int *) malloc (xy * sizeof(int));
    curr1 = (int *) malloc (xy * sizeof(int));
    for (int i = 1; i <= xy; i++)
         size1[i] = 0;
    start1[1]  = records + 1;
    size1[1] = M * B;
    end1[1] = start1[1] + size1[1] - 1;
    for (int i = 2; i <= listsInPhase1; i++) {
        start1[i] = end1[(i - 1)] + 1;
        size1[i] = M * B;
        if ( i * M * B > records)
            size1[i] = records - ( (i - 1) * M * B);
        end1[i] = start1[i] + size1[i] - 1;
    }
    x = 1;
    while ( x <= xy - listsInPhase1) {
           for ( int j = input * (x - 1) + 1; j <= input * x && j < xy; j++) {
               start1[x + listsInPhase1] = end1[listsInPhase1 + x - 1] + 1;
               size1[x + listsInPhase1] += size1[j];
           }
           end1[x + listsInPhase1] = start1[x + listsInPhase1] + size1[x + listsInPhase1] - 1;
           x++;
    }
    for (int i = 1; i <= xy; i++)
         cout << "List " << i << " starts at " << start1[i] << " and ends at " << end1[i] << " with size " << size1[i] << endl;
    cout << endl;
    int location = 2 * records + 1;
    int l = listsInPhase1 + 1;
    x = 1;
    int psize;
    while (x <= xy - listsInPhase1) {
        psize = size1[x + listsInPhase1];
        int* inputbuffer = NULL;
        inputbuffer = (int *) malloc (size1[x + listsInPhase1] * sizeof(int));
        int* outputbuffer = NULL;
        outputbuffer = (int *) malloc (size1[x + listsInPhase1] * sizeof(int));
        int startl = (x - 1) * input + 1;
        int lastl = startl + input - 1;
        if (lastl > xy - 1) 
            lastl = xy - 1;
        int listsincluded = lastl - startl + 1;
        int v = 0;
        int vv = 0;
        cout << "input buffer: ";
        for (int p = start1[startl]; p <= end1[lastl]; p++) {
            inputbuffer[v] = Disk[p];
            cout << inputbuffer[v] << " ";
            v++;
        }
        cout << endl << endl;
        int* subst;
        int* subend;
        int* subsize;
        int* subrem;
        int* inputblock;
        int* used;
        int* nsize;
        subst = (int *) malloc (listsincluded * sizeof(int));
        subend = (int *) malloc (listsincluded * sizeof(int));
        subsize = (int *) malloc (listsincluded * sizeof(int));
        subrem = (int *) malloc (listsincluded * sizeof(int));
        inputblock = (int *) malloc (listsincluded * sizeof(int));
        used = (int *) malloc (listsincluded * sizeof(int));
        nsize = (int *) malloc (listsincluded * sizeof(int));
        for (int p = 1; p <= listsincluded; p++) {
            subst[p] = start1[p + (x - 1) * input];
            subend[p] = end1[p + (x - 1) * input];
            subsize[p] = size1[p + (x - 1) * input];
            subrem[p] = subsize[p];
            inputblock[p] = 0;
            used[p] = 0;
        }
        cout << "lists included in list " << x + listsInPhase1 << " are: ";
        for (int ll = startl; ll <= lastl; ll++)
             cout << ll << " ";
        cout << endl;
        for (int p = 1; p <= listsincluded; p++) {
             nsize[p] = 0;
        }
        if (listsincluded == input) {
            for (int p = 2; p <= listsincluded; p++) {
                 nsize[p] += subsize[p - 1] + nsize[p - 1];
            }
        } else if (listsincluded < input) {
                  for (int p = 2; p <= listsincluded; p++) {
                       nsize[p] += subsize[p - 1] + nsize[p - 1];
                  }
        }
        int Mem[M * B] = {0};
        int memoryinputfull = 0;
        for (int n = 1; n <= listsincluded; n++) {
            for (int m = 0; m < B && subrem[n] > 0 && used[n] < subsize[n] ; m++) {
                Mem[(n - 1) * B + m] = inputbuffer[m + nsize[n]];
                subrem[n] -= 1;
                memoryinputfull += 1;
                inputblock[n] += 1;
                used[n] += 1;
            }
        }
        cout << "Memory: ";    // showing memory output at the time
        for ( int n = 0; n < M * B; n++ )
              cout << setw(4)<< Mem[n];
        cout << endl;
        int memoryoutputfull = 0;
        while ( memoryinputfull != 0) {
              int min = 32767;
              int change, loc;
              for (int n = 0; n < input; n++) {   //find the minimum of first record of each block of memory input buffer
                   if (Mem[n] != 0 && Mem[n] < min) {
                              min = Mem[n];
                              loc = n;
                   }
                   if (Mem[n + B] != 0 && Mem[n + B] < min) {
                              min = Mem[n + B];
                              loc = n + B;
                   }
              }
              for(int  n = loc;  n < loc + B -1; n++) {   // rearrange records in block after we take the minimum records
                   Mem[n] = Mem[n + 1];
                   change = n + 1;
              }
              Mem[input * B + memoryoutputfull] = min;
              memoryoutputfull += 1;
              Mem[change] = 0;
              memoryinputfull -= 1;
              int chB = change / B + 1;
              inputblock[chB] -= 1;
              int us;
              for (int mm = 1; mm <= listsincluded; mm++) {
                  if (inputblock[mm] == 0 && used[mm] < subsize[mm]) {
                                     for (int m = 0; m < B ; m++) {
                                         us = used[mm];
                                         if (used[mm] < subsize[mm]) {
                                                      Mem[(mm - 1) * B + m] = inputbuffer[used[mm] + nsize[mm]];
                                                      subrem[mm] -= 1;
                                                      memoryinputfull += 1;
                                                      inputblock[mm] += 1;
                                                      used[mm] += 1;
                                                      us = used[mm] - 1;
                                         }
                                     }
                  }
              }
              cout << "Memory: ";    // showing memory output at the time
              for ( int n = 0; n < M * B; n++ )
                   cout << setw(4) << Mem[n];
              cout << endl;
              if (memoryoutputfull == B) {
                                   for (int n = 0; n < B; n++) {
                                       outputbuffer[vv] = Mem[input * B + n];
                                       vv++;
                                       Mem[input * B + n] = 0;
                                       memoryoutputfull -= 1;
                                       }
              }  else if (memoryoutputfull > 0 && memoryoutputfull < B) {
                                   for (int n = 0; n < B; n++) {
                                       if (Mem[input * B + n] != 0) {  
                                                     outputbuffer[vv] = Mem[input * B + n];
                                                     vv++;
                                                     Mem[input * B + n] = 0;
                                                     memoryoutputfull -= 1;
                                       }
                                   }
              }
              if (memoryoutputfull == 0)
                  sleep(1); 
              cout << "Memory: ";    // showing memory output at the time
              for ( int n = 0; n < M * B; n++ )
                    cout << setw(4) << Mem[n];
                    cout << endl;  
        }
        v = 0;
        cout << "list after sort ";
        for (int p = 0; p < psize; p++) {
            cout << outputbuffer[p] << " ";
            Disk[start1[x + listsInPhase1] + v] = outputbuffer[p];
            v++;
        }
        cout << endl << endl << endl;
        free (subst);
        free (subend);
        free (subsize);
        free (subrem);
        free (inputblock);
        free (used);
        free (inputbuffer);
        free (outputbuffer);
        listsincluded = 0;
        x++;
        } // while (x <= xy - listsInPhase1) {
    return 0;
}
