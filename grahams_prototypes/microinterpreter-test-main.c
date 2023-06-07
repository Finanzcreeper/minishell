#include "micro.h"

int main(int argc, char **argv, char **env)
{
	t_node nodes[7];
	t_list *cmd_elements1;
	t_list *cmd_elements2;
	t_list *cmd_elements3;
	t_list *cmd_elements4;

	(void)argc;
	(void)argv;

	cmd_elements1 = NULL;
	cmd_elements2 = NULL;
	cmd_elements3 = NULL;
	cmd_elements4 = NULL;

	ft_lstadd_back(&cmd_elements1, ft_lstnew("ls"));
	ft_lstadd_back(&cmd_elements2, ft_lstnew("grep"));
	ft_lstadd_back(&cmd_elements2, ft_lstnew("micro"));
	ft_lstadd_back(&cmd_elements3, ft_lstnew("sort"));
	ft_lstadd_back(&cmd_elements3, ft_lstnew("-r"));	
	ft_lstadd_back(&cmd_elements4, ft_lstnew("wc"));
	ft_lstadd_back(&cmd_elements4, ft_lstnew("-l"));

	nodes[0].type = N_PIPE;
	nodes[1].type = N_PIPE;
	nodes[2].type = N_CMD;
	nodes[3].type = N_PIPE;
	nodes[4].type = N_CMD;
	nodes[5].type = N_CMD;
	nodes[6].type = N_CMD;

	nodes[0].left = &nodes[1];
	nodes[0].right = &nodes[2];
	nodes[1].left = &nodes[3];
	nodes[1].right = &nodes[4];
	nodes[3].left = &nodes[5];
	nodes[3].right = &nodes[6];		

	nodes[2].left = NULL;
	nodes[2].right = NULL;
	nodes[4].left = NULL;
	nodes[4].right = NULL;
	nodes[5].left = NULL;
	nodes[5].right = NULL;
	nodes[6].left = NULL;
	nodes[6].right = NULL;
			
	nodes[5].cmd_elements = cmd_elements1;
	nodes[6].cmd_elements = cmd_elements2;
	nodes[4].cmd_elements = cmd_elements3;
	nodes[2].cmd_elements = cmd_elements4;

	// traverse_ast(&nodes[5], env); // 1st cmd: ls
	// traverse_ast(&nodes[3], env); // 1st pipe: ls | grep micro
	// traverse_ast(&nodes[1], env); // 1st and 2nd pipe : ls | grep micro | sort -r
	// traverse_ast(&nodes[0], env); // 1st 2nd and 3rd pipe : ls | grep micro | sort -r | head -n 2
	
	// traverse_ast(&nodes[6], env); // second command only HANGS
	// traverse_ast(&nodes[4], env); // third command only HANGS
	traverse_ast(&nodes[2], env); // fourth command only HANGS
}