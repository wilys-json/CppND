FROM dorowu/ubuntu-desktop-lxde-vnc:focal

WORKDIR /root/project

# Install dependencies
RUN sudo apt-key adv --fetch-keys http://repos.codelite.org/CodeLite.asc && \
	apt-add-repository 'deb https://repos.codelite.org/wx3.1.4/ubuntu/ focal universe' && \
	apt update && \
	apt install -y build-essential && \
	apt-get install -y libwxgtk3.1-dev && \
	apt install -y cmake && \
	apt install -y git

# Install VS Code
# See: https://code.visualstudio.com/docs/setup/linux
RUN curl https://packages.microsoft.com/keys/microsoft.asc | gpg --dearmor > packages.microsoft.gpg && \
	install -o root -g root -m 644 packages.microsoft.gpg /usr/share/keyrings/ && \
	sh -c 'echo "deb [arch=amd64 signed-by=/usr/share/keyrings/packages.microsoft.gpg] https://packages.microsoft.com/repos/vscode stable main" > /etc/apt/sources.list.d/vscode.list' && \
	apt-get install apt-transport-https && \
	apt-get update && \
	apt-get install code
