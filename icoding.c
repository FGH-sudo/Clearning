#include <stdio.h>

int main()
{
	int i0, i1, i2, i3, b;
	char buf1[128], a;
	char buf2[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};
	char buf3[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};

	printf("Enter message to be encrypted: \n");
	b = 0;
	do
	{
		a = getchar();
		buf1[b] = a;
		b++;
	} while (a != '\n');
	b -= 1;
	buf1[b] = '\0';
	printf("Enter shift amount (1-25): ");
	scanf("%d", &i0);

	for (i1 = 0; i1 < (sizeof(buf1) - 1); i1++)
	{
		for (i2 = 0; i2 < 26;)
		{
			if ((buf1[i1]) == (buf2[i2]) && (i2 + i0) < 26)
			{
				i3 = i2 + i0;
				buf1[i1] = buf2[i3];
				break;
			}
			else if ((buf1[i1]) == (buf2[i2]) && (i2 + i0) >= 26)
			{
				i3 = i2 + i0 - 26;
				buf1[i1] = buf2[i3];
				break;
			}
			else if ((buf1[i1]) == (buf3[i2]) && (i2 + i0) < 26)
			{
				i3 = i2 + i0;
				buf1[i1] = buf3[i3];
				break;
			}
			else if ((buf1[i1]) == (buf3[i2]) && (i2 + i0) >= 26)
			{
				i3 = i2 + i0 - 26;
				buf1[i1] = buf3[i3];
				break;
			}
			else
			{
				i2++;
			}
		}
	}

	printf("Encrypted message: %s\n", buf1);

	return 0;
}