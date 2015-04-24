#include<stdio.h>
#include<stdlib.h>
int $$state_num=0;
int y;

int main() {
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
$$current=0;
$$changeState();
if ($$flag[$$current-1]==0){printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=0;
} else {printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
y=1;
}
;
printf("state %d:", $$state_num);
printf("Global: ");
printf("y=%d ",y);
printf("\n");
$$state_num++;
return 0;
}
