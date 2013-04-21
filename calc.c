#include"headerfile.h"
extern char INPUT_BUFFER[200];
extern int INPUT_BUFFER_PTR;

int isInt_s( char a ){
	if(a >= '0' && a <= '9')
		return 1;
	else
		return 0;
}
int atoi_s( char a ){
	return (int) ( a - '0');
}

int calculator( char *array ){
	int num[16],numb=0;
	short unsigned int i=0,j=0,m,n;
	char temp,op[15];
	while(*array != '\0'){
		temp = *array;
		array++;
		if( isInt_s(temp) == 1 ){
			numb = 10*numb + atoi_s(temp);
			continue;
		}
		switch(temp){
			case ' ':
				if(numb == 0)
					break;
				num[i++] = numb;
				numb = 0;
				break;
			default:
				op[j++] = temp;
				if(numb == 0)
					break;
				else{
					num[i++] = numb;
					numb = 0;
					break;
				}
		}
	}
	num[i] = numb;
	for(m= 0, n = 0 ; m < i, n < j; m++, n++){
		switch(op[n]){
			case '+':
				num[m+1] = num[m] + num[m+1]; 
				break;
			case '-':
				num[m+1] = num[m] - num[m+1];
				break;
			case '*':
				num[m+1] = num[m] * num[m+1];
				break;
			case '/':
				num[m+1] = num[m] / num[m+1];
				break;
			case '%':
				num[m+1] = num[m] % num[m+1];
				break;
			case '^':
				numb = 1;
				for(temp=0;temp<num[m+1];temp++)
					numb *=num[m];
				num[m+1] = numb;
				break;
			default:
				print("\\nUnknown operand, exiting\\n");
				return 0;
				break;
		}
	}
	return num[i];

}


calc(int arrayIndex){
		
	//print("\\narray index is@i\\n", arrayIndex);
	char array[INPUT_BUFFER_PTR-arrayIndex+1];
	//print("length of array is @i\\n", INPUT_BUFFER_PTR-arrayIndex+1);
	int i=0;
	while(arrayIndex<=INPUT_BUFFER_PTR)
	{
		array[i]=INPUT_BUFFER[arrayIndex];
		i++;
		arrayIndex++;
	}	

	//array = strdup("9 / 3 * 4");
	//puts(array);
	//puts(" ");
	//calc(array);
	int value=0;
	value=calculator(array);
	print("\\nThe value is @i\\n",value);
	return;
}


