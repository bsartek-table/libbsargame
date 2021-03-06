SRCSDIR :=	srcs
INCSDIR :=	incs
BUILDDIR :=	.build

BIN :=		$(BUILDDIR)/libbsargame.so

SRCS :=		$(wildcard srcs/*.c)
OBJS :=		$(subst $(SRCSDIR), $(BUILDDIR), $(patsubst %.c,%.o,$(SRCS)))
DEPS :=		$(patsubst %.o,%.d,$(OBJS))

CPPFLAGS :=	$(addprefix -I,$(INCSDIR)) -MMD
CFLAGS :=	-W -Wall -Wcast-align -Winline -Wredundant-decls -Wshadow -Wpointer-arith \
		-Wstrict-prototypes -Wuninitialized -Wwrite-strings -fPIC
LDFLAGS :=	-shared -lpigpiod_if2 -lpthread -L. -lws2811

TARGET ?=	debug

ifeq ($(filter $(TARGET),release debug),)
$(error "target" must have the value "release" or "debug")
endif

ifeq ($(TARGET),debug)
CPPFLAGS +=	-DDEBUG=1
CFLAGS +=	-g3
endif
ifeq ($(TARGET),release)
CPPFLAGS +=	-DRELEASE=1
CFLAGS +=	-O2
endif

all:			$(BIN)
			@cp -vf $< .

$(BIN):			$(OBJS)
			$(CC) $(OUTPUT_OPTION) $^ $(LDFLAGS)

$(BUILDDIR)/%.o:	$(SRCSDIR)/%.c | $(BUILDDIR)
			$(CC) -c $(OUTPUT_OPTION) $(CPPFLAGS) $(CFLAGS) $<

$(BUILDDIR):;		@mkdir -vp $(BUILDDIR)

clean:;			@rm -vrf $(BUILDDIR)
docs:;			@doxygen Doxyfile

-include $(DEPS)

.PHONY: all clean docs
