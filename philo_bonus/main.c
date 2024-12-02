/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: andjenna <andjenna@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/11/12 11:09:11 by andjenna          #+#    #+#             */
/*   Updated: 2024/12/02 17:24:03 by andjenna         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "philo_bonus.h"

void	start_simulation(t_philo *philo, t_prog *prog)
{
	int	i;
	int	status;

	i = 0;
	status = 0;
	while (i < prog->nb_of_philo)
	{
		philo[i].pid = fork();
		if (philo[i].pid == -1)
		{
			printf("Error : fork failed\n");
			while (i--)
				kill(philo[i].pid, SIGKILL);
			return ;
		}
		else if (philo[i].pid == 0)
			ft_routine(&philo[i]);
		usleep(1000);
		i++;
	}
	i = 0;
	while (i < prog->nb_of_philo)
	{
		waitpid(philo[i].pid, &status, 0);
		if (WIFEXITED(status) == 1 && WEXITSTATUS(status) == 0)
		{
			if (prog->death->__align != 1)
			{
				printf("%s%d %d %s%s\n", PURPLE, get_time_ms() - philo->prog->start,
					philo[i].id, "died", RESET);
			}
			// if (prog->has_eaten == prog->nb_time_to_eat)
			// {
			// 	printf("philo[%d].%d->nb_eaten = %d\n", i, philo[i].id, philo[i].nb_eaten);
			// 	printf("%sAll philosophers have eaten %d times%s\n", YELLOW, prog->nb_time_to_eat, RESET);
			// }
			// printf("Philosophe %d a terminé avec le code %d\n", philo[i].id, WEXITSTATUS(status));
			terminate_process(prog);
			break ;
		}
		i++;
	}
}

static int	ft_parse_args(int ac, char **av)
{
	if (ac < 5 || ac > 6)
	{
		printf("Error : wrong number of arguments\n");
		return (1);
	}
	else if (ft_parse_params(ac, av))
		return (1);
	return (0);
}

int	main(int ac, char **av)
{
	t_prog	prog;
	t_philo	*philo;

	if (ft_parse_args(ac, av) || ft_init_prog(&prog, ac, av))
		return (1);
	philo = malloc(sizeof(t_philo) * prog.nb_of_philo);
	if (!philo)
	{
		ft_clean_sem(&prog);
		printf("Error : malloc failed\n");
		return (1);
	}
	ft_init_philo(philo, &prog);
	prog.philo = philo;
	prog.start = get_time_ms();
	philo->last_meal = prog.start;
	start_simulation(philo, &prog);
	ft_free(&prog, philo);
	return (0);
}
