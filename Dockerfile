FROM debian:stretch

SHELL ["/bin/bash", "-c"]

RUN apt-get update
RUN apt-get install -y curl xz-utils python git golang-go bsdtar wget
RUN mkdir "remarkable-keywriter"
WORKDIR "remarkable-keywriter"
RUN curl https://storage.googleapis.com/remarkable-codex-toolchain/codex-x86_64-cortexa7hf-neon-rm11x-toolchain-3.1.15.sh -o install-toolchain.sh
RUN chmod +x ./install-toolchain.sh
RUN ./install-toolchain.sh
RUN apt-get install -y qtcreator
RUN wget https://dl.google.com/go/go1.14.2.linux-amd64.tar.gz
RUN tar -xvf go1.14.2.linux-amd64.tar.gz
RUN mv go /usr/local/
ENV GOROOT=/usr/local/go
RUN mkdir -p ./go
ENV GOPATH=/remarkable-keywriter/go
ENV PATH=$GOPATH/bin:$GOROOT/bin:$PATH
RUN go get -u github.com/tcnksm/ghr
RUN wget https://toltec-dev.org/thirdparty/lib/libqsgepaper-5.15-v2.a -O /opt/codex/rm11x/3.1.15/sysroots/cortexa7hf-neon-remarkable-linux-gnueabi/usr/lib/libqsgepaper.a
ADD "https://www.random.org/cgi-bin/randbyte?nbytes=10&format=h" skipcache
COPY . /remarkable-keywriter/
RUN git submodule init
RUN git submodule update --remote
RUN apt-get install -y vim
CMD sleep 3650d
# RUN source /opt/codex/rm11x/3.1.15/environment-setup-cortexa7hf-neon-remarkable-linux-gnueabi \
# ls /opt/codex/ && ls /tmp/ && qmake edit.pro -spec linux-oe-g++ && ls /opt/codex/ && ls /tmp/ && /usr/bin/make
