include				$(shell git rev-parse --show-toplevel)/Makefiles/setup.mk


# ***************************** CONFIGURATION ******************************** #

NAME			:=	AlCu
# MSG_PROGRESS	:=	""
MSG_USAGE		?=	"Usage: ./$(NAME)"
COLOR_MAKE		:=	STY_YEL


include				$(MAKEFILES_DIR)/config.mk
include				$(MAKEFILES_DIR)/build.mk
include				$(MAKEFILES_DIR)/doc.mk
include				$(MAKEFILES_DIR)/help.mk
