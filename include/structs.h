/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   structs.h                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/13 20:44:10 by npizzi            #+#    #+#             */
/*   Updated: 2025/02/08 06:37:12 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef STRUCTS_H
# define STRUCTS_H

# include <stdbool.h>

# define MAX_HEREDOCS_NUM 16

typedef enum e_token_type
{
	TOK_PIPE,
	TOK_REDIR_IN,
	TOK_REDIR_OUT,
	TOK_REDIR_APP,
	TOK_HEREDOC,
	TOK_AND,
	TOK_OR,
	TOK_OPN_PAREN,
	TOK_CLS_PAREN,
	TOK_ENV_VAR,
	TOK_WILDCARD,
	TOK_WORD,
	TOK_SNGL_QT,
	TOK_DBL_QT,
	TOK_UNKNOWN
}						t_token_type;

typedef enum e_ast_type
{
	AST_PIPELINE,
	AST_COMMAND,
	AST_IO_REDIRECT,
	AST_WORD,
	AST_HEREDOC,
	AST_FILENAME,
	AST_HEREEND,
	AST_CMD_NAME,
	AST_ARG
}						t_ast_type;

typedef struct s_token_node
{
	bool				combine_next;
	bool				is_quoted;
	t_token_type		type;
	char				*value;
	struct s_token_node	*next;
}						t_token_node;

typedef struct s_ast_node
{
	t_ast_type			type;
	bool				is_quoted;
	char				*value;
	struct s_ast_node	*left;
	struct s_ast_node	*right;
}						t_ast_node;

typedef struct s_msdata
{
	bool				is_interactive;
	bool				is_pipe;
	bool				is_last;
	int					last_exit;
	int					line_num;
	int					in_fd;
	int					out_fd;
	int					pipe_fd[2];
	int					previous_fd;
	pid_t				last_pid;
	char				*heredocs[MAX_HEREDOCS_NUM];
	char				*prompt;
	char				*input;
	char				**env;
	t_token_node		*token_list;
	t_ast_node			*ast;
	bool				has_input; // !
}						t_msdata;

typedef struct s_parser
{
	t_token_node		*curr;
}						t_parser;

typedef struct s_parse_vars
{
	t_token_node		*curr_tok;
	t_token_node		*word_tok;
	t_ast_node			*curr_node;
	t_ast_node			*prefix;
	t_ast_node			*suffix;
	t_ast_node			*name;
	t_ast_node			*temp_node;
	t_ast_node			*temp_suf;
}						t_parse_vars;

#endif
