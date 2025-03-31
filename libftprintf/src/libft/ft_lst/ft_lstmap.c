/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   ft_lstmap.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: tuthayak <tuthayak@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/19 16:44:23 by tuthayak          #+#    #+#             */
/*   Updated: 2024/12/19 16:44:23 by tuthayak         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../../inc/libft.h"

t_list	*ft_lstmap(t_list *lst, void *(*f)(void*), void (*del)(void*))
{
	t_list	*temp;
	t_list	*new_lst;
	t_list	*new_node;
	void	*new_content;

	if (!lst || !f)
		return (NULL);
	temp = lst;
	new_lst = NULL;
	while (temp)
	{
		new_content = f(temp->content);
		new_node = ft_lstnew(new_content);
		if (!new_node)
		{
			del(new_content);
			ft_lstclear(&new_lst, del);
			return (NULL);
		}
		ft_lstadd_back(&new_lst, new_node);
		temp = temp->next;
	}
	return (new_lst);
}
