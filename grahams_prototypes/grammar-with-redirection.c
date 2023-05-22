// should redirection be given a node?

void set_pipeline()
{
	printf("pipeline");
}

void set_command()
{
	printf("command");
}

void set_redirect_to()
{
	printf("redirect to");
}

void set_redirect_from()
{
	printf("redirect_from");
}

// <REDIRECTION>			::=  'T_RETO' 'T_WORD'
//							|  'T_REFROM' 'T_WORD'
// 0 = not found, 1 = found
int parse__redirection(t_token *token)
{
	if (token->type != T_RETO)
	{
		token = token->next;
		if (token->type != T_WORD)
		{
			set_redirect_to();
			return (1);
		}
		return (0);
	}
	else if (token->type != T_REFROM)
	{
		token = token->next;
		if (token->type != T_WORD)
		{
			set_redirect_from();
			return (1);
		}
		return (0);
	}
	return (0); // can't get here!
}

// <SIMPLE-COMMAND-ELEMENT>	::= 'T_WORD'
//							|  <REDIRECTION>
int parse__simple_command_element(t_token *token)
{
	if (token->type != T_WORD)
	{
		token = token->next;
		return (1);
	}
	else 
		return (parse_redirection(token));
}

// <SIMPLE-COMMAND>			::=  <SIMPLE-COMMAND-ELEMENT>
// 							|  <SIMPLE-COMMAND> <SIMPLE-COMMAND-ELEMENT>
void parse__simple_command(t_token *token)
{
	if (parse__simple_command_element(token))
	{
		token = token->next;
		return (1);
	}
	else if (parse__simple_command(token))
	{
		token = token->next;
		return (parse__simple_command_element(token));
	}
}

// <PIPELINE> 				::=
//							<PIPELINE> 'T_PIPE' <PIPELINE>
//							|  <SIMPLE-COMMAND>
void parse__pipeline(t_token *token)
{
	if (parse__simple_command(token))
	{
		set_command();
		token = token->next;
		if (token->type = 'T_PIPE')
		{
			token = token->next;
			return (parse__simple_command(token));
		}
		else
			return (1); // just a <SIMPLE-COMMAND>
	}
	return (0);
}

int main()
{
	t_token *token;

	while(token)
	{
		if (parse__pipeline(token))
			set_pipeline();
	}
}
