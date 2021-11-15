FROM debian:latest

# Dependencies
# Maybe it could be used for a build environment?
# but for a production set, some packages might not be applied
RUN apt-get update && apt-get install -y wget gcc\
	libncurses5-dev\
	autoconf make build-essential

# creating snakett user
RUN useradd -rm -d /home/snakett -s /bin/bash -g root -G sudo -u 1001 snakett -p snakett
