/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils6.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: jdupuis <jdupuis@student.42perpignan.fr    +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/05/21 02:16:42 by jdupuis           #+#    #+#             */
/*   Updated: 2025/05/21 02:48:04 by jdupuis          ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "minishell.h"

void	*ft_realloc(void *ptr, size_t size)
{
	void	*new_ptr;

	if (ptr == NULL)
		return (malloc(size));
	if (!size)
		return (ptr);
	new_ptr = malloc(size);
	ft_memcpy(new_ptr, ptr, size);
	free(ptr);
	return (new_ptr);
}

static char	*__sub_write_digits(char *ptr, unsigned int num)
{
	char	num_buf[20];
	int		len;

	len = 0;
	if (num == 0)
		*ptr++ = '0';
	else
	{
		while (num > 0)
		{
			num_buf[len++] = '0' + (num % 10);
			num /= 10;
		}
		while (len > 0)
			*ptr++ = num_buf[--len];
	}
	return (ptr);
}

static char	*__sub_handle_integer(char *ptr, va_list ap)
{
	int				num;
	unsigned int	u_num;

	num = va_arg(ap, int);
	if (num < 0)
	{
		*ptr++ = '-';
		u_num = (unsigned int)(~num + 1);
	}
	else
		u_num = (unsigned int)num;
	return (__sub_write_digits(ptr, u_num));
}

int	ft_vsprintf(char *str, const char *fmt, va_list ap)
{
	t_format	f;

	f.i = 0;
	f.ptr = str;
	while (fmt[f.i])
	{
		if (fmt[f.i] == '%')
		{
			f.i++;
			if (fmt[f.i] == 's')
				f.ptr = __sub_handle_string(f.ptr, ap);
			else if (fmt[f.i] == 'd')
				f.ptr = __sub_handle_integer(f.ptr, ap);
			else
				*f.ptr++ = fmt[f.i];
		}
		else
			*f.ptr++ = fmt[f.i];
		f.i++;
	}
	*f.ptr = '\0';
	return (f.ptr - str);
}

int	ft_sprintf(char *str, const char *fmt, ...)
{
	va_list	ap;
	int		ret;

	va_start(ap, fmt);
	ret = ft_vsprintf(str, fmt, ap);
	va_end(ap);
	return (ret);
}
