//
//  main.c
//  mint
//
//  Created by 김세훈 on 2014. 5. 15..
//  Copyright (c) 2014년 김세훈. All rights reserved.
//

#include <stdio.h>
#include "mint.h"

int main(int argc, const char * argv[])
{
    char** test = str_split("ABC\nDEF\nGHI", "\n");
    
    printf("%s\n",test[0]);
    if(strcmp("ABC", test[0]) == 0)
    {
        printf("맞음");
    }
    
}

    
    
    
    
    


