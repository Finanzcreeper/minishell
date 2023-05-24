
#include "proto.h"

void	double_quoter(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	while (string[sain->c] != '"')
	{
		if (strin[sain->c] == '$')
		{
		
		}
		sain->substring[sain->k++] = string[sain->c++];
	}
	sain->substring[sain->k++] = string[sain->c++];
	pushcurrentsub(sain, string, list, defs);
}

void	single_quoter(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	while (string[sain->c] != 39)
	{
		sain->substring[sain->k++] = string[sain->c++];
	}
	sain->substring[sain->k++] = string[sain->c++];
	pushcurrentsub(sain, string, list, defs);
}

void	quote_handler(t_sain *sain, char *string, t_token **list, t_defs defs)
{
	pushcurrentsub(sain, string, list, defs);
	if (string[sain->c] == '"')
	{
		sain->substring[sain->k++] = string[sain->c++];
		double_quoter(sain, string, list, defs);
	}
	if (string[sain->c] == 39)
	{
		sain->substring[sain->k++] = string[sain->c++];
		single_quoter(sain, string, list, defs);
	}
}
