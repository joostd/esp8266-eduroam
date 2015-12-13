# esp8266-eduroam
eduroam on a esp8622 SoC

# osx yosemite

	sudo hdiutil create ~/Documents/case-sensitive.dmg -volname "case-sensitive" -size 10g -fs "Case-sensitive HFS+"
	sudo hdiutil mount ~/Documents/case-sensitive.dmg

# prerequisites

	brew tap homebrew/dupes
	brew install binutils coreutils automake wget gawk libtool gperf gnu-sed --with-default-names grep

# build toolchain

	cd /Volumes/case-sensitive/
	git clone --recursive https://github.com/pfalcon/esp-open-sdk.git
	cd esp-open-sdk/
	make STANDALONE=y
	cd -

# esptool

	cd /Volumes/case-sensitive
	wget https://github.com/igrr/esptool-ck/releases/download/0.4.6/esptool-0.4.6-osx.tar.gz
	tar xf esptool-0.4.6-osx.tar.gz
	cp esptool-0.4.6-osx/esptool esp-open-sdk/xtensa-lx106-elf/bin/
	# export ESPTOOL=/Volumes/case-sensitive/esp-open-sdk/esptool/esptool.py 

	export PATH=$PATH:/Volumes/case-sensitive/esp-open-sdk/xtensa-lx106-elf/bin
	export ESP8266_SDK_ROOT=/Volumes/case-sensitive/esp-open-sdk/esp_iot_sdk_v1.4.0

# example

	cd /Volumes/case-sensitive
	git clone https://github.com/nkolban/Sample-ESP8266-App.git
	cd Sample-ESP8266-App/Sample\ ESP8266\ App/
	make
	make flash COMPORT=/dev/tty.usbserial-AJ030T8C 

# example 2

	cd /Volumes/case-sensitive
	git clone https://github.com/slaff/esp-hello-world.git
	cd esp-hello-world/
	make
	make flash ESPPORT=/dev/tty.usbserial-AJ030T8C 
	esptool  --port /dev/tty.usbserial-AJ030T8C write_flash 0x00000 firmware/0x00000.bin 0x40000 firmware/0x40000.bin

