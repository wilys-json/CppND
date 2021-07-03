FROM dorowu/ubuntu-desktop-lxde-vnc:latest

WORKDIR /root/project

# Install dependencies
# See: https://github.com/cpp-io2d/P0267_RefImpl/blob/P0267R8/BUILDING.md
RUN apt update && \
	apt install -y build-essential && \
	apt install -y cmake && \
	apt install -y git && \
	apt-get install -y libsdl2-dev && \
	apt install -y pkg-config
