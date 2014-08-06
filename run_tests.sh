#!/bin/bash -e

if [ -z $AVR_TOOLS_PATH ]; then
    echo "AVR_TOOLS_PATH not set.  This could cause problems"
fi

if [ -z $ARDUINO_INSTALL_DIR ]; then
    echo "ARDUINO_INSTALL_DIR not set.  This could cause problems"
fi

function print_vars
{
    echo "Environment variables:"
    echo "   " DELTA=$DELTA
    echo "   " MOTHERBOARD=$MOTHERBOARD
    echo "   " ENABLE_AUTO_BED_LEVELING=$ENABLE_AUTO_BED_LEVELING
    echo "   " Z_SAFE_HOMING_FIXED_POINT=$Z_SAFE_HOMING_FIXED_POINT
    echo "   " CUSTOM_M_CODES=$CUSTOM_M_CODES
    echo "   " CUSTOM_G_CODES=$CUSTOM_G_CODES
    echo "   " MSM_Printeer=$MSM_Printeer
    echo "   " FSR_BED_LEVELING=$FSR_BED_LEVELING
}

function reset_vars
{
    export DELTA=
    export MOTHERBOARD=
    export ENABLE_AUTO_BED_LEVELING=
    export Z_SAFE_HOMING_FIXED_POINT=
    export CUSTOM_M_CODES=
    export CUSTOM_G_CODES=
    export MSM_Printeer=
    export FSR_BED_LEVELING=
}

function reset_configs
{
    git checkout HEAD -- Marlin/Configuration.h
    git checkout HEAD -- Marlin/Configuration_adv.h
}

function run
{
    print_vars

    if [ -n "$DELTA" ]; then
        echo "DELTA" && cp Marlin/example_configurations/delta/Configuration* Marlin/
    fi

    CFG=Marlin/Configuration.h
    cp $CFG $CFG.orig
    cat $CFG.orig | ./set-defines.sh MOTHERBOARD ENABLE_AUTO_BED_LEVELING Z_SAFE_HOMING_FIXED_POINT CUSTOM_M_CODES MSM_Printeer FSR_BED_LEVELING > $CFG
    echo "Current defines:"
    grep -e MOTHERBOARD -e ENABLE_AUTO_BED_LEVELING -e AZ_SAFE_HOMING_FIXED_POINT -e DELTA -e CUSTOM_M_CODES -e MSM_Printeer -e FSR_BED_LEVELING $CFG
    sleep 2
    (cd Marlin && make clean && make HARDWARE_MOTHERBOARD=$MOTHERBOARD AVR_TOOLS_PATH=$AVR_TOOLS_PATH ARDUINO_INSTALL_DIR=$ARDUINO_INSTALL_DIR)

    reset_vars
    reset_configs
}


reset_vars
reset_configs

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 CUSTOM_M_CODES=1 CUSTOM_G_CODES=1 MSM_Printeer=1 FSR_BED_LEVELING=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 Z_SAFE_HOMING_FIXED_POINT=1 CUSTOM_M_CODES=1 MSM_Printeer=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 Z_SAFE_HOMING_FIXED_POINT=1 CUSTOM_M_CODES=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 Z_SAFE_HOMING_FIXED_POINT=1
run

MOTHERBOARD=81 ENABLE_AUTO_BED_LEVELING=1 
run


MOTHERBOARD=81
run

MOTHERBOARD=3
run

MOTHERBOARD=3 DELTA=1
run

echo "All tests passed ok!"
