#!/bin/sh

sudo apt-get -y install build-essential libssl-dev libnl-dev
pushd /tmp
wget http://w1.fi/releases/wpa_supplicant-2.5.tar.gz
tar xzf wpa_supplicant-2.5.tar.gz 
cd wpa_supplicant-2.5/wpa_supplicant/
sed '/CONFIG_EAPOL_TEST=y/s/^#//g' < defconfig >.config
make eapol_test
cp eapol_test /usr/local/bin
popd
