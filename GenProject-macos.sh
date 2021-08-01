# /bin/bash


if [ ! -d"./macos" ];then
  mkdir ./macos
else
  rm -rf ./macos
  cmake ..
fi