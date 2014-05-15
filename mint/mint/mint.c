//
//  mint.c
//  mint
//
//  Created by 김세훈 on 2014. 5. 15..
//  Copyright (c) 2014년 김세훈. All rights reserved.
//

#include <stdio.h>
#include "mint.h"

char** str_split(char* a_str,  char* a_delim)
{
    unsigned long size = strlen(a_str);
    int index=0;
    
    char **result;
    char *poi, *copy;
   
    copy = (char *) malloc(sizeof(char)*size);
    result = (char **) malloc(10*sizeof(char *));
    
    strcpy(copy,a_str);
    poi = strtok(copy, a_delim);
    while(poi != NULL)
    {
        result[index] = poi;
        index++;
        poi = strtok(NULL, a_delim);
    }
    
    return result;
}