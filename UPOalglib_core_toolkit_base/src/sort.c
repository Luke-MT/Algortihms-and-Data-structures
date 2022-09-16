/* vim: set tabstop=4 expandtab shiftwidth=4 softtabstop=4: */

/*
 * Copyright 2015 University of Piemonte Orientale, Computer Science Institute
 *
 * This file is part of UPOalglib.
 *
 * UPOalglib is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * UPOalglib is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with UPOalglib.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <assert.h>
#include "sort_private.h"
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


void upo_insertion_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    size_t j;
    unsigned char *basePtr = base;
    for(size_t i = 1; i < n; i++){
        j = i;
       
        while(j > 0 && cmp(basePtr+j*size, basePtr+(j-1)*size) < 0){
            upo_swap(basePtr +(j*size), basePtr +((j-1)*size),size);
            j--;
        }
    }
}

void upo_merge_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/
    upo_merge_sort_rec(base,0,n-1,size,cmp);
}
void upo_merge_sort_rec(void *base, size_t low, size_t high, size_t size, upo_sort_comparator_t cmp){
    if(low >= high)
        return;
    size_t mid = (low+high)/2;
    upo_merge_sort_rec(base,low,mid,size,cmp);
    upo_merge_sort_rec(base,mid+1,high,size,cmp);
    upo_merge_sort_merge(base,low,mid,high,size,cmp);
}

void upo_merge_sort_merge(void *base, size_t low, size_t mid, size_t high, size_t size, upo_sort_comparator_t cmp){
    size_t i = 0, j = mid+1 - low;

    unsigned char *basePtr = base, 
    *aux = malloc(size * (high-low+1));
    //upo_copy    
    memcpy(aux, basePtr + low*size, (high-low+1)*size);
    
    for(size_t k = low; k <= high; k++){
        if(i > (mid - low)){
            upo_copy(basePtr + k*size, aux + j*size, size);
            j++;
        }
        else if(j > (high - low)){
            upo_copy(basePtr + k*size, aux + i*size, size);
            i++;
        }
        else if(cmp(aux + j*size, aux + i*size) < 0){
            upo_copy(basePtr + k*size, aux + j*size, size);
            j++;
        }
        else{
            upo_copy(basePtr + k*size, aux + i*size, size);
            i++;
        }
    }
    free(aux);
}

void upo_quick_sort(void *base, size_t n, size_t size, upo_sort_comparator_t cmp)
{
    /* TO STUDENTS:
     *  Remove the following two lines and put here your implementation. */
    /*fprintf(stderr, "To be implemented!\n");
    abort();*/

    upo_quick_sort_rec(base,0,n-1,size,cmp);
}

void upo_quick_sort_rec(void *base, size_t low, size_t high, size_t size, upo_sort_comparator_t cmp){
    if(low >= high)
        return;

    size_t j = upo_quick_sort_partition(base, low, high, size, cmp);

    if(j>0)
        upo_quick_sort_rec(base, low, j-1, size, cmp);
    upo_quick_sort_rec(base, j+1, high, size, cmp);
}

size_t upo_quick_sort_partition(void *base, size_t low, size_t high, size_t size, upo_sort_comparator_t cmp){
    size_t p = low, i = low, j = high;
    unsigned char *basePtr = base;
    while(1){
        while(i < high && cmp(basePtr + i*size, basePtr + p*size) < 0){
            i++;
        }
        
        while(j > low  && cmp(basePtr + j*size, basePtr + p*size) > 0)
            j--;
        
        if(i >= j)
            break;

        upo_swap(basePtr + i*size, basePtr + j*size,size);
    }
    upo_swap(basePtr + p*size, basePtr +j*size, size);
    return j;
}
