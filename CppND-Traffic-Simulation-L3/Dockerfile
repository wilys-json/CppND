FROM dorowu/ubuntu-desktop-lxde-vnc:latest

WORKDIR /root/project

# Install dependencies
# See: https://github.com/cpp-io2d/P0267_RefImpl/blob/P0267R8/BUILDING.md
RUN apt update && \
	apt install -y build-essential && \
	apt install -y cmake && \
	apt install -y git && \
	apt install -y unzip && \
	apt install -y libgtk2.0-dev && \
	apt install -y pkg-config

# Install OpenCV
RUN	wget -O opencv.zip https://github.com/opencv/opencv/archive/master.zip && \
	unzip opencv.zip && \
	mkdir -p build && cd build && \
	cmake  ../opencv-master && \
	cmake --build .

# Set OpenCV Path
RUN export OpenCV_DIR=/root/project/build
