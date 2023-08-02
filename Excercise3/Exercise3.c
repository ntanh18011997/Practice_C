#include <stdio.h>
#include <string.h>
typedef enum {
	add,
	sub,
	mul,
	div,
	not_valid
} operation_t;

enum
{
	SUCCESS,
	WRONG_NUMBER_OF_VALUE,
	OPERATION_IS_NOT_SUPPORT,
	WRONG_SYNTAX
};
operation_t recognize_operation(char value)
{
	switch(value)
	{
		case '+':
			return add;
		case '-':
			return sub;
		case '*':
			return mul;
		case '/':
			return div;
		default:
			return not_valid;
	}
}

float result;
operation_t operation;
int value1;
int value2;

int main(int argc, char* argv[])
{
	if(argc != 7)
	{
		printf("you supply wrong number of value");
		return WRONG_NUMBER_OF_VALUE;
	}

	for(int i=1;i < argc; i+=2)
	{
		if(strcmp(argv[i], "-o") == 0)
		{
			operation = recognize_operation(argv[i+1][0]);
			if(operation == not_valid)
			{
				printf("operator is not supported");
				return OPERATION_IS_NOT_SUPPORT;
			}
		}
		else if(strcmp(argv[i], "-n0") == 0)
		{
			int x;
			sscanf(argv[i+1], "%d", &x);
			value1 = x;
		}
		else if(strcmp(argv[i], "-n1") == 0)
		{
			int x;
			sscanf(argv[i+1], "%d", &x);
			value2 = x;
		}
		else{
			printf("wrong syntax");
			return WRONG_SYNTAX;
		}
	}

	switch(operation)
	{
		case add:
			result = value1 + value2;
			break;
		case sub:
			result = value1 - value2;
			break;
		case mul:
			result = value1 * value2;
			break;
		case div:
			result = value1 / value2;
			break;
	}
	printf("result is %f",result);
	return SUCCESS;
}
