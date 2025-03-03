/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/09/11 21:22:17 by thopgood          #+#    #+#             */
/*   Updated: 2025/02/05 11:49:49 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H
# include "../libft/include/libft.h"
# include "error_msgs.h"
# include "structs.h"
# include <dirent.h>
# include <errno.h>
# include <fcntl.h>
# include <limits.h>
# include <readline/history.h>
# include <readline/readline.h>
# include <signal.h>
# include <stdbool.h>
# include <stddef.h>
# include <stdio.h>
# include <stdlib.h>
# include <string.h>
# include <sys/stat.h>
# include <sys/types.h>
# include <sys/wait.h>
# include <unistd.h>

// * NUMERIC MACROS *
# define CHILD_PROCESS_ID 0
# define KEY_NOT_EXIST 2
# define READ_END 0
# define WRITE_END 1
# define FD_CLOSED -1
# define SINGLE_QUOTE 39
# define DOUBLE_QUOTE 34
# define FNAME_MAXLEN 256

// * GLOBAL SIGNAL VARIABLE *
extern sig_atomic_t	g_sig_received;

// * DEFAULT PATH *
# define DEFAULT_PATH "PATH=/usr/local/bin:/usr/bin:/bin"

// * CLI *
int					get_prompt(t_msdata *msdata);
void				process_user_input(t_msdata *msdata);
void				execute_command(t_msdata *ms_data, char **command_array);
char				**create_argv(t_ast_node *command_node);
bool				is_only_spaces(char *input);
void				free_msdata(t_msdata *msdata);
void				reset_default_values(t_msdata *msdata);
char				*array_last(char **array);
void				update_shell_level(t_msdata *msdata);

// * BUILT-INS COMMANDS *
int					ms_cd(t_msdata *ms_data, char **args);
int					ms_echo(t_msdata *ms_data, char **args);
int					ms_env(t_msdata *ms_data);
int					ms_export(t_msdata *ms_data, char **args);
int					ms_pwd(t_msdata *msdata);
void				ms_exit(t_msdata *ms_data, char **args);
long long			ft_atoill(const char *nptr, bool *out_of_range);
int					ms_unset(char **environment, char **args);

// * BUILT-INS UTILS *
bool				is_identifier_valid(char *identifier);
int					get_key_len(char *variable);
int					update_pwd(t_msdata *msdata);
int					update_var_value_overwrite(char *variable, char **env);
int					update_variable_value(char *variable, char **environment);
char				*get_env_value(char *key, char **env);
char				**add_var_to_env(char *variable, char **envp);
char				**sort_array(char **environ_array);
char				**copy_env(t_msdata *msdata, char **environment);
char				**copy_env_if(t_msdata *msdata, char **env);

// * TOKENISE *
t_token_node		*tokenise(t_msdata *msdata);
void				tokenise_metachar(char **input, t_msdata *msdata);
void				tokenise_paren(char **input, t_msdata *msdata);
void				tokenise_quotes(char **input, t_msdata *msdata);
void				tokenise_redir(char **input, t_msdata *msdata);
bool				has_unclosed_quotes(char *token);
bool				is_metachar(char c);
bool				is_env_var_valid_char(char c);
void				combine_tokens(t_msdata *msdata, t_token_node *list);
void				lst_append(t_token_node **lst, t_token_node *new);
t_token_node		*lstlast(t_token_node *lst);
t_token_node		*new_token_node(char *value, t_token_type type,
						t_msdata *msdata);
void				free_list(t_token_node **lst);

// * EXPANSION *
char				*expand_quoted_var(char *token, t_msdata *msdata);
void				expand_tokens(t_token_node **tokens_list, t_msdata *msdata);
void				remove_quotes_in_place(char **str);
void				update_tokens_list(t_token_node **head_of_list);
void				split_word(t_token_node **token, t_msdata *msdata);
void				safe_free_array(char ***arr);
void				expand_wildcards(t_token_node **tokens_list,
						t_msdata *msdata);
void				add_token_in_place(char *value, t_token_type tok_type,
						t_token_node **node, t_msdata *msdata);

// * PARSE *
t_ast_node			*parse(t_msdata *msdata, t_token_node *tokens);
t_ast_node			*parse_pipeline(t_msdata *msdata, t_parse_vars *vars);
t_ast_node			*parse_command(t_msdata *msdata, t_parse_vars *vars);
void				free_ast(t_ast_node *node);
t_ast_node			*create_ast_node(t_msdata *msdata, t_ast_type type,
						char *value);
t_token_node		*consume_token(t_token_node **tokens);
int					is_redir(t_token_type tok_type);
t_ast_node			*parse_io_redirect(t_msdata *msdata, t_token_node **tokens);
t_ast_node			*parse_cmd_name(t_msdata *msdata, t_token_node **tokens);
t_ast_node			*parse_cmd_prefix(t_msdata *msdata, t_token_node **tokens);
t_ast_node			*parse_cmd_suffix(t_msdata *msdata, t_parse_vars *vars);

// * EXECUTION *
void				wait_and_update_exit(pid_t pid, int *exit_status,
						t_msdata *msdata);
bool				has_to_be_forked(t_ast_node *command_node,
						t_msdata *msdata);
bool				is_directory(char *path);

// * HEREDOC *
void				adjust_heredoc_delimiter(t_token_node *list);
int					check_for_heredocs(t_ast_node *node, t_msdata *msdata);
int					count_heredocs(t_token_node *tokens_list);
void				generate_temp_file(t_msdata *msdata);
void				unlink_all_heredocs(t_msdata *msdata);

// * SIGNALS *
void				install_default_handler(void);
void				install_heredoc_signals(void);
void				install_children_signals(void);

// * UTILS *
void				safe_free(char **ptr);
void				traverse_tree(t_ast_node *head_of_three, t_msdata *msdata);

// * FILE HANDLING *
void				dup2_io(t_msdata *msdata);
void				close_safe(int fd);
int					redirect_input(t_ast_node *redirection_node,
						t_msdata *msdata);
int					redirect_output(t_ast_node *redirection_node,
						t_msdata *msdata);

// * ERROR HANDLING *
void				*safe_calloc(t_msdata *msdata, size_t n);
void				fatal_error(t_msdata *msdata, char *err_msg, int errcode);
void				print_err(char *str);

#endif
