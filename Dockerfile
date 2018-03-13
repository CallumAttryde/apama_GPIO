FROM resin/rpi-raspbian

LABEL \
	name="Apama Community Core Edition" \
	arch="ARMv7hf" \
	base="rpi-raspbian" \
	maintainer="Callum Attryde (callum.attryde@softwareag.com)" \
	build-date="20180313"

COPY GPIOPlugin.cpp /opt/GPIOPlugin.cpp
COPY GPIOPlugin.h /opt/GPIOPlugin.h
COPY GPIOPlugin.mon /opt/GPIOPlugin.mon
COPY makefile /opt/makefile
COPY tutorials /opt/tutorials

RUN \
	apt-get update && \
	apt-get install -y build-essential && \
	apt-get install -y git-core && \
	git clone git://git.drogon.net/wiringPi && \
	cd wiringPi && \
	git pull origin && \
	./build

RUN apt-get install -y python-rpi.gpio

RUN \
	ACCE_BASE_URL="https://downloads.apamacommunity.com/apama-core/10.1.0.4" && \
	ACCE_TGZ_FILE="apama_core_10.1.0.4_armv7_linux.tar.gz" && \
	ACCE_INSTALL_FOLDER="/opt/apamacce" && \
	mkdir $ACCE_INSTALL_FOLDER && \
	curl -s $ACCE_BASE_URL/$ACCE_TGZ_FILE | tar -xvz -C $ACCE_INSTALL_FOLDER

ENV \
	APAMA_HOME=/opt/apamacce \
	PATH=/opt/apamacce/bin:$PATH \
	LD_LIBRARY_PATH=/opt/apamacce/lib:$LD_LIBRARY_PATH

EXPOSE 15903
