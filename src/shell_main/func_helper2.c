/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   helper2.c                                          :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: mel-houa <mel-houa@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/08/27 21:45:54 by mel-houa          #+#    #+#             */
/*   Updated: 2025/08/27 21:49:58 by mel-houa         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	clean_all(void)
{
	t_clean	**cleanchild;

	cleanchild = grepclean();
	cleanup_env((*cleanchild)->env);
	gc_free_all((*cleanchild)->gc);
	if ((*cleanchild)->cmd)
		free((*cleanchild)->cmd);
	if ((*cleanchild)->pid)
		free((*cleanchild)->pid);
	free(*cleanchild);
}
