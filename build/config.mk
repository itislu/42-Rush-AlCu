ifndef CONFIG_MK
CONFIG_MK		:=	1

include				$(shell git rev-parse --show-toplevel)/build/setup.mk


# ************************* DEFAULT CONFIGURATION **************************** #

MSG_PROGRESS	?=	"🔨"
COLOR_MAKE		?=	STY_GRE


# ************************** BUILD CONFIGURATION ***************************** #

#	Default targets

.DEFAULT		:
					$(MAKE) help

.DEFAULT_GOAL	:=	all


#	Directories

SRC_DIR			:=	src
LIB_DIR			:=	$(REPO_ROOT)/lib
INC_DIRS		:=	inc $(SRC_DIR) $(LIB_DIR)/libft/inc
BUILD_DIR		:=	build
OBJ_DIR			:=	$(BUILD_DIR)/_obj
DEP_DIR			:=	$(BUILD_DIR)/_dep
DOC_DIR			:=	docs
DOCKER_DIR		:=	$(REPO_ROOT)/docker


#	Dependencies

BUILDFILES		=	$(filter-out %.d,$(MAKEFILE_LIST))
LIBRARIES		:=	$(LIB_DIR)/libft
LIBRARIES_EXT	:=	ncurses


#	Flags

CC				:=	cc
CC_VERSION		:=	$(shell $(CC) --version | head -1)
IS_CLANG		:=	$(if $(findstring clang,$(CC_VERSION)),true)
CFLAGS_STD		:=	-Wall -Wextra -Werror -pedantic
CFLAGS_DBG		:=	-ggdb3
CFLAGS_SAN		:=	-fsanitize=address,undefined,bounds,float-divide-by-zero
CFLAGS_OPT		:=	-O3
CFLAGS_CLANG	:=	-Wdocumentation	# Only supported by clang
CFLAGS			?=	$(CFLAGS_STD) $(CFLAGS_DBG) $(if $(IS_CLANG),$(CFLAGS_CLANG))
CPPFLAGS		+=	$(addprefix -I,$(INC_DIRS))
DEPFLAGS		=	-M -MP -MF $@ -MT "$(OBJ_DIR)/$*.o $@"
LDFLAGS			:=	$(addprefix -L,$(LIBRARIES))
LDLIBS			:=	$(addprefix -l,$(patsubst lib%,%,$(notdir $(LIBRARIES) $(LIBRARIES_EXT))))


#	Files

SRC_EXTENSION	:=	.c
SRC				:=	$(shell find $(SRC_DIR) -type f -name "*$(SRC_EXTENSION)" -printf "%P\n")


#	Valgrind

VALGRIND		:=	$(shell which valgrind)

VALGRINDFLAGS	=	--errors-for-leak-kinds=all \
					--leak-check=full \
					--show-error-list=yes \
					--show-leak-kinds=all \
					--trace-children=yes \
					--track-origins=yes

VALGRINDFDFLAGS	:=	--track-fds=all


#	Terminal

TERMINAL		?=	$(if $(shell command -v gnome-terminal),gnome-terminal,\
					$(if $(shell command -v terminator),terminator,\
					$(if $(shell command -v xterm),xterm,\
					)))

TERMINALTITLE	:=	$(if $(filter val,$(MAKECMDGOALS)),valgrind $(NAME),\
					$(if $(filter valfd,$(MAKECMDGOALS)),valgrind-fd $(NAME),\
					$(NAME)))

TERMINALFLAGS	?=	$(if $(filter gnome-terminal,$(TERMINAL)),--title="$(TERMINALTITLE)" --,\
					$(if $(filter terminator,$(TERMINAL)),--title="$(TERMINALTITLE)" -x,\
					$(if $(filter xterm,$(TERMINAL)),-title "$(TERMINALTITLE)" -e,\
					)))


#	Requirements

ifndef NAME
    $(error NAME is not defined)
endif


endif
