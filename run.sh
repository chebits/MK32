#!/usr/bin/bash

mkdir cmakebuild || echo ''
docker run --rm -i -t --privileged -v `pwd`:/home/workspace mk32:latest /bin/bash -c "pwd && ls -lah && cmake /home/workspace && make && /bin/bash"
