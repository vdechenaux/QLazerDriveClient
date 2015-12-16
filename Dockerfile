FROM debian:8

RUN apt-get update
RUN apt-get install -y build-essential qt5-qmake qt5-default libqt5websockets5-dev

RUN mkdir /code
COPY . /code/
WORKDIR /code
RUN qmake
RUN make

ENTRYPOINT ["/bin/bash"]
