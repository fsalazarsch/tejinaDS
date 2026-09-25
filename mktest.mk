all:
	@echo DEVKITARM=[$(DEVKITARM)]  DEVKITPRO=[$(DEVKITPRO)]  SHELL=$(SHELL)
ifeq ($(strip $(DEVKITARM)),)
	@echo EMPTY_DETECTED
endif