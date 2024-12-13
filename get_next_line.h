#ifndef GET_NEXT_LINE_H
# define GET_NEXT_LINE_H

# include <fcntl.h>
# include <unistd.h>
# include <stdlib.h>
# include <stdio.h>
# include <fcntl.h>
# include <string.h>
# include <stdbool.h>

# ifndef BUFFER_SIZE
#  define BUFFER_SIZE 7
# endif



typedef	struct s_list
{
	int	len;
	char	content[BUFFER_SIZE + 1];	// contient la string extraite de buff
	struct s_list	*end; // pointe sur le dernier element de la liste
	struct s_list	*next; // pointe sur le prochain element de la liste
}	t_list;

typedef struct s_len
{
	int	read;
	int	to_nl;
	int	dst;
}	t_len;

char	*get_next_line(int fd);
void	ft_free(t_list **lst, char *stach);
size_t	ft_strlen(const char *s);
char	*ft_strncat(char *dest, char *src, int n);
int	len_to_next_delimiter(const char *str);
char	*ft_strchr(const char *str, int c);
int	stach_to_node(t_list **lst, t_len *len, char **stach);
int	fd_to_node(t_list **lst, t_len *len, int fd);
char	*lst_to_string(t_list **lst, t_len len);
void	init_struct(t_len *len, char *buff);

#endif
