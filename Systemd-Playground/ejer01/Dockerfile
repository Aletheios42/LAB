FROM ubuntu:20.04
RUN echo "nameserver 8.8.8.8" > /etc/resolv.conf
RUN echo "nameserver 1.1.1.1" >> /etc/resolv.conf
RUN apt-get update && \
  apt-get install --assume-yes systemd
RUN rm -rf /lib/systemd/system/* /etc/systemd/system/*
CMD ["/lib/systemd/systemd"]
