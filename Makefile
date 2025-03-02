include				$(shell git rev-parse --show-toplevel)/build/setup.mk


# ***************************** CONFIGURATION ******************************** #

NAME			:=	AlCu
MSG_PROGRESS	:=	"📍"
MSG_USAGE		?=	"Usage: ./$(NAME) [file]"
COLOR_MAKE		:=	STY_GRE


include				$(MAKEFILES_DIR)/config.mk
include				$(MAKEFILES_DIR)/build.mk
include				$(MAKEFILES_DIR)/doc.mk
include				$(MAKEFILES_DIR)/help.mk
