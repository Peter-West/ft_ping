#******************************************************************************#
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: ppellegr <ppellegr@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2015/06/04 17:52:30 by ppellegr          #+#    #+#              #
#    Updated: 2015/06/04 17:52:30 by ppellegr         ###   ########.fr        #
#                                                                              #
#******************************************************************************#

NAME				:= ft_ping
SRCS				:= main.c
PATH_OBJ			:= objs
PATH_SRC			:= srcs
PATH_INC			:= incs
CXX					:= clang-3.5
CXXFLAGS			:= -Wall -Wextra -Werror
DEBUG				:= -g
OBJECTS				:= $(patsubst %.c, $(PATH_OBJ)/%.o, $(SRCS))
INCLUDES			:= -I./$(PATH_INC)
LIBFT				:= libft/libft.a
INC_LIBFT			:= -I libft/includes

.PHONY: all clean fclean re

all: $(NAME)

build:
	@($(MAKE) -C ./libft)

$(NAME): $(OBJECTS)
	$(CXX) -o $@ $(OBJECTS) $(DEBUG) $(CXXFLAGS) $(LIBFT)

$(PATH_OBJ)/%.o: $(addprefix $(PATH_SRC)/,%.c)
	@mkdir -p $(PATH_OBJ)
	$(CXX) -c -o $@ $(CXXFLAGS) $^ $(DEBUG) $(INCLUDES) $(INC_LIBFT)

clean:
	rm -f $(OBJECTS)

fclean: clean
	rm -f $(NAME)

re: fclean $(NAME)