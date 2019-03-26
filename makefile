LIB_C = chat_client
LIB_S = chat_server

TARGET = $(LIB_C) $(LIB_S)

all:
	$(MY_TARGETS)

.PHONY: $(LIB_C)
$(LIB_C):
	cd $(LIB_C)
	make

.PHONY: $(LIB_S)
$(LIB_S):
	cd $(LIB_S)
	make

allclean:
	cd $(LIB_C)/
	make clean
	cd ..
	cd $(LIB_S)/
	make clean
