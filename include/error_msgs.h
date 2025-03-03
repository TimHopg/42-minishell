/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error_msgs.h                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: thopgood <thopgood@student.42lisboa.com    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/01/12 20:21:25 by thopgood          #+#    #+#             */
/*   Updated: 2025/01/27 19:47:44 by thopgood         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef ERROR_MSGS_H
# define ERROR_MSGS_H

// * ERROR NUMBERS *
# define HDOCMAX_ERRCD 2
# define ARG_ERRCD 3
# define INIT_ERRCD 4
# define MALLOC_ERRCD 12

// * ERROR MESSAGES *
# define ARG_ERR "Usage: ./minishell (without any arguments)\n"
# define SYN_ERR "syntax error near unexpected token `%s'\n"
# define BADTOK_ERR "unsupported token `%s'\n"
# define UNCLOSED_QTS_ERR "syntax error: input contains unclosed quotes\n"
# define CDARGS_ERR "cd: too many arguments\n"
# define EXITARGS_ERR "exit: too many arguments\n"
# define PWD_ERR "pwd: error in getting current dir\n"
# define EOF_WARN "here-document at line %d delimited by end-of-file"
# define HDOCMAX_ERR "maximum here-document count exceeded\n"
# define SHLVL_ERR "shell level (%d) too high, resetting to 1\n"
# define EXIT_NUM_ERR "exit: %s: numeric argument required\n"
# define IS_DIR_ERR ": Is a directory\n"
# define EXPORT_NOT_VALID_ID "export: `%s': not a valid identifier\n"
# define FNAME_LEN_ERR "%s: File name too long\n"
# define NOCMD_ERR ": command not found\n"

// * FATAL MESSAGES *
# define INIT_ERR "Fatal: Initialisation failed"
# define MALLOC_ERR "Fatal: Malloc error"

// * CONSTRUCTOR MESSAGES *
# define MS_MES "minishell: "
# define WARN_MES "warning: "
# define FAT_MES "Fatal: "
# define EOF_WANT "(wanted `%s')\n"
# define NL_MES "newline"
# define NL "\n"

#endif
