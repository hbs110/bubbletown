#!/bin/sh

docker run -it -v `pwd`/../../:/bubble_src bubble/build /bubble_src/games/02-bubble-town/docker_build_android.sh
