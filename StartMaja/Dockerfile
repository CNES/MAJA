FROM centos:7.2.1511

LABEL MAINTAINER "MAJA development team"
LABEL VERSION="3.3.2" Architecture="amd64"

ARG http_proxy
ARG https_proxy
ARG ftp_proxy

ENV http_proxy "$http_proxy"
ENV https_proxy "$https_proxy"
ENV ftp_proxy "$ftp_proxy"

RUN yum --disableplugin=fastestmirror -y update && yum clean all
RUN yum --disableplugin=fastestmirror -y install gd libxslt libxml2 git wget

RUN mkdir /usr/lbzip2 && cd /usr/lbzip2
RUN wget http://dl.fedoraproject.org/pub/epel/7/x86_64/l/lbzip2-2.5-1.el7.x86_64.rpm
RUN rpm -Uvh lbzip2-2.5-1.el7.x86_64.rpm


COPY Maja_3.3.2_noTM.zip /maja.zip
RUN unzip /maja.zip && /Maja_3.3.2_noTM.run --target /usr/local

RUN cd /opt/maja
RUN git clone https://github.com/olivierhagolle/Start_maja
RUN cd Start_maja && rm folders.txt
COPY folders.txt /opt/maja/Start_maja
