#
# Copyright (c) 2001, 2002 Swedish Institute of Computer Science.
# All rights reserved.
#
# Redistribution and use in source and binary forms, with or without modification,
# are permitted provided that the following conditions are met:
#
# 1. Redistributions of source code must retain the above copyright notice,
#    this list of conditions and the following disclaimer.
# 2. Redistributions in binary form must reproduce the above copyright notice,
#    this list of conditions and the following disclaimer in the documentation
#    and/or other materials provided with the distribution.
# 3. The name of the author may not be used to endorse or promote products
#    derived from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR IMPLIED
# WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
# MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT
# SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
# EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT
# OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
# INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
# CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING
# IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY
# OF SUCH DAMAGE.
#
# This file is part of the lwIP TCP/IP stack.
#
# Author: Adam Dunkels <adam@sics.se>
#

all compile: example_app makefsdata
.PHONY: all

LWIPDIR=../../../../src

include ../Common.mk

CFLAGS+=-I$(CONTRIBDIR)/examples/example_app

TESTFLAGS?=
CFLAGS+=$(TESTFLAGS)

MAKEFSDATAOBJS=$(notdir $(MAKEFSDATAFILES:.c=.o))

clean:
	rm -f *.o $(LWIPLIBCOMMON) $(APPLIB) example_app makefsdata *.s .depend* *.core core

depend dep: .depend

include .depend

.depend: $(CONTRIBDIR)/examples/example_app/test.c default_netif.c $(LWIPFILES) $(APPFILES) $(MAKEFSDATAFILES)
	$(CCDEP) $(CFLAGS) -MM $^ > .depend || rm -f .depend

example_app: .depend $(LWIPLIBCOMMON) $(APPLIB) default_netif.o test.o
	$(CC) $(CFLAGS) -o example_app test.o default_netif.o -Wl,--start-group $(APPLIB) $(LWIPLIBCOMMON) -Wl,--end-group $(LDFLAGS)

makefsdata: .depend $(MAKEFSDATAOBJS)
	$(CC) $(CFLAGS) -o makefsdata $(MAKEFSDATAOBJS)
