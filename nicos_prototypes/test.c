#include "../libft/libft.h"

char	*int_to_string(int in)
{
	int		c;
	int		len;
	char	*out;

	c = 0;
	c = in;
	while (c > 0)
	{
		c = c / 10;
		len++;
	}
	out = ft_calloc(len + 1, sizeof(char));
	while (in > 0)
	{
		c = in % 10;
		out[--len] = c + 48;
		in = in / 10;
	}
	return (out);
}

int main(void)
{
	int		in;
	char	*out;
	in = 987654321;
	out = int_to_string(in);
	ft_printf("%s\n", out);
}
