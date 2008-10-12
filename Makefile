#!/usr/bin/make
#
# Copyright 2006 Sony Computer Entertainment Inc.
#
# Licensed under the MIT Open Source License, for details please see license.txt or the website
# http://www.opensource.org/licenses/mit-license.php
# 


ifeq ($(COLLADA_DOM_PLATFORM),)
COLLADA_DOM_PLATFORM = PC
endif

ifeq ($(COLLADA_DOM_VERSION),)
export COLLADA_DOM_VERSION = 1.4
endif

MAKEFILE_NAME = $(addsuffix $(COLLADA_DOM_PLATFORM),Makefile_)

include $(MAKEFILE_NAME)

