#!/bin/bash -e

# script to call from Travis to build a binary and upload it to S3

ARTIFACT_PATH=Marlin/applet/Marlin.hex
export PYTHONPATH=$TRAVIS_BUILD_DIR

echo "Building for deployment"
git reset --hard
git clean -d -x -f

aws s3 cp s3://missionst-factory/uploader.tar.gz uploader.tar.gz
tar xvzf uploader.tar.gz

(cd Marlin \
    && make clean \
    && make HARDWARE_MOTHERBOARD=81 AVR_TOOLS_PATH=$AVR_TOOLS_PATH \
            ARDUINO_INSTALL_DIR=$ARDUINO_INSTALL_DIR)

python missionst/factory/storage.py \
    --local_path "$ARTIFACT_PATH" --remote_path "$DEPLOY_PATH" --upload
