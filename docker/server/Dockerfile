FROM debian:9.4

WORKDIR /herc_server
# ADD . /herc_server
RUN apt-get update && apt-get -y install \
	make \
	gcc \
	mysql-server \
	default-libmysqlclient-dev \
	 zlib1g-dev \
	libpcre3-dev \
	git

RUN git clone --shallow-since 11.03.2018 https://github.com/HerculesWS/Hercules.git . \
	&& git checkout d89690fbdbaa5dc78f98d96ee91403e329c12af1
COPY ./wait-for-it.sh /herc_server/wait-for-it.sh
COPY ./npc /herc_server/npc
COPY ./plugins /herc_server/plugins
COPY ./config/general/import /herc_server/conf/import
COPY ./config/general/groups.conf /herc_server/conf/
COPY ./config/general_comm /herc_server/conf/import
COPY ./config/comm/sql_connection.conf /herc_server/conf/global/

# work around. FIX ME!
# COPY ./src/mmo.h /herc_server/src/common/

# db import missing, not tested, low prio
COPY ./db/pre-re/exp.txt /herc_server/db/pre-re

RUN ./configure --enable-packetver=20151029 --disable-renewal CPPFLAGS="-DMAX_LEVEL=255"\
	&& make clean \
	&& make sql \
	&& groupadd -g 999 appuser \
	&& useradd -r -u 999 -g appuser appuser \
	&& chown appuser /herc_server

USER appuser
#RUN more config/global/sql_connection.conf
