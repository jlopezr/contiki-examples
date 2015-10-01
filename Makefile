CONTIKI = ../..
ifndef TARGET
TARGET=sky
endif

#all: 0-hello 1-blink 2-etimer 3-twoetimer 4-ctimer 5-rtimer 6-1proto 7-2proto 8-button 9-lightsensor 10-button+thread 11-button+2process #sky-collect rt-leds test-button test-cfs tcprudolph0
all: udp-client

UIP_CONF_IPV6=1

%.tgz: %.ihex
	mkdir $(basename $<) ; \
	mv $< $(basename $<) ; \
	echo $(basename $<)/$(basename $<).ihex 600 > $(basename $<)/runfile ; \
	tar czf $@ $(basename $<)

%.class: %.java
	javac $(basename $<).java

viewrssi: ViewRSSI.class
	make login | java ViewRSSI

include $(CONTIKI)/Makefile.include
