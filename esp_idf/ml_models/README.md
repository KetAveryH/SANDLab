# Model Deployment Guide:

Each model folder will contain a project that can be deployed onto an ESP32s3. However, in order to deploy these models the projects must be copied over to the ESP-DL directory (until further development).

Prerequisites:
- ESP IDF VSCode Extension >= 5.2.1
- ESP DL directory git clones
- CMake >= 3.29.*


For any project you want to deploy ensure that you copy it into the `~/ESP-DL/tutorial` folder. Before building double check that in the `sdkconfig` file `CONFIG_SPIRAM_MODE_QUAD=y` is swapped out with `CONFIG_SPIRAM_MODE_OCT=y`. From there you should be able to select the proper port, configure the proper device, build, flash, and finally monitor the project code on the ESP32.
