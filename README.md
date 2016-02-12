# esp8266-eduroam
eduroam on a esp8266 SoC

# Prerequisites

Use [esp-open-sdk](https://github.com/pfalcon/esp-open-sdk) toolchain for building firmware.

Note: use version 1.5.2 or later of the espressif sdk.

# build ESP8266 eduroam demo


    cd user
    make
    
Use esptool to burn your demo program.

# Setup authentication server

The test environment consists of an Ubuntu VM.
VirtualBox is used to host the VMs.
Vagrant is used to automate VM management.
Ansible is used for provisioning

To get started, use:

    vagrant up
    
to build the VM, and use:

    vagrant ssh

to logon. In the sequal we assume you are working on the guest VM.


# debugging

For debugging it is useful to generate additional logging.
Stop the freeradius daemon and start it in debugging mode:

    sudo invoke-rd.d freeradius stop
    sudo freeradius -X

A simple test without using EAP:

    radtest john secret 127.0.0.1 100 testing123

This should result in an Access-Accept.

## debugging EAP-TLS

    sh test/eapol_test_install.sh
    
You should now have a new binary `/usr/local/bin/eapol_test`.

To debug EAP methods, first copy the server's certificate generated during install, which (because it is self-signed)
needs to be referenced as a CA certificate.

    cp /etc/freeradius/certs/server.pem /vagrant/server_ca.pem
    
To test EAP configuration using PEAP (useful because client certificates are not involved):

    eapol_test -c /vagrant/test/eapol_test.conf.peap -a127.0.0.1 -p1812 -stesting123

To test EAP configuration using TLS:

    eapol_test -c /vagrant/test/eapol_test.conf.tls -a127.0.0.1 -p1812 -stesting123