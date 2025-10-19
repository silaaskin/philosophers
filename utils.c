/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: silaaskin <silaaskin@student.42.fr>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2025/10/16 10:07:45 by silaaskin         #+#    #+#             */
/*   Updated: 2025/10/16 10:07:50 by silaaskin        ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo.h"

long	ft_atoi(const char *str)
{
	long	result;
	int		i;

	i = 0;
	result = 0;
	while (str[i] == ' ' || str[i] == '\t' || str[i] == '\n')
		i++;
	if (str[i] == '+')
		i++;
	if (str[i] == '-')
		return (-1);
	while (str[i])
	{
		if (str[i] >= '0' && str[i] <= '9')
		{
			result *= 10;
			result += (str[i] - '0');
			i++;
			if (result > 2147483648)
				break ;
		}
		else
			return (-1);
	}
	return (result);
}
