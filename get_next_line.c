/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   get_next_line.c                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: dabuchhe <dabuchhe@student.42lyon.fr>      +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/10 23:49:23 by dabuchhe          #+#    #+#             */
/*   Updated: 2024/12/13 03:59:34 by dabuchhe         ###   ########lyon.fr   */
/*                                                                            */
/* ************************************************************************** */

#include "get_next_line.h"

char	*ft_strncat(char *dest, char *src, int n)
{
	int	i;
	int	j;

	i = 0;
	j = 0;
	if (!src || !dest)
		return NULL;
	while (dest[i])
		i++;
	while (src[j] && (j + i < n))
	{
		dest[i + j] = src[j];
		j++;
	}
	dest[i + j] = '\0';
	return (dest);
}

void	ft_free(t_list **lst, char *stach)
{
	t_list	*tmp;

	if (!lst)
		return ;
	tmp = *lst;
	while (*lst != NULL)
	{
		tmp = (*lst)->next;
		free(*lst);
		*lst = tmp;
	}
	if (stach == NULL)
		free(stach);
}

void	add_new_node(t_list **lst)
{
	t_list	*node;

	node = calloc(1, sizeof(t_list));
	if (!node)
		return ;
	node->len = 0;
	node->end = node;
	if (*lst == NULL)
		*lst = node;
	else
	{
		(*lst)->end->next = node;
		(*lst)->end = node;
	}
	node->next = NULL;
}
char	*read_stach(t_list **lst, char *stach, bool *is_nl)
{
	size_t	i;

	i = 0;
	if (!stach)
		return (NULL);
	add_new_node(lst);
	while (stach[i] && stach[i] != '\n')
	{
		(*lst)->end->content[i] = stach[i];
		i++;
	}
	if (stach[i] == '\n')
	{
		(*lst)->end->content[i++] = '\n';
		*is_nl = true;
	}
	(*lst)->end->len = i;
	if (stach[i] == '\0')
		return (NULL);
	return (&stach[i]);
}
/// return index of \n or \0
///	return -1 if NULL
int	detect_nl(t_list **lst, char *str, bool *nl)
{
	int	i;

	if (!(*lst)->end->content)
		return (-1);
	i = 0;
	while (str[i] && str[i] != '\n')
		i++;
	if (str[i] == '\n' || str[i] == '\0')
	{
		//*nl = true;
		(*lst)->end->len = i; //////
		return (i);
	}
	//printf("OK\n");
	return(i);
}

void	read_fd(t_list **lst, int fd, bool is_nl)
{
	if (fd < 0)
		return ;
	add_new_node(lst);
	(*lst)->end->len = read(fd, (*lst)->end->content, BUFFER_SIZE);
	// if ((*lst)->end->len <= 0)
	// {
	// 	free((*lst)->end);
	// 	(*lst)->end = NULL;
	// }
}
char	*ft_substr(char const *src, unsigned int start, size_t len)
{
	char	*dst;
	size_t	i;

	i = 0;
	if (start >= strlen(src))
		return (calloc(1, 1));
	if (len > strlen(src) - start)
		len = strlen(src) - start;
	dst = malloc(sizeof(char) * len + 1);
	if (!dst)
		return (NULL);
	while (i < len)
	{
		dst[i] = src[start + i];
		i++;
	}
	dst[i] = '\0';
	return (dst);
}

char	*lst_to_string(t_list **lst, t_len len)
{
	t_list	*tmp;
	char	*dst;

	if (len.dst <= 0)
		return (NULL);
	dst = calloc((len.dst + 1), sizeof(char));
	if (!dst)
		return (NULL);
	tmp = *lst;
	while (tmp != NULL)
	{
		ft_strncat(dst, tmp->content, len.dst);
		tmp = tmp->next;
	}
	return (dst);
}

void	total_len(t_list **lst, t_len *len)
{
	t_list	*tmp;
	int	i;

	i = 0;
	tmp = *lst;
	len->dst = 0;
	while (tmp != NULL && i < 50)
	{
		len->dst += tmp->len;
		tmp = tmp->next;
		printf("len.dst = %i\n", len->dst);
		i++;
	}
}

char	*get_next_line(int fd)
{
	t_list	*lst = NULL;
	t_len	len;
	static char	*stach = NULL;
	char	*dst;
	bool	is_nl = false;
	int i = 5;

	if (fd < 0)
		return (NULL);
	stach = read_stach(&lst, stach, &is_nl);		
	while (is_nl == false && i > 0)
	{
		add_new_node(&lst);
		read_fd(&lst, fd, is_nl);
		if (detect_nl(&lst, lst->end->content, &is_nl) >= 0)
		{
			// printf("dst = %s\n", dst);
			//printf("nl = %i\n", is_nl);
			total_len(&lst, &len);
			stach = ft_substr(lst->end->content, lst->end->len + 1, BUFFER_SIZE + 1);
		}
		//i--;
	}
	printf("stach = %s\n", stach);
	dst = lst_to_string(&lst, len);
	ft_free(&lst, stach);
	return (dst);
}

int	main(void)
{
	int fd = open("test.txt", O_RDONLY);
	char *dst;
	int i = 1;
	while (1)
	{
		dst = get_next_line(fd);
		printf("--------------------------\n");
		printf("dst = %s\n", dst);
		printf("--------------------------\n");
		if (dst == NULL)
		{
			return 0;
		}
		if (dst)
		{
			printf("%i\n", i++);
			free(dst);
		}
	}
}
